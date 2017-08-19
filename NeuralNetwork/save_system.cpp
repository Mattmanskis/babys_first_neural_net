#include "stdafx.h"
#include "save_system.h"
#include <fstream>
#include <iomanip>
#include "network.h"
#include <iostream>
#include <cassert>

bool fileExists(std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

std::string save_network(Network & network, int generation, int number)
{
	std::string file_name = std::to_string(generation) + "_" + std::to_string(number) + "_network.txt";
	std::ofstream file(file_name);
	file << std::setprecision(30);
	for (int layer = 1; layer < network.size(); layer++)
	{
		for (int neuron = 0; neuron < network[layer].size(); neuron++)
		{
			for (int connection = 0; connection < network[layer][neuron].size(); connection++)
			{
				file << network[layer][neuron][connection];
				if(connection < network[layer][neuron].size()-1)
					file << 'A';
			}
			if (neuron < network[layer].size() - 1)
				file << 'B';
		}
		if (layer < network.size() - 1)
			file << 'C';
	}
	file << 'A';
	return file_name;
}

void load_network(Network & network, std::string file_name)
{
	std::ifstream file(file_name);
	file >> std::setprecision(30);
	std::string data;
	std::getline(file, data);
	std::vector<int> indicators;
	indicators.resize(data.size() / 8);
	int l_index = 0;
	for (int x = 0; x < data.size(); x++)
	{
		if (data[x] == 'A' || data[x] == 'B' || data[x] == 'C')
		{
			indicators[l_index] = x;
			l_index++;
		}
	}
	int layer = 1;
	int neuron = 0;
	int connection = 0;
	std::string::size_type offset;
	std::string temp;
	network[layer][neuron][connection] = std::stof(data);
	int index_size = l_index -1;
	l_index = 0;
	while (l_index < index_size)
	{
		switch (data[indicators[l_index]])
		{
		case 'A':
			connection++;
			break;
		case 'B':
			connection = 0;
			neuron++;
			break;
		case 'C':
			connection = 0;
			neuron = 0;
			layer++;
			break;
		default:
			assert(false);
		}
		temp = data.substr(indicators[l_index]+1,indicators[l_index]+11);
		network[layer][neuron][connection] = std::stof(temp);
		l_index++;
	}
}

void save_all_and_check(std::vector<network_group> &n , int generation)
{
	for (int x = 0; x < n.size(); x++)
	{
		save_network(n[x].network, generation, x);
	}

	network_group temp_net;
	std::string file_name;

	for (int x = 0; x < n.size(); x++)
	{
		file_name = std::to_string(generation) + "_" + std::to_string(x) + "_network.txt";
		load_network(temp_net.network, file_name);
		for (int layer = 0; layer < temp_net.network.size(); layer++)
		{
			for (int neuron = 0; neuron < temp_net.network[layer].size(); neuron++)
			{
				for (int connection = 0; connection < temp_net.network[layer][neuron].size(); connection++)
				{
					//make sure that all weights are the same in the save file and the network that is being saved.
					assert(temp_net.network[layer][neuron][connection] == n[x].network[layer][neuron][connection]);
				}
			}
		}
	}
}

int load_network_vector(std::vector<network_group> &n)
{
	std::string gen_string;
	std::cout << "enter the generation number you want to load";
	std::getline(std::cin, gen_string);
	int gen_int = stoi(gen_string);
	bool not_ended = true;
	int count = 0;
	while (not_ended)
	{
		not_ended = fileExists(std::to_string(gen_int) + "_" + std::to_string(count) + "_network.txt"); //checks if the file it wants to load exists
		count++;
	}
	n.resize(count*2 - 1); //count = size of the file + 1, multiply by 2 and subtact 1 to get double the network size +1
	for (int x = 0; x < (n.size()-1)/2; x++)
	{
		load_network(n[x].network, std::to_string(gen_int) + "_" + std::to_string(x) + "_network.txt");
	}
	std::cout << "done loading \n";
	return gen_int;
}
