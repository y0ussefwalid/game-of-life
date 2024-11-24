#pragma once
#include <iostream>
#include <vector>
class univere {
private:
	std::vector<std::vector<int>> grid;
public:
	void menu();
	void initialize();
	void run(int);
	void next_generation();
	int count_neighbors(int);
	void display();
};