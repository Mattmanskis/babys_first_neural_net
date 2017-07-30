#include "stdafx.h"
#include "network.h"
#include <random>
#include <time.h>
#include <math.h>

std::vector<float> network_group::output(std::vector<float> input)
{
	t_network[0] = input;
	float val = 0;
	for (int layer = 1; layer < network.size(); layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
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
	network[1].resize(30);
	network[2].resize(20);
	network[3].resize(10);
	network[4].resize(9);
	for (int layer = 1; layer < network.size() - 1; layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			network[layer][neuron].resize(network[layer - 1].size()); //resize neuron to hold connection weights to previous layer
		}
	}
}

void network_group::fill_network(Network & network)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> random(-.5f, .5f);

	for (int layer = 1; layer < network.size() - 1; layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				network[layer][neuron][connection] = random(rng);
			}
		}
	}
}

void network_group::set_t_network_size(T_Network & network)
{
	network.resize(5); // 5 layers
	network[0].resize(9); //#of neurons per layer
	network[1].resize(30);
	network[2].resize(20);
	network[3].resize(10);
	network[4].resize(9);
}
