#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	// Data vector
	std::vector<int> data;

	// Extract data from data file
	std::ifstream file("day1.txt");

	// Read data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		data.push_back(std::stoi(str));
	}
	file.close();

	// Check number of increasing elements
	int numIncreasing = 0;
	for (int i = 0; i < data.size()-3; i++)
	{
		int sum1 = data[i] + data[i + 1] + data[i + 2];
		int sum2 = data[i + 1] + data[i + 2] + data[i + 3];

		if (sum2 > sum1)
			numIncreasing++;
	}

	printf("Num Increasing: %d\n", numIncreasing);



	return 0;
}