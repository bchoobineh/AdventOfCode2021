#include <stdio.h>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>

void clearStack(std::stack<char>& charStack)
{
	while (!charStack.empty())
	{
		charStack.pop();
	}
}

int getSymbolScore(char c)
{
	if (c == ')')
		return 3;
	else if (c == ']')
		return 57;
	else if (c == '}')
		return 1197;
	else
		return 25137;
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day10.txt");

	// Character stack used to check syntax validity 
	std::stack<char> charStack;

	// Autocomplete scores
	std::vector<long long> autoScores;

	// Syntax Score
	long long syntaxScore = 0;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		// Clear the stack every line
		clearStack(charStack);

		// Corruption Check
		bool corrupted = false;

		for (int i = 0; i < str.length(); i++)
		{
			char c = str[i];

			// Left hand side of chunk
			if (c == '(' || c == '{' || c == '[' || c == '<')
			{
				charStack.push(c);
				continue;
			}

			else if (c == ')')
			{
				if (charStack.empty())
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}

				// Valid Chunk
				else if (charStack.top() == '(')
				{
					charStack.pop();
					continue;
				}
				// Invalid Chunk
				else
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}
			}

			else if (c == ']')
			{
				if (charStack.empty())
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}

				// Valid Chunk
				if (charStack.top() == '[')
				{
					charStack.pop();
					continue;
				}
				// Invalid Chunk
				else
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}
			}

			else if (c == '}')
			{
				if (charStack.empty())
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}

				// Valid Chunk
				if (charStack.top() == '{')
				{
					charStack.pop();
					continue;
				}
				// Invalid Chunk
				else
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}
			}

			else if (c == '>')
			{
				if (charStack.empty())
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}

				// Valid Chunk
				if (charStack.top() == '<')
				{
					charStack.pop();
					continue;
				}
				// Invalid Chunk
				else
				{
					corrupted = true;
					syntaxScore += getSymbolScore(c);
					break;
				}
			}
		}

		// If corrupted then don't do next steps
		if (corrupted)
			continue;

		// Calc autocomplete score if not corrupted
		long long autoScore = 0;
		while (!charStack.empty())
		{
			autoScore *= 5;
			
			char c = charStack.top();
			charStack.pop();
			if (c == '(')
				autoScore += 1;
			else if (c == '[')
				autoScore += 2;
			else if (c == '{')
				autoScore += 3;
			else
				autoScore += 4;
		}

		autoScores.push_back(autoScore);


	}
	file.close();

	// Total Syntax Score:
	printf("Syntax Score: %llu \n", syntaxScore);

	// Find middle autocomplete score
	std::sort(autoScores.begin(), autoScores.end());

	long long midScore = autoScores[autoScores.size() / 2];
	printf("Middle Auto Score: %llu\n", midScore);


	return 0;
}