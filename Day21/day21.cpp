#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

// Cases
// ------------
// 1 1 1 - 3
// 1 1 2 - 4
// 1 1 3 - 5
// 1 2 1 - 4
// 1 2 2 - 5
// 1 2 3 - 6
// 1 3 1 - 5
// 1 3 2 - 6
// 1 3 3 - 7
// 
// 2 1 1 - 4
// 2 1 2 - 5
// 2 1 3 - 6
// 2 2 1 - 5
// 2 2 2 - 6
// 2 2 3 - 7
// 2 3 1 - 6
// 2 3 2 - 7
// 2 3 3 - 8
// 
// 3 1 1 - 5
// 3 1 2 - 6
// 3 1 3 - 7
// 3 2 1 - 6
// 3 2 2 - 7
// 3 2 3 - 8
// 3 3 1 - 7
// 3 3 2 - 8
// 3 3 3 - 9

// (Val, Frequency)
const std::unordered_map<int, int> diceOps = { {3,1}, {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1} };



struct GameState
{
	int p1Position; // Range between 1 and 10
	int p2Position; // Range between 1 and 10

	int p1Score = 0; // Range between 0 and 20
	int p2Score = 0; // Range between 0 and 20

	bool turn = true; // Whose turn it is

	GameState(int pos1, int pos2) : p1Position(pos1), p2Position(pos2) {};

	auto operator == (const GameState& other) const
	{
		return
			(p1Position == other.p1Position &&
				p2Position == other.p2Position &&
				p1Score == other.p1Score &&
				p2Score == other.p2Score &&
				turn == other.turn);
	}
};




class GameHash
{
public:

	size_t operator()(const GameState& state) const
	{
		return (state.p1Position + (state.p2Position * 100) + (state.p1Score * 1000) + (state.p2Score * 10000) + (state.turn * 50000000));
	}
};

void playGame(std::unordered_map<GameState, long long, GameHash>& gameUniverses, long long& p1Wins, long long& p2Wins)
{
	std::unordered_map<GameState, long long, GameHash> newGames;

	std::unordered_map<GameState, long long, GameHash>::iterator iter;
	for (iter = gameUniverses.begin(); iter != gameUniverses.end(); iter++)
	{
		for (int i = 3; i <= 9; i++)
		{
			GameState gs = iter->first;

			// P1's Turn
			if (gs.turn)
			{
				gs.p1Position = ((gs.p1Position + i - 1) % 10) + 1;
				gs.p1Score += gs.p1Position;
			}
			// P2's Turn
			else
			{
				gs.p2Position = ((gs.p2Position + i - 1) % 10) + 1;
				gs.p2Score += gs.p2Position;
			}

			// Change turn
			gs.turn = !gs.turn;

			long long newUniverses = iter->second * diceOps.at(i);

			if (gs.p1Score >= 21)
			{
				p1Wins += newUniverses;
			}
			else if (gs.p2Score >= 21)
			{
				p2Wins += newUniverses;
			}

			else
			{
				newGames[gs] += newUniverses;
			}

		}
	}

	gameUniverses.swap(newGames);
}



int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day21.txt");

	// Parse out Player 1 and Player 2's starting spots on the board
	std::string str;
	std::getline(file, str);
	int position1 = (str[str.length() - 1]) - '0';

	std::getline(file, str);
	int position2 = (str[str.length() - 1]) - '0';
	//position1 = 10;


	printf("Start 1: %d  Start2: %d\n", position1, position2);

	// Initialize initial game state
	GameState initialState(position1, position2);

	// Hashmap of all universes
	std::unordered_map<GameState, long long, GameHash> gameUniverses;
	gameUniverses[initialState] = 1;

	long long p1Score = 0;
	long long p2Score = 0;

	while (!gameUniverses.empty())
	{
		playGame(gameUniverses, p1Score, p2Score);

	}

	printf("P1: %llu  P2: %llu\n", p1Score, p2Score);


	return 0;
}