#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

void basinFinder(std::vector<std::vector<bool>>& visited, std::vector<std::vector<int>> heightMap, int* size,
	int col, int row)
{
	// Handle stop case for recursion
	if (row < 0 || col < 0 || col >= heightMap.size() || row >= heightMap[0].size() || heightMap[col][row] == 9 ||
		visited[col][row] == true)
	{
		return;
	}

	// Increase current basin size
	*size = *size + 1;

	// Mark current node as visited
	visited[col][row] = true;

	// Consider moving 4 other directions
	basinFinder(visited, heightMap, size, col + 1, row);
	basinFinder(visited, heightMap, size, col - 1, row);
	basinFinder(visited, heightMap, size, col, row + 1);
	basinFinder(visited, heightMap, size, col, row - 1);
}

bool isLowest(std::vector<std::vector<int>> heightMap, int col, int row)
{
	int height = heightMap.size();
	int width = heightMap[0].size();

	std::vector<std::pair<int, int>> neighbors;

	// Left Neighbor
	if ((row - 1) >= 0)
	{
		std::pair<int, int> tmp(col, row - 1);
		neighbors.push_back(tmp);
	}

	// Right Neighbor
	if ((row + 1) < width)
	{
		std::pair<int, int> tmp(col, row + 1);
		neighbors.push_back(tmp);
	}

	// Top Neighbor
	if ((col - 1) >= 0)
	{
		std::pair<int, int> tmp(col-1, row);
		neighbors.push_back(tmp);
	}

	// Bot Neighbor
	if ((col + 1) < height)
	{
		std::pair<int, int> tmp(col + 1, row);
		neighbors.push_back(tmp);
	}

	int sourceVal = heightMap[col][row];

	for (int i = 0; i < neighbors.size(); i++)
	{
		if (heightMap[neighbors[i].first][neighbors[i].second] <= sourceVal)
			return false;
	}

	return true;
}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day9.txt");

	// Height map data buffer
	std::vector<std::vector<int>> heightMap;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::vector<int> heightRow;
		for (int j = 0; j < str.length(); j++)
			heightRow.push_back(str.at(j) - '0');

		heightMap.push_back(heightRow);
	}
	file.close();

	// Data buffer of low points
	std::vector<int> lowPoints;

	// Iterate through entire height map
	for (int i = 0; i < heightMap.size(); i++)
	{
		for (int j = 0; j < heightMap.at(i).size(); j++)
		{
			if (isLowest(heightMap, i, j))
				lowPoints.push_back(1 + heightMap[i][j]);

		}
	}

	// Sum
	int sum = 0;
	for (int i = 0; i < lowPoints.size(); i++)
		sum += lowPoints[i];
	printf("Sum: %d\n", sum);

	// Create a visited mapping
	std::vector<std::vector<bool>> visited;
	for (int i = 0; i < heightMap.size(); i++)
	{
		std::vector<bool> row(heightMap[0].size(), false);
		visited.push_back(row);
	}

	// Find Basins
	std::vector<int> basinSizes;
	for (int i = 0; i < heightMap.size(); i++)
	{
		for (int j = 0; j < heightMap[i].size(); j++)
		{
			// Only do Basin search if node hasn't been visited yet and does not have 
			// height value equal to 9
			if (visited[i][j] == false && heightMap[i][j] != 9)
			{
				// Use Recursive Helper function
				int curSize = 0;
				basinFinder(visited, heightMap, &curSize, i, j);
				basinSizes.push_back(curSize);

			}
		}
	}

	std::sort(basinSizes.begin(), basinSizes.end());
	int numBasins = basinSizes.size();
	int prod = basinSizes[numBasins - 1] * basinSizes[numBasins - 2] * basinSizes[numBasins - 3];
	printf("Product of top 3 basin sizes: %d\n", prod);



	return 0;
}