#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void updateBingoBoard(int drawnNumber, std::vector<std::vector<int>> &bingoBoard)
{
	// Go through bingo board and flip bingo tile to -1 if it matches the drawn number
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (bingoBoard[i][j] == drawnNumber)
				bingoBoard[i][j] = -1;
		}
	}
}

bool checkWinner(std::vector<std::vector<int>> bingoBoard)
{
	// Check rows
	for (int i = 0; i < 5; i++)
	{
		bool bingo = true;
		for (int j = 0; j < 5; j++)
		{
			if (bingoBoard[i][j] != -1)
			{
				bingo = false;
				break;
			}
		}

		if (bingo)
			return true;
	}

	// Check columns
	for (int i = 0; i < 5; i++)
	{
		bool bingo = true;
		for (int j = 0; j < 5; j++)
		{
			if (bingoBoard[j][i] != -1)
			{
				bingo = false;
				break;
			}
		}

		if (bingo)
			return true;
	}
	
	return false;
}

int calculateScore(std::vector<std::vector<int>> bingoBoard)
{
	int score = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (bingoBoard[i][j] != -1)
				score += bingoBoard[i][j];
		}
	}

	return score;
}

int main(int argc, char* argv[])
{
	// Drawn Numbers
	std::vector<int> drawnNumbers;

	// Bingo Boards
	std::vector<std::vector<std::vector<int>>> bingoBoards;

	// Extract data from data file
	std::ifstream file("day4.txt");

	// Read data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		// Process first line
		if (drawnNumbers.empty())
		{
			// Construct stream 
			std::stringstream ss(str);
			std::string tmp;
			while (std::getline(ss, tmp, ','))
			{
				drawnNumbers.push_back(std::stoi(tmp));
			}

			// Skip next empty line
			std::getline(file, str);
			continue;
		}

		// Process bingo board
		std::vector<std::vector<int>> board;
		for (int i = 0; i < 5; i++)
		{
			std::vector<int> row;

			// Construct stream
			std::stringstream ss(str);
			std::string tmp;
			while (std::getline(ss, tmp, ' '))
			{
				// Ignore if forward whitepace in 1 digit number
				if (tmp.size() < 1)
					continue;

				row.push_back(std::stoi(tmp));
			}

			board.push_back(row);

			// Get next line
			std::getline(file, str);
		}
		bingoBoards.push_back(board);				
	}

	// Close the file
	file.close();

	// Process drawn numbers until winner is found
	for (int i = 0; i < drawnNumbers.size(); i++)
	{
		// Update all bingo boards with the drawn number
		for (int j = 0; j < bingoBoards.size(); j++)
		{
			updateBingoBoard(drawnNumbers[i], bingoBoards[j]);
		}

		// Check for winners
		for (int j = 0; j < bingoBoards.size(); j++)
		{
			bool wasWinner = checkWinner(bingoBoards[j]);
			if (wasWinner && bingoBoards.size() <= 1)
			{
				printf("Score: %d\n", calculateScore(bingoBoards[j]) * drawnNumbers[i]);
				return 0;
			}
			else if (wasWinner)
			{
				// Erase this bingo card as it is not the final winner
				bingoBoards.erase(bingoBoards.begin() + j);
			}
		}
	}


	return 0;
}