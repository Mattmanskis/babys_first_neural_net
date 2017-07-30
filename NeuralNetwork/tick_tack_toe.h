#pragma once
#include "definitions.h"
#include "network.h"
#include <iostream>

//game vec flipped between each turn for consistancy, and faster learning, 1 is always you -1 is always them 
void flip_vec(std::vector<float> &game_vec);

//takes highest number returned from network, and translates into an int 0-8
int interpret(std::vector<float> &decision_vec);
 
bool check_win(std::vector<float> &game);

void print_game(std::vector<float> &game);

void network_v_network(network_group &net_1, network_group &net_2);

void human_v_network(network_group &net);