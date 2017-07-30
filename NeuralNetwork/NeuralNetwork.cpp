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
	std::vector<network_group> network_v;
	network_v.resize(105);
	for (int x = 0; x < network_v.size(); x++)
	{
		for (int y = 0; y < network_v.size(); y++)
		{
			network_v_network(network_v[x], network_v[y]);
		}
	}
	std::vector<int> fittness_vec;
	fittness_vec.resize(105);
	for (int x = 0; x < network_v.size(); x++)
	{
		fittness_vec[x] = network_v[x].fittness;
	}
	std::sort(fittness_vec.begin(),fittness_vec.end());
	for (int x = 0; x < fittness_vec.size(); x++)
	{
		std::cout << fittness_vec[x] << "\n";
	}
	auto end = time(NULL);
	std::cout << difftime(start, end) << "seconds passed";
	std::cin >> fittness_vec[0];
}

