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
	std::string option;
	bool valid = false;
	while (!valid)
	{
		std::cout << "generate or play?\n";
		std::getline(std::cin, option);
		if (option == "play"||option == "generate")
			valid = true;
	}
	std::cout << "\n";
	if (option == "generate")
	{
		std::string save_time;
		std::cout << "enter how often you would like to save networks, in seconds";
		std::getline(std::cin, save_time);
		auto save_timer = time(NULL);
		auto cout_timer = time(NULL);
		auto now = time(NULL);
		std::vector<network_group> network_v; //creates a vector of network groups
		network_v.resize(101);
		int count = 0;
		while (true)
		{
			for (int x = 0; x < network_v.size(); x++)
			{
				for (int y = 0; y < network_v.size(); y++)
				{
					network_v_network(network_v[x], network_v[y]); //all networks battle against eachother to determine fitness
				}
			}
			bool sorted = false;
			while (!sorted) //sort vector of networks by fitness
			{
				int swaps = 0;
				for (int x = 0; x < network_v.size() - 2; x++)
				{
					if (network_v[x + 1].fittness > network_v[x].fittness)
					{
						std::swap(network_v[x + 1], network_v[x]);
						swaps++;
					}
				}
				if (network_v[network_v.size() - 1].fittness > network_v[network_v.size() - 2].fittness)
				{
					std::swap(network_v[network_v.size() - 1], network_v[network_v.size() - 2]);
					swaps++;
				}
				if (swaps == 0)
					sorted = true;
			}
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> random(0, (network_v.size() - 1) / 2); //top 50% of vectors are coppied into bottom 50% of vectors, then slightly changed
			for (int x = 0; x < (network_v.size() - 1) / 2; x++)
			{
				network_v[x + (network_v.size() - 1) / 2].modify_vector(network_v[x].network);
			}
			network_v[network_v.size() - 1].modify_vector(network_v[0].network);
			now = time(NULL);
			if (abs(difftime(now, save_timer)) > std::stoi(save_time)) //saves all networks every save_time seconds
			{
				for (int x = 0; x < network_v.size()/2; x++)
				{
					save_network(network_v[x].network, count, x);
				}
				save_timer = time(NULL);
			}
			if (abs(difftime(now, cout_timer)) > 60) //outputs fitness of last generation every 60 seconds
			{
				std::cout << "generation " << count << '\n';
				for (int x = 1; x < network_v.size(); x++)
				{
					std::cout << "fitness :" << network_v[x].fittness << '\n';
				}
				std::cout << "seconds until save:" << std::stoi(save_time) - abs(difftime(now, save_timer)) << '\n';
				cout_timer = time(NULL);
			}

			for (int x = 0; x < network_v.size(); x++) //resets fitnesses for next generation
			{
				network_v[x].fittness = 0;
			}
			count++;
		}
	}
	else //user decides to take on one of the ai
	{
		std::string file_name;
		network_group n; 
		while (!fileExists(file_name))
		{
			std::cout << "please enter valid file name with no spaces, including .txt \n";
			std::getline(std::cin, file_name);
		}
		load_network(n.network, file_name);
		human_v_network(n);
	}
}

