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
#include "back_prop_test.h"

int main()
{
	//test();
	std::string option;
	bool valid = false;
	while (!valid)
	{
		std::cout << "generate or play?\n";
		std::getline(std::cin, option);
		if (option == "play"||option == "generate")
			valid = true;
	}
	valid = false;
	std::cout << "\n";
	if (option == "generate")
	{
		std::string save_time;
		std::string output_time;
		int start_time_o = 0;
		float mutation_chance;
		std::vector<network_group> network_v; //creates a vector of network groups
		int gen_count = 0;

		std::cout << "enter how often you would like to save networks, in seconds \n";
		std::getline(std::cin, save_time);

		std::cout << " \n enter how often you would like to get information from console, in seconds \n";
		std::getline(std::cin, output_time);

		valid = false;

		bool use_ai = false;

		while (!valid)
		{
			std::cout << "\n use ai? y/n \n";
			std::getline(std::cin, option);
			if (option == "y" || option == "n")
				valid = true;
		}

		if (option == "y")
		{
			use_ai = true;
		}

		valid = false;

		bool use_auto_save = false;

		while (!valid)
		{
			std::cout << "\n auto save new highest network? y/n \n";
			std::getline(std::cin, option);
			if (option == "y" || option == "n")
				valid = true;
		}

		if (option == "y")
		{
			use_auto_save = true;
		}

		valid = false;

		while (!valid)
		{
			std::cout << "\n start from existing generation of networks? y/n \n";
			std::getline(std::cin, option);
			if (option == "y" || option == "n")
				valid = true;
		}
		
		if (option == "y")
		{

			gen_count = load_network_vector(network_v);
			std::cout << " \n enter start time in seconds \n";
			std::string temp;
			std::getline(std::cin, temp);
			start_time_o = std::stoi(temp);
		}
		else
		{
			network_v.resize(2);
		}



		auto start_time = time(NULL);
		auto save_timer = time(NULL);
		auto cout_timer = time(NULL);
		auto now = time(NULL);
		float max_fitness = -1000000;
		int max_fitness_gen = 0;

		while (true)
		{
			if (use_ai)
			{
				for (int x = 0; x < network_v.size(); x++)
				{
					ai_v_ai(network_v[x]);
					ai_v_network(network_v[x], true); //plays ai with both first and second move
					ai_v_network(network_v[x], false);
					// divide fitness by 2 games to get avarage fitness
					network_v[x].fitness *= .5;
				}
			}
			else
			{
				for (int x = 0; x < network_v.size(); x++)
				{
					for (int y = 0; y < network_v.size(); y++)
					{
						network_v_network(network_v[x], network_v[y]);
						network_v_network(network_v[y], network_v[x]);
					}
				}
				for (int x = 0; x < network_v.size(); x++)
				{
					// divide fitness by number of networks *2 to get avarage fitness 
					network_v[x].fitness = network_v[x].fitness / (network_v.size() * 2);
				}
			}

			bool sorted = false;

			while (!sorted) //sort vector of networks by fitness
			{
				int swaps = 0;
				for (int x = 0; x < network_v.size() - 2; x++)
				{
					if (network_v[x + 1].fitness > network_v[x].fitness)
					{
						std::swap(network_v[x + 1], network_v[x]);
						swaps++;
					}
				}
				if (network_v[network_v.size() - 1].fitness > network_v[network_v.size() - 2].fitness)
				{
					std::swap(network_v[network_v.size() - 1], network_v[network_v.size() - 2]);
					swaps++;
				}
				if (swaps == 0)
					sorted = true;
			}

			if (network_v[0].fitness > max_fitness)
			{
				max_fitness = network_v[0].fitness;
				max_fitness_gen = gen_count;
				if (use_auto_save)
				{
					if (gen_count > 30)
					{
						for (int x = 0; x < network_v.size() / 2; x++)
						{
							save_network(network_v[x].network, max_fitness_gen, x);
						}
					}
				}
			}
			/*
			// mutation chance  = f(x)/f(x)^2 for f(x) = time runing / (max_possible_fitness - current_highest_fitness) + 2
			//mutation_chance = 100 * (abs(difftime(now, start_time) + start_time_o) / (501 - network_v[0].fitness) + 2) / ((abs(difftime(now, start_time) + start_time_o) / (501 - network_v[0].fitness) + 2) *  (abs(difftime(now, start_time) + start_time_o) / (501 - network_v[0].fitness) + 2));
			//top 50% of vectors are coppied into bottom 50% of vectors, then slightly changed
			for (int x = 30; x < 45 ; x++)
			{
				network_v[x].modify_vector(network_v[0].network, mutation_chance);
			}
			for (int x = 45; x < 60; x++)
			{
				network_v[x].modify_vector(network_v[1].network, mutation_chance);
			}
			for (int x = 2; x < 30; x++)
			{
				network_v[x + 58].modify_vector(network_v[x].network, mutation_chance);
			}
			network_v[88].modify_vector(network_v[2].network, mutation_chance);
			network_v[89].modify_vector(network_v[3].network, mutation_chance);
			*/
			now = time(NULL);
			if (abs(difftime(now, save_timer)) > std::stoi(save_time)) //saves all networks every save_time seconds
			{
				for (int x = 0; x < network_v.size()/2; x++)
				{
					save_network(network_v[x].network, gen_count, x);
				}
				save_timer = time(NULL);
			}
			if (abs(difftime(now, cout_timer)) > std::stoi(output_time)) //outputs fitness of last generation every 60 seconds
			{
				std::cout << "generation " << gen_count << '\n';
				for (int x = 0; x < network_v.size(); x++)
				{
					std::cout << "fitness :" << network_v[x].fitness << '\n';
				}
				std::cout << "seconds until save:" << std::stoi(save_time) - abs(difftime(now, save_timer)) << '\n';
				std::cout << "max fitness is " << max_fitness << " at gen " << max_fitness_gen << '\n';
				cout_timer = time(NULL);
			}

			for (int x = 0; x < network_v.size(); x++) //resets fitnesses for next generation
			{
				network_v[x].fitness = 0;
			}
			gen_count++;
		}
	}
	else if(option == "play")//user decides to take on one of the ai
	{
		std::string file_name;
		network_group n; 
		while (!fileExists(file_name))
		{
			std::cout << "please enter valid file name with no spaces, including .txt \n";
			std::getline(std::cin, file_name);
		}
		load_network(n.network, file_name);
		while(true)
			human_v_network(n);
	}
	system("PAUSE");
}

