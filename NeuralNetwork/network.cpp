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

std::vector<float> network_group::output(std::vector<float> input)
{
	t_network[0] = input;
	float val = 0;
	for (int layer = 1; layer < network.size()-1; layer++)
	{
		t_network[layer][0] = network[layer][0][0];
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				val += network[layer][neuron][connection] * t_network[layer-1][connection];
			}
			t_network[layer][neuron] = 1 / (1 + exp(val));
			val = 0;
		}
	}
	for (int neuron = 0; neuron < network[network.size()-1].size(); neuron++)	//output layer has no bias neurons, and can't be computed in the same loop as others
	{
		for (int connection = 0; connection < network[network.size() - 1][neuron].size(); connection++)
			{
				val += network[network.size() - 1][neuron][connection] * t_network[network.size() - 2][connection];
			}
			t_network[network.size() - 1][neuron] = 1 / (1 + exp(-.4*val));
			val = 0;
	}
	return(t_network[t_network.size() - 1]);
}

void network_group::backprop(std::vector<float> input, std::vector<float> e_output, float training_weight)
{
	//first runs net through normally for output
	t_network[0] = input;
	float val = 0;
	for (int layer = 1; layer < network.size() - 1; layer++)
	{
		t_network[layer][0] = network[layer][0][0];
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				val += network[layer][neuron][connection] * t_network[layer - 1][connection];
			}
			t_network[layer][neuron] = 1 / (1 + exp(val));
			val = 0;
		}
	}
	for (int neuron = 0; neuron < network[network.size() - 1].size(); neuron++)	//output layer has no bias neurons, and can't be computed in the same loop as others
	{
		for (int connection = 0; connection < network[network.size() - 1][neuron].size(); connection++)
		{
			val += network[network.size() - 1][neuron][connection] * t_network[network.size() - 2][connection];
		}
		t_network[network.size() - 1][neuron] = 1 / (1 + exp(-.4*val));
		val = 0;
	}

	//finds error based on expected output and saves it in the last layer of error net
	for (int x = 0; x < e_output.size()-1; x++)
	{
		if (e_output[x] != -1)
		{
			error_net[network_specs[0] - 1][x][0] = e_output[x] - t_network[network_specs[0]-1][x];
		}
		else
			error_net[network_specs[0]-1][x][0] = 0;
	}

	val = 0;

	for(int error = 0; error < network_specs[network_specs.size() - 1]; error ++)
	{
		for (int neuron = 1; neuron < network[network_specs[0] - 2].size(); neuron++) //gets the error of the last hidden layer based on calculated error
		{
			error_net[network_specs[0] - 2][neuron][error] = error_net[network_specs[0] - 1][error][0] * network[network_specs[0]-1][error][neuron];
		}
	}

	for (int error = 0; error < network_specs[network_specs.size() - 1]; error++)
	{
		if (error_net[network_specs[0] - 1][0][error] != 0)
		{
			for (int layer = 1; layer < network_specs[network_specs.size()]-2; layer++)
			{
				for (int neuron = 0; neuron < network[layer].size(); neuron++)
				{
					for (int connection = 0; connection < network[layer][neuron].size(); connection++)
					{
						float x = t_network[layer - 1][connection];
						network[layer][neuron][connection] += training_weight*error_net[layer][neuron][error] * -exp(-x) * (1 / ((1 + exp(-x))*(1 + exp(-x))));
					}
				}
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
}

void network_group::set_t_network_size(T_Network & network, std::vector<int> specs)
{
	network.resize(specs[0]); // 5 layers
	for (int x = 0; x < specs.size()-1; x++)
	{
		network[x].resize(specs[x+1]);
	}
}

void network_group::set_error_network_size(Network & network, std::vector<int> specs)
{
	network.resize(specs[0]); // 5 layers
	for (int x = 0; x < specs.size()-1; x++)
	{
		network[x].resize(specs[x+1]);
	}
	for (int layer = 1; layer < network.size() ; layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			network[layer][neuron].resize(specs[specs.size()-1]); //resize neuron to hold an error value reletive to each output neuron
		}
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
