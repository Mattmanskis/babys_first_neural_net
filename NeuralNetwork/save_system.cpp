#include "stdafx.h"
#include "save_system.h"
#include <fstream>
#include <iomanip>
#include "network.h"

std::string save_network(Network & network, int generation, int number)
{
	std::string file_name = std::to_string(generation) + "_" + std::to_string(number) + "_network.txt";
	std::ofstream file(file_name);
	file << std::setprecision(30);
	for (int layer = 1; layer < network.size() - 1; layer++)
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
			_ASSERT(true);
		}
		temp = data.substr(indicators[l_index]+1,indicators[l_index]+11);
		network[layer][neuron][connection] = std::stof(temp);
		l_index++;
	}
}
