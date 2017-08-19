#pragma once
#include "definitions.h"
#include <vector>
#include <random>


struct network_group
{
	Network network;

	T_Network t_network;

	Network error_net;

	float fitness = 0;

	std::vector<int> network_specs = { 5,9,61,61,61,9 };
	network_group()
	{
		set_network_size(network,network_specs);
		fill_network(network);
		set_t_network_size(t_network,network_specs);
		set_error_network_size(error_net, network_specs);
	};

	std::vector<float> output(std::vector<float> input);

	void network_group::backprop(std::vector<float> input, std::vector<float> output, float training_rate);

	//vector should have number of layers as first number (including input and output layers) and number of neurons per layer for all numbers after that
	void set_network_size(Network & network, std::vector<int> specs);

	//vector should have number of layers as first number (including input and output layers) and number of neurons per layer for all numbers after that
	void set_t_network_size(T_Network & network, std::vector<int> specs);

	//vector should have number of layers as first number (including input and output layers) and number of neurons per layer for all numbers after that
	void set_error_network_size(Network & network, std::vector<int> specs);

	void fill_network(Network &network);

	void modify_vector(Network & net_1, float mutation_chance);

};

int get_random_8_0();

