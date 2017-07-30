// FstreamPractice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "specifications.h"
#include "generator.h"

bool fileExists(std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

std::string get_name()
{
	std::string file_name = "map.txt";
	int count = 1;
	while (true)
	{
		if (fileExists(file_name))
		{
			file_name = "map";
			file_name = file_name + std::to_string(count) + ".txt";
			count++;
		}
		else
		{
			return file_name;
		}
	}

}

int main()
{
	specifications specs;
	std::string file_name = "map.txt";

	map m_map;

	srand(time(NULL));

	std::cout << "enter number of maps to generate \n";
	int number_of_maps = 0;
	std::cin >> number_of_maps;

	for (int x = 0; x < number_of_maps; x++)
	{
		//m_map.make_h_wall(coordinate(1, 1), 8);
		//m_map.make_h_wall(coordinate(1, 8), 8);
		//m_map.make_v_wall(coordinate(1, 1), 8);
		//m_map.make_v_wall(coordinate(8, 1), 8);
		generator(m_map, specs);
		file_name = get_name();
		std::cout << "map " << x + 1 << "\n";
		m_map.print_map();
		m_map.save_map(file_name);
	}
	std::cin >> number_of_maps;
}

