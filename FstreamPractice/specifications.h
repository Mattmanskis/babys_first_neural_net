#pragma once
#include <iostream>
#include <vector>
#include <fstream>

struct coordinate
{
	int x;
	int y;
	coordinate()
	{
		x = 0;
		y = 0;
	}
	coordinate(int x_val,int y_val)
	{
		x = x_val;
		y = y_val;
	};
	void get_pair()
	{
		std::cout << "enter x then y seperated by a space \n";
		std::cin >> x;
		std::cin >> y;
	}
};

struct specifications 
{
	coordinate dimensions;
	coordinate max_lengths;
	coordinate min_lengths;
	int max_walls; 
	int abundance; //0-100 how much random chit you want in it

	specifications()
	{
		std::cout << "dimensions \n";
		dimensions.get_pair();
		std::cout << "max lengths \n";
		max_lengths.get_pair();
		std::cout << "min lengths \n";
		min_lengths.get_pair();
		std::cout << "max walls \n";
		std::cin >> max_walls;
		if (max_walls % 2)
			max_walls++;
		if (max_walls < 4)
			max_walls = 4;
	};
};

struct map
{
	std::vector<std::vector<char>> grid;

	void set_grid_size(coordinate dimensions)
	{
		grid.resize(dimensions.y);
		for (int x = 0; x < dimensions.x; x++)
			grid[x].resize(10);
		set_to_default();
	}

	void set_to_default()
	{
		for (int y = 0; y < grid.size(); y++)
		{
			for (int x = 0; x < grid[y].size();x++)
			{
				grid[y][x] = ' ';
			}
		}
	} 

	void make_h_wall(coordinate cord, int length)
	{
		if(length/abs(length) == 1)
			for (int x = cord.x; x < (cord.x + length); x++)
			{
				grid[cord.y][x] = '0';
			}
		else
			for (int x = cord.x; x > (cord.x + length); x--)
			{
				grid[cord.y][x] = 'O';
			}
	}

	void make_v_wall(coordinate cord, int length)
	{
		if (length / abs(length) == 1)
			for (int y = cord.y; y < (cord.y + length); y++)
			{
				grid[y][cord.x] = 'O';
			}
		else
			for (int y = cord.y; y > (cord.y + length); y--)
			{
				grid[y][cord.x] = '0';
			}
	}

	void save_map(std::string file_name)
	{
		std::ofstream txt_file(file_name);
		for (auto vec : grid)
		{
			for (auto c : vec)
			{
				txt_file << c;
			}
			txt_file << '\n';
		}
	}

	void print_map()
	{
		for (auto vec : grid)
		{
			for (auto c : vec)
			{
				std::cout << c;
			}
			std::cout << '\n';
		}
	}
};