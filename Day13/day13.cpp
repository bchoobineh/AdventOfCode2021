#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

int visibleDots(std::vector<std::vector<char>> dataBuffer, int height, int width)
{
	int dotCount = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (dataBuffer[i][j] == '#')
				dotCount++;
		}
	}

	return dotCount;
}

void drawBuffer(std::vector<std::vector<char>> dataBuffer, int height, int width)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c", dataBuffer[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");
}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day13.txt");

	// Data points
	std::vector<std::pair<int, int>> dataPairs;
	bool pairsDone = false;

	// Folding instructions
	std::vector<std::pair<std::string, int>> foldData;

	// Find grid dimensions
	int width = -1;
	int height = -1;


	// Parse data line by line
	std::string str;
	while (std::getline(file, str))
	{
		// If string is empty then moving to fold instructions
		if (str.length() < 1)
		{
			pairsDone = true;
			continue;
		}

		// Grab (x,y) data point
		if (!pairsDone)
		{
			std::stringstream ss(str);
			std::string tmp;
			std::getline(ss, tmp, ',');
			int x = std::stoi(tmp);
			width = std::max(x+1, width);

			std::getline(ss, tmp, ',');
			int y = std::stoi(tmp);
			height = std::max(y+1, height);

			dataPairs.push_back(std::pair<int, int>(x, y));
		}

		// Grab folding instructions
		else
		{
			// Grab 3rd space delim string
			std::stringstream ss(str);
			std::string tmp;
			std::getline(ss, tmp, ' ');
			std::getline(ss, tmp, ' ');
			std::getline(ss, tmp, ' ');

			// Delimit this string with delimiter '='
			std::string eq = tmp;
			std::stringstream ssEq(eq);
			
			std::getline(ssEq, tmp, '=');
			std::string axis = tmp;

			std::getline(ssEq, tmp, '=');
			int val = std::stoi(tmp);

			foldData.push_back(std::pair<std::string, int>(axis, val));
		}
	}
	file.close();

	// Create data buffer to hold points
	std::vector<std::vector<char>> dataBuffer;
	for (int i = 0; i < height; i++)
	{
		std::vector<char> row(width, '.');
		dataBuffer.push_back(row);
	}

	// Initialize data buffer from data pairs
	for (int i = 0; i < dataPairs.size(); i++)
	{
		dataBuffer[dataPairs[i].second][dataPairs[i].first] = '#';
	}

	// Perform folding actions
	for (int i = 0; i < foldData.size(); i++)
	{
		int num = foldData[i].second;

		// Handle X and Y axis folds		
		// X axis fold
		if (foldData[i].first == "x")
		{
			// Copy everything for right side to left side
			for (int j = 0; j < height; j++)
			{
				for (int k = num+1; k < width; k++)
				{
					if (dataBuffer[j][k] == '#')
					{
						dataBuffer[j][(num-1) - (k - (num+1))] = '#';
					}
				}
			}

			// Update width
			width = num;

		}

		// Y axis fold
		else
		{
			// Copy everything from lower half to upper half
			for (int j = num+1; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					if (dataBuffer[j][k] == '#')
						dataBuffer[(num - 1) - (j - (num + 1))][k] = '#';
				}
			}

			// Update height
			height = num;
		}
	}

	// Number of Dots
	int numDots = visibleDots(dataBuffer, height, width);


	printf("Width: %d  Height: %d\n", width, height);
	printf("Number of Dots: %d\n", numDots);
	drawBuffer(dataBuffer, height, width);

	return 0;
}