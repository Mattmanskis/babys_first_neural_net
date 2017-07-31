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


std::vector<float> network_group::output(std::vector<float> input)
{
	t_network[0] = input;
	float val = 0;
	for (int layer = 1; layer < network.size(); layer++)
	{
		t_network[layer][0] = network[layer][0][0];
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				val += network[layer][neuron][connection] * t_network[layer-1][connection];
			}
			t_network[layer][neuron] = 1 / (1 + exp(-.4*val));
			val = 0;
		}
	}
	return(t_network[t_network.size() - 1]);
}

void network_group::set_network_size(Network & network)
{
	network.resize(5); // 5 layers
	network[0].resize(9); //#of neurons per layer
	network[1].resize(61);
	network[2].resize(61);
	network[3].resize(61);
	network[4].resize(9);
	int x;
	for (int layer = 1; layer < network.size(); layer++)
	{
		network[layer][0].resize(1);
		for (int neuron = 1; neuron < network[layer].size(); neuron++)
		{
			network[layer][neuron].resize(network[layer - 1].size()); //resize neuron to hold connection weights to previous layer
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

void network_group::set_t_network_size(T_Network & network)
{
	network.resize(5); // 5 layers
	network[0].resize(9); //#of neurons per layer
	network[1].resize(61);
	network[2].resize(61);
	network[3].resize(61);
	network[4].resize(9);
}

void network_group::modify_vector(Network & net_1)
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
				if (rand2 < .0025f)
					network[layer][neuron][connection] += rand;
				if (rand2 < .005f && rand2 >= .0025f)
					network[layer][neuron][connection] -= rand;
				if (rand2 < .0075 && rand2 >= .005)
					network[layer][neuron][connection] *= rand;
				if (rand2 <= .01 && rand2 >= .0075)
					network[layer][neuron][connection] *= -1;
			}
		}
	}
}
