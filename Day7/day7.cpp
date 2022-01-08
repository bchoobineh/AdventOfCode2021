#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

int calculateFuelCost(int steps)
{
	int result = 0;
	int count = 1;
	for (int i = 0; i < steps; i++)
	{
		result += count;
		count++;
	}

	return result;
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day7.txt");

	// Crab position data buffer
	std::vector<int> crabPositionBuffer;
	int smallest = INT_MAX;
	int largest = -1;


	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);
		std::string tmp;
		while (std::getline(ss, tmp, ','))
		{
			int num = std::stoi(tmp);
			if (num < smallest)
				smallest = num;
			if (num > largest)
				largest = num;


			crabPositionBuffer.push_back(num);
		}
	}
	file.close();

	long long fuelCost = -1;
	for (int i = smallest; i <= largest; i++)
	{
		long long cost = 0;
		for (int j = 0; j < crabPositionBuffer.size(); j++)
		{
			int distance = std::abs(crabPositionBuffer.at(j) - i);
			cost += calculateFuelCost(distance);
		}

		if (fuelCost == -1)
			fuelCost = cost;
		else
		{
			if (cost < fuelCost)
				fuelCost = cost;
		}
	}

	printf("Fuel Cost: %llu\n", fuelCost);

	return 0;
}