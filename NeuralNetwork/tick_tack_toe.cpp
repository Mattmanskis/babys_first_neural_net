#include "stdafx.h"
#include "tick_tack_toe.h"
#include <string>
const std::vector<std::vector<float>> lines{ { 0,1,2 }, { 3,4,5 } ,{ 6,7,8 },{ 0,3,6 },{ 1,4,7 },{ 2,5,8 },{ 0,4,8 },{ 2,4,6 } };
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

int search_game(std::vector<float> game, int check_this, int number_to_skip) //returns first index if found, returns -1 if none
{
	int found = 0;
	for (int x = 0; x < 9; x++)
	{
		if (game[x] == check_this)
		{
			if (found == number_to_skip)
				return x;
			found++;
		}
	}
	return -1;
}

int search_lines(int value, int number_to_skip) //finds the lines that you can win on that a given piece is in, -1 if none
{
	int found = 0;
	for (int x = 0; x < lines.size(); x++)
	{
		for (int y = 0; y < lines[x].size(); y++)
		{
			if (lines[x][y] == value)
			{
				if (found == number_to_skip)
					return x;
				found++;
			}
		}
	}
	return -1;
}

int ai_decision(std::vector<float> game)
{
	int val; 
	if (search_game(game, -1,0) == search_game(game, 1,0)) //if there are no plays on the board
		return 0;
	else
	{
		int piece_count=0;
		int line_count=0;
		while (search_game(game, 1, piece_count) != -1) //goes through game looking for pieces that are yours
		{
			while (search_lines(search_game(game, 1, piece_count), line_count)) //for chosen piece looks for a line of three conneceted to that piece
			{
				bool good = true;
				val = search_lines(search_game(game, 1, piece_count), line_count); //for chosen line looks through peices that are blocking it
				for (int x = 0; x < lines[val].size(); x++)
				{
					if (game[lines[val][x]] == -1)
						good = false;
				}
				if (good)
				{
					for (int x = 0; x < lines[val].size(); x++)
					{
						if (game[lines[val][x]] == 0)
							return lines[val][x];
					}
				}
				line_count++;
			}
			piece_count++;
		}
	}
	return search_game(game, 0, 0);
}

void ai_v_network(network_group & net_1,bool goes_first)
{
	std::vector<float> game;
	game.resize(9);
	int offset = 0;
	if (!goes_first)
		offset++;
	for (int x = 0; x < 9; x++) //set all squares to empty
	{
		game[x] = 0;
	}
	for (int x = 0; x < 9; x++)
	{
		int decision;
		if ((x + offset) % 2 == 0) //if x+offset is even net_1 goes
		{
			decision = interpret(net_1.output(game));
		}
		else //ai net_2 goes
		{
			decision = ai_decision(game);
		}
		if (game[decision] != 0)
		{
			if ((x + offset) % 2 == 0)
			{
				net_1.fittness -= 5;
			}
			else
				std::cout << "ai bad move \n";
		}
		else
		{
			if ((x + offset) % 2 == 0)
			{
				net_1.fittness += 5;
			}
			game[decision] = 1;
			if (check_win(game))
			{
				if ((x + offset) % 2 == 0)
				{
					net_1.fittness += 100;
				}
				else
				{
					net_1.fittness -= 25;
				}
				return;
			}
		}
		flip_vec(game);
	}
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