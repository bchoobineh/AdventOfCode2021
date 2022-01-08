#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#define NUM_DAYS 256

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::ifstream file("day6.txt");

	// Fish data buffer
	std::vector<int> fishBuffer;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);
		std::string tmp;
		while (std::getline(ss, tmp, ','))
		{
			fishBuffer.push_back(std::stoi(tmp));
		}
	}
	file.close();
	

	// Zero out hashmap of fish
	std::unordered_map<int, long long> fishMap;
	for (int i = 0; i <= 8; i++)
		fishMap[i] = 0;

	// Insert fish buffer into fish map
	for (int i = 0; i < fishBuffer.size(); i++)
		fishMap[fishBuffer.at(i)]++;


	// Simulate each day
	for (int i = 1; i <= NUM_DAYS; i++)
	{
		long long numNewborn = 0;
		for (int j = 0; j <= 8; j++)
		{
			if (j == 0)
			{
				numNewborn = fishMap[j];
			}
			else
			{
				fishMap[j - 1] = fishMap[j];
			}
		}

		fishMap[8] = numNewborn;
		fishMap[6] += numNewborn;
	}

	long long numFish = 0;
	for (int i = 0; i <= 8; i++)
		numFish += fishMap[i];

	printf("Number of Fish: %llu\n", numFish);

	return 0;
}
