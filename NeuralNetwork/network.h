#pragma once
#include "definitions.h"
#include <vector>
#include <random>


struct network_group
{
	Network network;
	T_Network t_network;
	int fittness = 0;
	network_group()
	{
		set_network_size(network);
		fill_network(network);
		set_t_network_size(t_network);
	};

	std::vector<float> output(std::vector<float> input);

	void set_network_size(Network &network);

	void fill_network(Network &network);

	void set_t_network_size(T_Network &network);

	void combine_vectors(Network &net_1, Network &net_2);
};