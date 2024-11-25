#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<fstream>
#include <iomanip>
class universe {
private:
	std::vector<std::vector<bool>> grid;
public:
	void menu();
	void initialize_menu();
	void initialize(bool);
	void file_initialization();
	bool is_valid(std::string&);
	void reset();
	void run();
	void next_generation(std::vector<std::vector<bool>>&);
	int count_neighbors(int,int);
	void display();
};