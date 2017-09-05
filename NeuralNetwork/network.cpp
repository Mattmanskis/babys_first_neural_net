#include "stdafx.h"
#include "network.h"
#include <random>
#include <time.h>
#include <math.h>
#include <utility>
std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<float> random_0_1(0.0f, 1.0f);
std::uniform_real_distribution<float> random_5_5(-.5f, .5f);
std::uniform_int_distribution<int> random_int_0_8(0, 8);

float activation(float x)
{
	return (2 / (1 + exp(-2 * x))) -1;
}

float d_activation(float x)
{
	return 1 - activation(x) * activation(x);
}

std::vector<float> network_group::output(std::vector<float> input)
{
	t_network[0] = input;
	for (int layer = 1; layer < network.size(); layer++)
	{
		t_network[layer][0] = network[layer][0][0];
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			t_network[layer][neuron] = 0;
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				t_network[layer][neuron] += network[layer][neuron][connection] * t_network[layer-1][connection];
			}
			t_network[layer][neuron] = activation(t_network[layer][neuron]);
		}
	}
	for (int neuron = 0; neuron < network[network.size()-1].size(); neuron++)	//output layer has no bias neurons, and can't be computed in the same loop as others
	{
		t_network[network.size()-1][neuron] = 0;
		for (int connection = 0; connection < network[network.size() - 1][neuron].size(); connection++)
		{
			t_network[network.size() - 1][neuron] += network[network.size() - 1][neuron][connection] * t_network[network.size() - 2][connection];
		}
		t_network[network.size() - 1][neuron] = activation(t_network[network.size() - 1][neuron]);
	}
	return(t_network[t_network.size() - 1]);
}

void network_group::backprop(std::vector<float> input, std::vector<float> e_output, float training_weight)
{
	//runs network normally to find the activated values of all neurons
	output(input);
	//finds delta of the last layer based on expected output and saves it in the last layer of error net
	for (int x = 0; x < e_output.size(); x++)
	{
		float z = 0;
		//computes z by adding all activated values of previous layer
		for (int output = 0; output < network[network_specs[0] - 2].size(); output++)
			z += t_network[network_specs[0] - 2][output];
		error_net[network_specs[0] - 1][x] =  (t_network[network_specs[0]-1][x] - e_output[x]) * d_activation(z);
	}

	//computes error of last hidden layer seperately (output layer has no bias neuron)
	for (int neuron = 0; neuron < network[network_specs[0]-2].size(); neuron++)
	{
		float blame = 0;	
		for (int connection = 0; connection < network[network_specs[0] - 1].size(); connection++)
		{
			blame += error_net[network_specs[0] - 1][connection] * network[network_specs[0] - 1][connection][neuron];
		}	
		error_net[network_specs[0] - 2][neuron] = d_activation(blame);
	}
	

	//comuptes delta for all other layers based on previous layer
	for (int layer = network_specs[0]-3; layer > 0; layer--)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			float blame = 0;
			for (int connection = 1; connection < network[layer + 1].size(); connection++)
			{
				blame += error_net[layer + 1][connection] * network[layer + 1][connection][neuron];
			}
			error_net[layer][neuron] = d_activation(blame);
		}
	}

	float blame;
	//adjusts weights of neurons based on blame
	for (int layer = 1; layer < network.size(); layer++)
	{
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			blame = error_net[layer][neuron];
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				network[layer][neuron][connection] -= training_weight*blame*t_network[layer - 1][connection];
			}
		}
	}
}

void network_group::set_network_size(Network & network, std::vector<int> specs)
{
	network.resize(specs[0]); // 5 layers
	for (int x = 0; x < specs.size()-1; x++)
	{
		//set layer sizes
		network[x].resize(specs[x+1]);
	}
	for (int layer = 1; layer < network.size(); layer++)
	{
		network[layer][0].resize(1);
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			network[layer][neuron].resize(network[layer - 1].size()); //resize neuron to hold connection weights to previous layer
		}
	}
	network[specs[0] - 1][0].resize(network[specs[0] - 2].size()); //the first output neuron shouldn't be a bias neuron
}

void network_group::set_t_network_size(T_Network & network, std::vector<int> specs)
{
	network.resize(specs[0]); // 5 layers
	for (int x = 0; x < specs.size()-1; x++)
	{
		network[x].resize(specs[x+1]);
	}
}

void network_group::set_error_network_size(T_Network & network, std::vector<int> specs)
{
	network.resize(specs[0]); // 5 layers
	for (int x = 0; x < specs.size()-1; x++)
	{
		network[x].resize(specs[x+1]);
	}
}

void network_group::fill_network(Network & network)
{
	for (int layer = 1; layer < network.size(); layer++)
	{
		if (layer != network.size() - 1)
		{
				network[layer][0][0] = random_5_5(rng);
			for (int neuron = 1; neuron < network[layer].size(); neuron++)
			{
				for (int connection = 0; connection < network[layer][neuron].size(); connection++)
				{
					network[layer][neuron][connection] = random_5_5(rng);
				}
			}
		}
		else
		{
			for (int neuron = 0; neuron < network[layer].size(); neuron++)
			{
				for (int connection = 0; connection < network[layer][neuron].size(); connection++)
				{
					network[layer][neuron][connection] = random_5_5(rng);
				}
			}
		}
	}
}

void network_group::modify_vector(Network & net_1, float mutation_chance)
{
	network = net_1;
	for (int layer = 1; layer < network.size() - 1; layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				float rand = random_0_1(rng);
				float rand2 = random_0_1(rng);
				if (rand2 < .0025f * mutation_chance)
					network[layer][neuron][connection] += rand;
				if (rand2 < .005f *mutation_chance && rand2 >= .0025f *mutation_chance)
					network[layer][neuron][connection] -= rand;
				if (rand2 < .0075 *mutation_chance && rand2 >= .005 *mutation_chance)
					network[layer][neuron][connection] *= rand;
				if (rand2 <= .01 *mutation_chance && rand2 >= .0075 *mutation_chance)
					network[layer][neuron][connection] *= -1;
			}
		}
	}
}

int get_random_8_0()
{
	return random_int_0_8(rng);
}
