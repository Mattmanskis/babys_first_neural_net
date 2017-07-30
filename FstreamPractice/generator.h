#pragma once

#include <vector>
#include <string>
#include "specifications.h"
#include "time.h"

int rand_range(int min, int max)
{
	int x = rand() % (max - min + 1) + min;
	return x;
}

void generator(map &m_map, specifications specs)
{
	//resize grid for specs first
	m_map.grid.resize(specs.dimensions.y);
	for (int x = 0; x < m_map.grid.size(); x++)
	{
		m_map.grid[x].resize(specs.dimensions.x);
	}
	m_map.set_to_default();

	coordinate current_loc(specs.dimensions.x/2, specs.dimensions.y/2);
	int length = 0;
	int count = 0;
	while (count < (specs.max_walls - 2))
	{
		count++;
		std::vector<int> options;
		if (current_loc.x != specs.dimensions.x - 1)
		{
			options.push_back(0);
			if (current_loc.x < specs.dimensions.x / 2)
				options.push_back(0);
		}
		if (current_loc.x != 0)
		{
			options.push_back(1);
			if (current_loc.x >= specs.dimensions.x / 2)
				options.push_back(1);
		}
		if (current_loc.y != specs.dimensions.y - 1)
		{
			options.push_back(2);
			if (current_loc.y < specs.dimensions.y / 2)
				options.push_back(2);
		}
		if (current_loc.y != 0)
		{
			options.push_back(3);
			if (current_loc.y >= specs.dimensions.y / 2)
				options.push_back(3);
		}

			switch (options[rand_range(0, options.size()-1)])
			{
				//positive x
			case 0:
				length = rand_range(specs.min_lengths.x, specs.max_lengths.x);
				if (length + current_loc.x > specs.dimensions.x - 1)
					length = specs.dimensions.x - current_loc.x - 1;
				_ASSERT(length != 0);
				m_map.make_h_wall(current_loc, length);
				current_loc.x += length;
				break;
			case 1:
				length = -rand_range(specs.min_lengths.x, specs.max_lengths.x);
				if (length + current_loc.x < 0)
					length = -current_loc.x;
				_ASSERT(length != 0);
				m_map.make_h_wall(current_loc, length);
				current_loc.x += length;
				break;
			case 2:
				length = rand_range(specs.min_lengths.y, specs.max_lengths.y);
				if (length + current_loc.y > specs.dimensions.y - 1)
					length = specs.dimensions.y - current_loc.y -1;
				_ASSERT(length != 0);
				m_map.make_v_wall(current_loc, length);
				current_loc.y += length;
				break;
			case 3:
				length =  -rand_range(specs.min_lengths.y, specs.max_lengths.y);
				if (length + current_loc.y < 0)
					length = -current_loc.y;
				_ASSERT(length != 0);
				m_map.make_v_wall(current_loc, length);
				current_loc.y += length;
				break;
			}
	}
}