#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <queue>

typedef std::pair<int, std::pair<int, int>> pr;

int getGridUnit(int x, int y, int originalWidth, int originalHeight, std::vector<std::vector<int>> riskLevels)
{
	int data = riskLevels[(x % originalHeight)][(y % originalWidth)] + (x / originalHeight) + (y / originalWidth);
	return ((data - 1) % 9) + 1;
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day15.txt");

	// Data buffer holding risk levels
	std::vector<std::vector<int>> riskLevels;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::vector<int> riskRow;
		for (int i = 0; i < str.length(); i++)
			riskRow.push_back(str[i] - '0');

		riskLevels.push_back(riskRow);
	}
	file.close();

	// Maze dimensions
	int originalHeight = riskLevels.size();
	int originalWidth = riskLevels[0].size();

	int height = riskLevels.size() * 5;
	int width = riskLevels[0].size() * 5;

	// Create lowest risk mapping
	std::vector<std::vector<int>> lowestRisk;
	for (int i = 0; i < height; i++)
	{
		std::vector<int> tmpRow;
		for (int j = 0; j < width; j++)
			tmpRow.push_back(INT_MAX);

		lowestRisk.push_back(tmpRow);
	}

	// Priority queue for Dijkstra's algorithm
	// Format: (risk, (x, y))
	//std::priority_queue<std::pair<int, std::pair<int, int>>> pq;
	std::priority_queue<pr, std::vector<pr>, std::greater<pr>> pq;

	// Add starting location to priority queue
	pq.push(std::make_pair(0, std::make_pair(0, 0)));

	// Keep track of elements we have visited via a set
	// Format: (x,y)
	std::set<std::pair<int, int>> visited;

	// Go until the priority queue is empty or otherwise
	while (!pq.empty())
	{
		// Grab top element
		std::pair<int, std::pair<int, int>> node = pq.top();
		int cost = node.first;
		int row = node.second.first;
		int col = node.second.second;

		// Pop top element
		pq.pop();

		// If we have already visited this, then skip
		if (visited.find(node.second) != visited.end())
			continue;

		// Add to visited
		visited.insert(node.second);

		// Update lowest risk cost
		lowestRisk[row][col] = cost;

		// If we have gotten to the destination then we can break out
		if (row == (height - 1) && col == (width - 1))
			break;

		// Consider neighbors

		// Top neighbor
		if (row > 0)
		{
			std::pair<int, std::pair<int, int>> dataNode;
			dataNode.first = cost + getGridUnit(row-1, col, originalWidth, originalHeight, riskLevels);
			dataNode.second = std::make_pair(row - 1, col);
			pq.push(dataNode);
		}

		// Bot Neighbor
		if ((row + 1) < height)
		{
			std::pair<int, std::pair<int, int>> dataNode;
			dataNode.first = cost + getGridUnit(row + 1, col, originalWidth, originalHeight, riskLevels);
			dataNode.second = std::make_pair(row + 1, col);
			pq.push(dataNode);
		}

		// Left Neighbor
		if (col > 0)
		{
			std::pair<int, std::pair<int, int>> dataNode;
			dataNode.first = cost + getGridUnit(row, col - 1, originalWidth, originalHeight, riskLevels);
			dataNode.second = std::make_pair(row, col - 1);
			pq.push(dataNode);
		}

		// Right Neighbor
		if ((col + 1) < width)
		{
			std::pair<int, std::pair<int, int>> dataNode;
			dataNode.first = cost + getGridUnit(row, col + 1, originalWidth, originalHeight, riskLevels);
			dataNode.second = std::make_pair(row, col + 1);
			pq.push(dataNode);
		}
	}

	printf("Lowest Risk Path: %d\n", lowestRisk[height - 1][width - 1]);



	return 0;
}