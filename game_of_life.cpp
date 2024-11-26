#include"game_of_life.h"
void universe::menu()
{
	std::string choice;
	std::cout << "Welcome to the game of life\n";
	while (true) {
		//this sequence deletes anything that was printed before menu
		std::cout << "\033[2J\033[H\033[3J";
		std::cout << "please choose an option:\n";
		//to make the output more logical as the first thing the user will do is initialize
		if(grid.empty())std::cout << "1.initialize\n";
		else std::cout << "1.layout settings\n";
		std::cout << "2.reset\n";
		std::cout << "3.run\n";
		std::cout << "4.exit\n";
		//display the grid if one was initialized
		display();
		std::cout << "enter your choice: ";
		getline(std::cin >> std::ws, choice);
		//to assure that the input is in valid format for the switch cases
		while(choice.size() != 1) {
			std::cout << "[error]: please enter a valid input: ";
			getline(std::cin >> std::ws, choice);
		}
		switch (choice[0])
		{
		case '1':
			//deleting right before going to initialize to not make the console look cleaner for the user
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
			std::cout << "[error]: Please choose a number between 1-4\n";
			//to let the user read the error before looping the menu again;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			break;
		}

	}
}

void universe::initialize_menu()
{
	while(true){
		// making the user initialize before doing anything else in the layout 
		if (grid.empty()) {
			initialize();
			return;
		}
		//after initializing the user will be able to access everything
		else {
			std::cout << "1.reinitialize grid[it will reset live cells]\n";
			std::cout << "2.set live cells\n";
		}
		std::cout << "3.return to main menu\n";
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
			return;
		default:
			std::cout << "\n[error]:invalid choice\n";
			break;
		}
	}

}
//a function that returns true if the given string is a valid number
bool universe::is_valid(std::string& num)
{
	for (int i = 0; i < num.size(); ++i) {
		if (!isdigit(num[i])) {
			return 0;
		}
	}
	return 1;
}
//reset living cells to dead ones
void universe::reset()
{
	for (int i = 0; i < grid.size(); ++i) {
		std::fill(grid[i].begin(), grid[i].end(), 0);
	}
}
void universe::run()
{
	//to stop the user from running on an empty grid
	if (grid.empty()) {
		std::cout << "[alert]:please initialize grid first\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}
	std::cout << "how many times you want to run: ";
	std::string times;
	getline(std::cin >> std::ws, times);
	//to check if it is a valid number and postive
	if (!is_valid(times)) {
		std::cout << "[error]:please enter a number\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}
	int n = stoi(times);
	std::vector<std::vector<bool>>new_grid(grid.size(), std::vector<bool>(grid[0].size(),0));
	//clearing the console before each output to make the output clear for the user
	std::cout << "\033[2J\033[H\033[3J";
	//printing the initial grid
	std::cout << "generation: " << 0 << '\n';
	display();
	//stopping to let the user quickly see the changes that happened with each generation
	std::this_thread::sleep_for(std::chrono::milliseconds(800));
	for (int i = 0; i < n;++i ) {
		std::cout << "\033[2J\033[H\033[3J";
		next_generation(new_grid);
		std::cout << "generation: " << i+1 << '\n';
		grid = new_grid;
		display();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//applying the rules of the game of life on the new_grid vector using the original grid
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
	//outputting the numbers of coloumns in the grid first
	int i = 1;
	for (i; i <= grid[0].size(); ++i) std::cout << std::setw(3) << std::left << i;
	i *= 3;
	//outputting the divider
	std::cout << '\n' << std::string(i, '-') << '\n';
	for (i = 0; i < grid.size(); ++i) {
		//otuputting the number of the row
		std::cout << std::setw(2) <<std::left << i + 1 << " |";
		for (int j = 0; j < grid[0].size(); ++j) {
			//if the grid is alive we output a '*' in green
			if (grid[i][j]) {
				// the first sequence turns on green colour and the last one turns colouring off
				std::cout << "\033[32m" << std::setw(3) << std::left << '*' << "\033[0m";
			}
			//if the grid is dead we output a '.' in red
			else {
				// the first sequence turns on red colour and the last one turns colouring off
				std::cout << "\033[31m" << std::setw(3) << std::left << '.' << "\033[0m";
			}
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
	std::cout << "4.load a pattern from a file\n";
	std::cout << "5. return to layout settings\n";
	std::cout << "please enter your choice: ";
	std::string choice;
	getline(std::cin >> std::ws, choice);
	while(choice.size() != 1) {
		std::cout << "please enter a valid input: ";
		getline(std::cin >> std::ws, choice);
	}
	int rows = 0, cols = 0;
	while (!rows) {
		if (choice.size() != 1) {
			std::cout << "[error]: please enter a valid single-character choice: ";
			getline(std::cin >> std::ws, choice);
			continue;
		}

		if (choice[0] == '1') {
			rows = 20;
			cols = 20;
		}
		else if (choice[0] == '2') {
			rows = 30;
			cols = 30;
		}
		else if (choice[0] == '3') {
			rows = 20;
			cols = 50;
		}
		else if (choice[0] == '4') {
			file_initialization();
			return;
		}
		else if (choice[0] == '5') {
			return;
		}
		else {
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
	std::cout << "2.enter a percentage of live cells you want[this will reset live cells]\n";
	std::cout << "3.return\n";
	std::cout << "please enter your choice: ";
	std::string choice;
	getline(std::cin >> std::ws, choice);
	while (choice.size() != 1) {
		std::cout << "please enter a valid input: ";
		getline(std::cin >> std::ws, choice);
	}
	//the user will choose the cells manually
	if (choice[0] == '1') {
		while (true) {
			//we first clear console to make the console clean as we will output the grid morethan once
			std::cout << "\033[2J\033[H\033[3J";
			display();
			std::cout << "if a cell is dead it will be alive and vise versa\n";
			std::cout << "enter the cell you wan[enter '0' to return]\n";
			std::cout << "row:";
			std::string row;
			getline(std::cin >> std::ws, row);
			//sending the input we got to check if it is a valid number and +vw
			if (!is_valid(row)) {
				std::cout << "[error]invalid input\n";
				continue;
			}
			int r = stoi(row);
			//the user gets back to initialize menu if he enters '0' in rows
			if (!r) return;
			else if (r > grid.size()) {
				std::cout << "[error]:out of range\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				continue;
			}
			std::cout << "[enter '0' to cancel choice]\n";
			std::cout << "coloumn: ";
			std::string coloumn;
			getline(std::cin >> std::ws, coloumn);
			if (!is_valid(coloumn)) {
				std::cout << "[error]:invalid input\n";
				continue;
			}
			int col = stoi(coloumn);
			//allow the user to cancel his choice by entering '0' in col
			if (!col)
				continue;
			else if(col > grid[0].size()){
				std::cout << "[error]:out of range\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				continue;
			}
			grid[r - 1][col - 1] = !grid[r-1][col-1];
		}
	}
	//the user will give us a percentage of live cells he wants
	else if (choice[0] == '2') {
		//we first reset to make sure we actually do the percentage set by the user
		//as the random number could be a cell that is already on 
		reset();
		double per = 0;
		int n = 0;
		while (!per) {
			std::cout << "Please enter the percentage you want: ";
			std::string num;
			getline(std::cin >> std::ws, num);
			if (is_valid(num) && stoi(num) <= 100) per = stoi(num);
			else "[error]:invalid input\n";
		}
		//initializes the random number generator with a new seed
		// ensuring that rand() produces a different sequence each time the program runs.
		//using "time(NULL)" so that the sequence differs every second
		srand(time(NULL));
		n = grid.size() * grid[0].size();
		int limit = n * (per / 100);
		//using a set to avoid using the same random number morethan once
		std::set<int> nums;
		while (nums.size() < limit) {
			int num = (rand() % n);
			nums.insert(num);
		}
		int cols = grid[0].size();
		for (auto& num : nums) {
			int row = num / cols;
			int col = num % cols;
			grid[row][col] = 1;
		}
	}
	else if (choice[0] == '3') return;
	else std::cout << "[error]:invalid choice\n";
}

void universe::file_initialization()
{
	std::fstream file;
	while (true) {
		std::cout << "enter file name[enter \"return\" to return to layout settings]: ";
		std::string file_name;
		getline(std::cin>>std::ws, file_name);
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
	//as the pattern should be the end of the file and nothing else comes after it
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

