#pragma once
#include "definitions.h"
#include "network.h"
#include <iostream>


//game vec flipped between each turn for consistancy, and faster learning, 1 is always you -1 is always them 
void flip_vec(std::vector<float> &game_vec);

//takes highest number returned from network, and translates into an int 0-8
int interpret(const std::vector<float> &decision_vec);
 
bool check_win(std::vector<float> &game);

void print_game(std::vector<float> &game);

void network_v_network(network_group &net_1, network_group &net_2);

void ai_v_ai(network_group& net);

void ai_v_network(network_group & net_1, bool goes_first);

void human_v_network(network_group &net);


struct game_state
{
	game_state* previous;
	std::vector<game_state> next;

	float odd_win = 0;
	float even_win = 0;
	float tie = 0;

	float rating = -2; //number between -1 and 1, -1 representing best move for even, 1 being best for odd

	bool odd; //weather it is an even or odd move

	bool starting = false;

	std::vector<float> game;

	game_state()
	{
		game.resize(9);
	}

	void fill(int index, game_state* calling = NULL)
	{
		if (calling == NULL) //null means it's the first state, or empty board
		{
			game = { 0,0,0,0,0,0,0,0,0 };
			odd = false;
			starting = true;
			next.resize(9);
			for (int x = 0; x < 9; x++)
			{
				next[x].fill(x,this);
			}
		}
		else
		{
			previous = calling;
			game = calling->game;
			odd = !calling->odd;
			if (odd)
				game[index] = 1;
			else
				game[index] = -1;
		}
		if (!check_win(game)) //if it's not a winning state
		{
			int count = 0;
			bool full = true;
			while (full)
			{
				if (game[count] == 0)
					full = false;
				else if (count == 8)
				{
					add_tie(0);
					return;
				}
				count++;
			} 
			next.resize(9);
			count = 0;
			for (int x = 0; x < 9; x++)
			{
				if (game[x] == 0)
				{
					next[x].fill(x, this);
				}
			}
		}
		else // if you are a winning state make no more states and hand back the win to chain of previous
		{
			add_win(odd,0);
		}
	}

	void add_tie(int depth)
	{
		tie += pow(.1,depth);
		if (!starting)
			previous->add_tie(depth +1);
		calc_rating();
	}

	void add_win(bool is_odd, int depth)
	{
		if (is_odd)
		{
			odd_win += pow(.1,depth);
			if(!starting)
				previous->add_win(is_odd,depth+1);
		}
		else
		{
			even_win += pow(.1, depth);
			if(!starting)
				previous->add_win(is_odd,depth+1);
		}
		calc_rating();
	}

	void calc_rating()
	{
		rating = (odd_win - even_win) / (even_win + odd_win + tie);
	}
};

void play_state(game_state* start);

void train_with_state(network_group &net, game_state* start);