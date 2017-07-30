// NeuralNetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "definitions.h"
#include "save_system.h"
#include "network.h"
#include "tick_tack_toe.h"
#include <iostream>
#include <algorithm>
#include <time.h>

int main()
{
	auto start = time(NULL);
	auto now = time(NULL);
	auto cout_time = time(NULL);
	std::vector<network_group> network_v;
	network_v.resize(51);
	int count = 0;
	while (true)
	{
		for (int x = 0; x < network_v.size(); x++)
		{
			for (int y = 0; y < network_v.size(); y++)
			{
				network_v_network(network_v[x], network_v[y]); //all networks battle to determine fitness
			}
		}
		bool sorted = false;
		while (!sorted) //sort vector by fitness
		{
			int swaps = 0;
			for (int x = 0; x < network_v.size()-2; x++)
			{
				if (network_v[x + 1].fittness > network_v[x].fittness)
				{
					std::swap(network_v[x + 1], network_v[x]);
					swaps++;
				}
			}
			if (swaps == 0)
				sorted = true;
		}
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> random(0, (network_v.size() - 1) / 2); //marry vectors, top 37 reproduce together to remove bottom 38
		for (int x = 0; x < (network_v.size() - 1) / 2; x++)
		{
			network_v[x + (network_v.size() -1) /2].combine_vectors(network_v[x].network, network_v[random(rng)].network);
		}
		network_v[network_v.size() - 1].combine_vectors(network_v[0].network, network_v[random(rng)].network);
		now = time(NULL);
		if (abs(difftime(now,start)) > 7200)
		{
			for (int x = 1; x < network_v.size(); x++)
			{
				save_network(network_v[x - 1].network, count, x);
			}
			start = time(NULL);
		}
		if (abs(difftime(now, cout_time)) > 60)
		{
			std::cout << "generation " << count << '\n';
			for (int x = 1; x < network_v.size(); x++)
			{
				std::cout << "fitness :" << network_v[x].fittness << '\n';
			}
			cout_time = time(NULL);
		}


		for (int x = 0; x < network_v.size(); x++)
		{
			network_v[x].fittness = 0;
		}
		count++;
	}
}

