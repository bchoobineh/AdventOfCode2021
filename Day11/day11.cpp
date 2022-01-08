#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

#define NUM_STEPS 2000

struct Node
{
	int energy; // Energy Level
	bool flashed; // Whether this node has flashed during the current step

	Node() :
		energy(0),
		flashed(false)
	{}

	Node(int startingEnergy) :
		energy(startingEnergy),
		flashed(false)
	{}
};

void resetFlashToggle(std::vector<std::vector<Node>>& dataBuffer)
{
	for (int i = 0; i < dataBuffer.size(); i++)
	{
		for (int j = 0; j < dataBuffer[i].size(); j++)
		{
			dataBuffer[i][j].flashed = false;
		}
	}
}

bool validSpot(std::vector<std::vector<Node>> dataBuffer, int col, int row)
{
	if (col < 0 || row < 0 || col >= dataBuffer.size() || row >= dataBuffer[0].size())
		return false;

	return true;
}

void printDataBuffer(std::vector<std::vector<Node>> dataBuffer)
{
	for (int i = 0; i < dataBuffer.size(); i++)
	{
		printf("\n");
		for (int j = 0; j < dataBuffer[0].size(); j++)
		{
			printf("%d", dataBuffer[i][j].energy);
		}
	}

	printf("\n\n");
}

void handleFlash(int* flashCount, std::vector<std::vector<Node>>& dataBuffer, int col, int row)
{

	// Flash count goes up
	*flashCount = *flashCount + 1;

	// Current Node has flashed and needs to be marked
	dataBuffer[col][row].flashed = true;

	// Each node has up to 8 possible neighbors. Consider only valid ones
	std::vector<std::pair<int, int>> neighbors;

	// Top
	if (validSpot(dataBuffer, col - 1, row))
		neighbors.push_back(std::pair<int, int>(col - 1, row));

	// Bot
	if (validSpot(dataBuffer, col + 1, row))
		neighbors.push_back(std::pair<int, int>(col + 1, row));

	// Left
	if (validSpot(dataBuffer, col, row - 1))
		neighbors.push_back(std::pair<int, int>(col, row-1));

	// Right
	if (validSpot(dataBuffer, col, row + 1))
		neighbors.push_back(std::pair<int, int>(col, row+1));

	// Top Left
	if (validSpot(dataBuffer, col - 1, row - 1))
		neighbors.push_back(std::pair<int, int>(col - 1, row - 1));

	// Top Right
	if (validSpot(dataBuffer, col - 1, row + 1))
		neighbors.push_back(std::pair<int, int>(col - 1, row + 1));

	// Bot Left
	if (validSpot(dataBuffer, col + 1, row - 1))
		neighbors.push_back(std::pair<int, int>(col + 1, row - 1));

	// Bot Right
	if (validSpot(dataBuffer, col + 1, row + 1))
		neighbors.push_back(std::pair<int, int>(col + 1, row + 1));

	// Consider all possible neighbors
	for (int i = 0; i < neighbors.size(); i++)
	{
		// Neighbor energy increases if they haven't flashed
		if(dataBuffer[neighbors[i].first][neighbors[i].second].flashed == false)
			dataBuffer[neighbors[i].first][neighbors[i].second].energy++;

		if (dataBuffer[neighbors[i].first][neighbors[i].second].energy > 9 &&
			dataBuffer[neighbors[i].first][neighbors[i].second].flashed == false)
		{
			dataBuffer[neighbors[i].first][neighbors[i].second].energy = 0;
			handleFlash(flashCount, dataBuffer, neighbors[i].first, neighbors[i].second);
		}

	}


}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day11.txt");

	// Octopus node data buffer
	std::vector<std::vector<Node>> dataBuffer;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::vector<Node> dataRow;
		for (int i = 0; i < str.length(); i++)
		{
			int digit = str[i] - '0';
			Node n(digit);
			dataRow.push_back(n);
		}

		dataBuffer.push_back(dataRow);
	}
	file.close();

	// Number of flashes
	int numFlashes = 0;

	// Process each step
	for (int i = 0; i < NUM_STEPS; i++)
	{
		for (int j = 0; j < dataBuffer.size(); j++)
		{
			for (int k = 0; k < dataBuffer[j].size(); k++)
			{
				// Grow in Energy each step if it is unflashed
				if(dataBuffer[j][k].flashed == false)
					dataBuffer[j][k].energy++;

				// Check if a flash is happening
				if (dataBuffer[j][k].energy > 9 && dataBuffer[j][k].flashed == false)
				{
					dataBuffer[j][k].energy = 0;
					handleFlash(&numFlashes, dataBuffer, j, k);
				}
			}
		}

		// Check for sync
		bool sync = true;
		for (int j = 0; j < dataBuffer.size(); j++)
		{
			for (int k = 0; k < dataBuffer[j].size(); k++)
			{
				if (dataBuffer[j][k].flashed == false)
				{
					sync = false;
					break;
				}
			}
		}

		if (sync)
		{
			printf("All synced on Turn: %d\n", i+1);
			break;
		}


		
		// Reset Flashed toggle for all nodes before moving to next step
		resetFlashToggle(dataBuffer);
	}

	// Number of Flashes after all steps
	printf("Number of Flashes Total: %d\n", numFlashes);



	return 0;
}