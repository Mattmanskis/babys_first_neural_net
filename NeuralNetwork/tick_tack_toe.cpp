#include "stdafx.h"
#include "tick_tack_toe.h"
#include <string>

//game vec flipped between each turn for consistancy, and faster learning, 1 is always you -1 is always them 
void flip_vec(std::vector<float>& game_vec)
{
	for (int x = 0; x < 9; x++)
		game_vec[x] *= -1;
}

//takes highest number returned from network, and translates into an int 0-8
int interpret(std::vector<float>& decision_vec)
{
	int decision = 0;
	for (int x = 0; x < 9; x++)
	{
		if (decision_vec[x] > decision_vec[decision])
			decision = x;
	}
	return decision;
}

bool check_win(std::vector<float>& game)
{
	if (1 == game[0] && game[0] == game[1] && game[1] == game[2])
		return true;
	if (1 == game[3] && game[3] == game[4] && game[4] == game[5])
		return true;
	if (1 == game[6] && game[6] == game[7] && game[7] == game[8])
		return true;
	if (1 == game[0] && game[0] == game[3] && game[3] == game[6])
		return true;
	if (1 == game[1] && game[1] == game[4] && game[4] == game[7])
		return true;
	if (1 == game[2] && game[2] == game[5] && game[5] == game[8])
		return true;
	if (1 == game[0] && game[0] == game[4] && game[4] == game[8])
		return true;
	if (1 == game[2] && game[2] == game[4] && game[4] == game[6])
		return true;
	return false;
}

void print_game(std::vector<float>& game)
{
	for (int x = 0; x < 9; x++)
	{
		std::cout << game[x];
		if (x % 3 == 2)
			std::cout << '\n';
	}
	std::cout << '\n';
}

void network_v_network(network_group & net_1, network_group & net_2)
{
	std::vector<float> game;
	game.resize(9);
	bool net_1_streak = true;
	bool net_2_streak = true;
	for (int x = 0; x < 9; x++) //set all squares to empty
	{
		game[x] = 0;
	}
	for (int x = 0; x < 9; x++)
	{
		int decision;
		if (x % 2 == 0) //if even net_1 goes
		{
			decision = interpret(net_1.output(game));
		}
		else //else net_2 goes
		{
			decision = interpret(net_2.output(game));
		}
		if (game[decision] != 0)
		{
			if (x % 2 == 0)
			{
				net_1.fittness--;
				net_1_streak = false;
			}
			else
			{
				net_2.fittness--;
				net_2_streak = false;
			}
		}
		else
		{
			game[decision] = 1;
			if (check_win(game))
			{
				if (x % 2 == 0)
				{
					net_1.fittness += 10;
					net_2.fittness -= 5;
				}
				else
				{
					net_2.fittness += 10;
					net_1.fittness -= 5;
				}
				return;
			}
		}
		flip_vec(game);
	}
	if (net_1_streak)
	{
		net_1.fittness += 2;
	}
	if (net_2_streak)
	{
		net_2.fittness += 2;
	}
}

void human_v_network(network_group & net)
{
	std::string choice;
	std::cout << "For this game each turn you will enter a number 0-8 which will represent your move, \n";
	std::cout << "0 represents upper left, 4 the middle square, and 8 lower right \n";
	std::cout << "Would you like to go first y\n? \n";
	while (choice != "y" && choice != "n")
		std::getline(std::cin, choice);
	int offset = 0;
	if (choice == "n")
		offset = 1;
	std::vector<float> game;
	game.resize(9);
	for (int x = 0; x < 9; x++) //set all squares to empty
	{
		game[x] = 0;
	}
	for (int x = 0; x < 9; x++)
	{
		print_game(game);
		int decision;
		if ((x+offset) % 2 == 0) //if even player goes
		{
			std::getline(std::cin, choice);
			decision = stoi(choice);
		}
		else //else network goes
		{
			decision = interpret(net.output(game));
		}
		if (game[decision] == 0)
		{
			game[decision] = 1;
			if (check_win(game))
			{
				if ((x+offset) % 2 == 0)
				{
					std::cout << "You win! \n";
				}
				else
				{
					std::cout << "Get Rekt! \n";
				}
				return;
			}
		}
		else {
			std::cout << "Error: Bad Move" << std::endl;
		}
		flip_vec(game);
	}
}