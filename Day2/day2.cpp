#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::ifstream file("day2.txt");

	// Submarine position, depth, and aim
	int position = 0;
	int depth = 0;
	int aim = 0;

	// Read the data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		// Parse the command
		std::stringstream ss(str);
		std::string tmp;

		// Get the command name
		std::string commandName;
		std::getline(ss, commandName, ' ');

		// Get the numerical command value
		std::getline(ss, tmp, ' ');
		int num = std::stoi(tmp);

		// Handle command cases
		if (commandName == "forward")
		{
			position += num;
			depth += (aim * num);
		}
		else if (commandName == "down")
		{
			aim += num;
		}
		else if (commandName == "up")
		{
			aim -= num;
		}
	}

	file.close();

	printf("Final Pos: %d  Depth: %d  Product: %d\n", position, depth, position * depth);


	return 0;
}