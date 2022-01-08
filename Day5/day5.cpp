#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

std::pair<int, int> extractPoint(std::string str)
{
	int x = 0;
	int y = 0;

	std::stringstream ss(str);
	std::string tmp;
	
	// Get X value
	std::getline(ss, tmp, ',');
	x = std::stoi(tmp);

	// Get Y value
	std::getline(ss, tmp, ',');
	y = std::stoi(tmp);

	std::pair<int, int> result(x, y);
	return result;
}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::ifstream file("day5.txt");

	// Source and destination point buffers
	std::vector<std::pair<int, int>> sources;
	std::vector<std::pair<int, int>> dests;

	// Read data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream spaceStream(str);
		std::string spaceStr; 

		// Get source pair
		std::getline(spaceStream, spaceStr, ' ');
		sources.push_back(extractPoint(spaceStr));

		// Skip Arrow
		std::getline(spaceStream, spaceStr, ' ');

		// Get dest pair
		std::getline(spaceStream, spaceStr, ' ');
		dests.push_back(extractPoint(spaceStr));
	}
	file.close();

	// Get board dimensions
	int maxHorizontal = 0;
	int maxVertical = 0;
	for (int i = 0; i < sources.size(); i++)
	{
		int x = std::max(sources.at(i).first, dests.at(i).first);
		maxHorizontal = std::max(x, maxHorizontal);

		int y = std::max(sources.at(i).second, dests.at(i).second);
		maxVertical = std::max(y, maxVertical);
	}

	// Create board
	std::vector<std::vector<int>> board;
	for (int i = 0; i < maxVertical+1; i++)
	{
		std::vector<int> tmp(maxHorizontal+1, 0);
		board.push_back(tmp);
	}

	// Update board with each line
	for (int i = 0; i < sources.size(); i++)
	{
		std::pair<int, int> src = sources.at(i);
		std::pair<int, int> dst = dests.at(i);

		// Handle infinite slope case. Happens in vertical line where x2 = x1
		if (dst.first == src.first)
		{
			int lower = std::min(src.second, dst.second);
			int upper = std::max(src.second, dst.second);
			for (int j = lower; j <= upper; j++)
				board[j][src.first]++;

			continue;
		}

		// Calculate slope
		int slope = (dst.second - src.second) / (dst.first - src.first);

		// Find leftmost point and rightmost index
		std::pair<int, int> left;
		int right;
		if (src.first < dst.first)
		{
			left = src;
			right = dst.first;
		}
		else
		{
			left = dst;
			right = src.first;
		}

		for (int j = left.first; j <= right; j++)
		{
			board[left.second][j]++;
			left.second += slope;
		}
	}

	// Determine number of spots with value 2 or higher
	int result = 0;
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board.at(i).size(); j++)
		{
			if (board[i][j] >= 2)
				result++;
		}
	}
	printf("Number of Overlaps: %d\n\n", result);




	return 0;
}