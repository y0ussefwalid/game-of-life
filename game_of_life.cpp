#include"game_of_life.h"
void universe::menu()
{
	std::string choice;
	std::cout << "Welcome to the game of life\n";
	while (true) {
		std::cout << "\033[2J\033[H\033[3J";
		std::cout << "please choose an option:\n";
		if(grid.empty())std::cout << "1.initialize\n";
		else std::cout << "1.layout settings\n";
		std::cout << "2.reset\n";
		std::cout << "3.run\n";
		std::cout << "4.exit\n";
		display();
		std::cout << "enter your choice: ";
		getline(std::cin >> std::ws, choice);
		while(choice.size() != 1) {
			//std::cout << "\033[1A\033[0G";
			std::cout << "[error]: please enter a valid input: ";
			getline(std::cin >> std::ws, choice);
		}
		switch (choice[0])
		{
		case '1':
			std::cout << "\033[2J\033[H\033[3J";
			initialize_menu();
			break;
		case '2':
			reset();
			break;
		case '3':
			run();
			break;
		case '4':
			return;
		default:
			std::cout << "invalid choice\n";
			break;
		}

	}
}

void universe::initialize_menu()
{
	while(true){
		if (grid.empty()) {
			initialize();
			continue;
		}
		else {
			std::cout << "1.reinitialize grid[it will reset live cells]\n";
			std::cout << "2.set live cells\n";
		}
		std::cout << "3.load a pattern from a file\n";
		std::cout << "4.return\n";
		std::cout << "enter your choice: ";
		std::string choice;
		getline(std::cin >> std::ws, choice);
		while (choice.size() != 1) {
			std::cout << "[error]:please enter a valid input: ";
			getline(std::cin >> std::ws, choice);
		}
		switch (choice[0])
		{
		case '1':
			initialize();
			break;
		case '2':
			set_cells();
			break;
		case '3':
			file_initialization();
			break;
		case '4':
			return;
		default:
			std::cout << "[error]:invalid choice\n";
			break;
		}
	}

}

bool universe::is_valid(std::string& num)
{
	for (int i = 0; i < num.size(); ++i) {
		if (!isdigit(num[i])) {
			return 0;
		}
	}
	return 1;
}
void universe::reset()
{
	for (int i = 0; i < grid.size(); ++i) {
		std::fill(grid[i].begin(), grid[i].end(), 0);
	}
}
void universe::run()
{
	if (grid.empty()) {
		std::cout << "[alert]:please initialize grid first\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
		return;
	}
	std::cout << "how many times you want to run: ";
	std::string times;
	getline(std::cin >> std::ws, times);
	if (!is_valid(times)) {
		std::cout << "please enter a number\n";
		return;
	}
	int n = stoi(times);
	int start = n;
	std::vector<std::vector<bool>>new_grid(grid.size(), std::vector<bool>(grid[0].size(),0));
	while (n--) {
		std::cout << "\033[2J\033[H\033[3J";
		display();
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
		next_generation(new_grid);
		grid = new_grid;
	}
	//grid = new_grid;
}
void universe::next_generation(std::vector<std::vector<bool>>& new_grid) {
	for (int i = 0; i < grid.size(); ++i) {
		for (int j = 0; j < grid[i].size(); ++j) {
			int neighbors = count_neighbors(i, j);
			if (grid[i][j]) {
				if (neighbors < 2) {
					new_grid[i][j] = false;
				}
				else if (neighbors == 2 || neighbors == 3) {
					new_grid[i][j] = true;
				}
				else if (neighbors > 3) {
					new_grid[i][j] = false;
				}
			}
			else {
				if (neighbors == 3) {
					new_grid[i][j] = true;
				}
				else {
					new_grid[i][j] = false;
				}
			}
		}
	}
}

int universe::count_neighbors(int row, int col) {
	int count = 0;

	// Define the directions for the 8 possible neighbors
	std::vector<std::pair<int, int>> directions = {
		{-1, -1}, {-1, 0}, {-1, 1},{0, -1},{0, 1},{1, -1}, {1, 0}, {1, 1}};

	for (const auto& dir : directions) {
		int newRow = row + dir.first;
		int newCol = col + dir.second;

		// Check if the neighbor is within bounds
		if (newRow >= 0 && newRow < grid.size() && newCol >= 0 && newCol < grid[0].size()) {
			if (grid[newRow][newCol]) { // If the neighbor is alive
				count++;
			}
		}
	}

	return count;
}

void universe::display()
{
	if (grid.empty()) {
		std::cout << "[initialize grid]\n";
		return;
	}
	std::cout << "    ";
	int i = 1;
	for (i; i <= grid[0].size(); ++i) std::cout << std::setw(3) << std::left << i;
	i *= 3;
	std::cout << '\n' << std::string(i, '-') << '\n';
	for (i = 0; i < grid.size(); ++i) {
		std::cout << std::setw(2) <<std::left << i + 1 << " |";
		for (int j = 0; j < grid[0].size(); ++j) {
			std::cout << std::setw(3) << std::left << grid[i][j];
		}
		std::cout << '\n';
	}
}


void universe::initialize()
{
	std::cout << "Please choose one of the following size options: \n";
	std::cout << "1. 20x20\n";
	std::cout << "2. 30x30\n";
	std::cout << "3. 20x50\n";
	std::cout << "4. return\n";
	std::cout << "please enter your choice: ";
	std::string choice;
	getline(std::cin >> std::ws, choice);
	while(choice.size() != 1) {
		std::cout << "please enter a valid input: ";
		getline(std::cin >> std::ws, choice);
		//std::cout << "\033[2k";
	}
	int rows = 0, cols = 0;
	while(!rows){
		switch (choice[0]) {
		case '1':
			rows = 20;
			cols = 20;
			break;
		case '2':
			rows = 30;
			cols = 30;
			break;
		case '3':
			rows = 20;
			cols = 50;
			break;
		case '4':
			return;
		default:
			std::cout << "[error]: please enter a valid choice: ";
			getline(std::cin >> std::ws, choice);
		}
	}
	grid.resize(rows);
	for (int i = 0; i < grid.size(); ++i) {
		grid[i].resize(cols);
		std::fill(grid[i].begin(), grid[i].end(), 0);
	}
}

void universe::set_cells()
{
	std::cout << "1.manually set live cells\n";
	std::cout << "2.enter a percentage of live cells you want\n";
	std::cout << "3.return\n";
	std::cout << "please enter your choice: ";
	std::string choice;
	getline(std::cin >> std::ws, choice);
	while (choice.size() != 1) {
		std::cout << "please enter a valid input: ";
		getline(std::cin >> std::ws, choice);
	}
	if (choice[0] == '1') {
		while (true) {
			std::cout << "\033[2J\033[H\033[3J";
			display();
			std::cout << "if a cell is dead it will be alive and vise versa\n";
			std::cout << "enter the cell you wan[enter '0' to return]\n";
			std::cout << "row:";
			std::string row;
			getline(std::cin >> std::ws, row);
			if (!is_valid(row)) {
				std::cout << "[error]invalid input\n";
				continue;
			}
			int r = stoi(row);
			if (r > grid.size()) {
				std::cout << "[error]:out of range\n";
				continue;
			}
			else if (!r) return;
			std::cout << "coloumn: ";
			std::string coloumn;
			getline(std::cin >> std::ws, coloumn);
			if (!is_valid(coloumn)) {
				std::cout << "[error]:invalid input\n";
				continue;
			}
			int col = stoi(coloumn);
			if (col > grid[0].size()|| !col) {
				std::cout << "[error]:out of range\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				continue;
			}
			grid[r - 1][col - 1] = !grid[r-1][col-1];
		}
	}
	else if (choice[0] == '2') {
		return;
	}
	else if (choice[0] == '3') return;
	else std::cout << "[error]:invalid choice\n";
}

void universe::file_initialization()
{
	std::fstream file;
	while (true) {
		std::cout << "enter file name[enter \"return\" to return to menu]\n: ";
		std::string file_name;
		getline(std::cin, file_name);
		if (file_name == "return") return;
		file.open(file_name);
		if (file.fail()) {
			std::cout << "couldnt find file\n";
			continue;
		}
		break;
	}
	std::string width;
	file >> width;
	if (!is_valid(width)) {
		std::cout << "invalid format\n";
		return;
	}
	std::string height;
	file >> height;
	if (!is_valid(height)){
		std::cout << "invalid format\n";
		return;
	}
	int w = stoi(width);
	int h = stoi(height);
	grid.resize(h);
	for (int i = 0; i < grid.size(); ++i) {
		grid[i].resize(w);
		std::fill(grid[i].begin(), grid[i].end(), 0);
	}
	std::string pattern;
	file >> pattern;
	if (!file.eof()) {
		std::cout << "invalid format\n";
		return;
	}
	for (int i = 0; i < pattern.size(); ++i) {
		int row = i / w;
		int col = i % w;
		grid[row][col] = (pattern[i] == '1' ? 1 : 0);
	}
	file.close();
}

