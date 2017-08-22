#pragma once
#include <vector>
#include "network.h"
#include <iostream>
#include <string>
/*
test to see if my back propagation works, I want it to train it to recognise the following
100
010 = 0
001

001
010 = 1
100

111
000 = 2
000

000
111
000 = 3

000
000 = 4
111

111
111 = 5
111

000
000 = 6
000
*/

std::vector<float> test_0 = { 1,0,0,0,1,0,0,0,1 };
std::vector<float> test_1 = { 0,0,1,0,1,0,1,0,0 };
std::vector<float> test_2 = { 1,1,1,0,0,0,0,0,0 };
std::vector<float> test_3 = { 0,0,0,1,1,1,0,0,0 };
std::vector<float> test_4 = { 0,0,0,0,0,0,1,1,1 };
std::vector<float> test_5 = { 1,1,1,1,1,1,1,1,1 };
std::vector<float> test_6 = { 0,0,0,0,0,0,0,0,0 };
std::vector<float> output_0 = { 1,0,0,0,0,0,0,0,0 };
std::vector<float> output_1 = { 0,1,0,0,0,0,0,0,0 };
std::vector<float> output_2 = { 0,0,1,0,0,0,0,0,0 };
std::vector<float> output_3 = { 0,0,0,1,0,0,0,0,0 };
std::vector<float> output_4 = { 0,0,0,0,1,0,0,0,0 };
std::vector<float> output_5 = { 0,0,0,0,0,1,0,0,0 };
std::vector<float> output_6 = { 0,0,0,0,0,0,1,0,0 };
std::vector<float> unknown_1 = { 1,0,1,0,1,0,1,0,1 };
std::vector<float> unknown_2 = { 1,1,1,1,0,1,1,1,1 };
std::vector<float> unknown_3 = { 1,0,1,0,0,0,1,0,1 };

void print_output(std::vector<float> output);

void test()
{
	network_group test_net;
	//trains net 100 times, with data and output
	for (int x = 0; x < 100; x++)
	{
		test_net.backprop(test_0, output_0, x / 100);
		test_net.backprop(test_1, output_1, x / 100);
		test_net.backprop(test_2, output_2, x / 100);
		test_net.backprop(test_3, output_3, x / 100);
		test_net.backprop(test_4, output_4, x / 100);
		test_net.backprop(test_5, output_5, x / 100);
		test_net.backprop(test_6, output_6, x / 100);
	}
	std::cout << "test 0 \n \n";
	print_output(test_net.output(test_0));
	std::cout << "test 1 \n \n";
	print_output(test_net.output(test_1));
	std::cout << "test 2 \n \n";
	print_output(test_net.output(test_2));
	std::cout << "test 3 \n \n";
	print_output(test_net.output(test_3));
	std::cout << "test 4 \n \n";
	print_output(test_net.output(test_4));
	std::cout << "test 5 \n \n";
	print_output(test_net.output(test_5));
	std::cout << "test 6 \n \n";
	print_output(test_net.output(test_6));
	std::cout << "unknown 1 \n \n";
	print_output(test_net.output(unknown_1));
	std::cout << "unknown 2 \n \n";
	print_output(test_net.output(unknown_2));
	std::cout << "unknown 3 \n \n";
	print_output(test_net.output(unknown_3));
}

void print_output(std::vector<float> output)
{
	for (int x = 0; x < output.size(); x++)
	{
		std::cout <<x << " : " << output[x] <<'\n';
	}
	std::cout << '\n';
}

