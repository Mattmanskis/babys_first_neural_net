#pragma once

#include <vector>
#include "definitions.h"
#include <string>
#include "network.h"
bool fileExists(std::string fileName);
std::string save_network(Network &network, int generation, int number);
void load_network(Network &network, std::string file_name);
int load_network_vector(std::vector<network_group> &n);