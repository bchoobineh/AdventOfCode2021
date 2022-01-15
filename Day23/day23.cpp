#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>

#define A_START 2
#define B_START 4
#define C_START 6
#define D_START 8

struct Burrow
{
	int64_t energyCost;
	std::vector<char> hallway;
	std::vector<char> roomA;
	std::vector<char> roomB;
	std::vector<char> roomC;
	std::vector<char> roomD;

	Burrow(int64_t energyCost, std::vector<char> hallway, std::vector<char> roomA, std::vector<char> roomB, std::vector<char> roomC, std::vector<char> roomD)
	{
		this->energyCost = energyCost;
		this->hallway = hallway;
		this->roomA = roomA;
		this->roomB = roomB;
		this->roomC = roomC;
		this->roomD = roomD;
	}

	bool operator == (const Burrow b) const
	{
		for (int i = 0; i < hallway.size(); i++)
		{
			if (hallway[i] != b.hallway[i])
				return false;
		}

		for (int i = 0; i < roomA.size(); i++)
		{
			if (roomA[i] != b.roomA[i])
				return false;
		}

		for (int i = 0; i < roomB.size(); i++)
		{
			if (roomB[i] != b.roomB[i])
				return false;
		}

		for (int i = 0; i < roomC.size(); i++)
		{
			if (roomC[i] != b.roomC[i])
				return false;
		}

		for (int i = 0; i < roomD.size(); i++)
		{
			if (roomD[i] != b.roomD[i])
				return false;
		}

		return true;
	}

	bool operator < (const Burrow& burrow) const
	{
		return (energyCost < burrow.energyCost);
	}
};

struct CompareBurrowState
{
	bool operator()(const Burrow& b1, const Burrow& b2)
	{
		return (b1.energyCost > b2.energyCost);
	}
};


Burrow parseDataFile(std::string filePath)
{
	// File object
	std::fstream file(filePath);

	std::vector<char> hallway;
	std::vector<char> roomA;
	std::vector<char> roomB;
	std::vector<char> roomC;
	std::vector<char> roomD;

	// Extract hallway
	std::string str;
	std::getline(file, str);
	std::getline(file, str);
	hallway = std::vector<char>(str.length() - 2, '.');

	// Extract room data
	while(std::getline(file, str))
	{
		int roomIndex = 0;
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] != '#' && str[i] != ' ')
			{
				if (roomIndex == 0)
				{
					roomA.push_back(str[i]);
					roomIndex++;
				}
				else if (roomIndex == 1)
				{
					roomB.push_back(str[i]);
					roomIndex++;
				}
				else if (roomIndex == 2)
				{
					roomC.push_back(str[i]);
					roomIndex++;
				}
				else
				{
					roomD.push_back(str[i]);
				}
			}
		}
	}

	// Create and initialize burrow
	Burrow startingBurrow(0, hallway, roomA, roomB, roomC, roomD);
	return startingBurrow;
}

Burrow createEndingBurrow()
{
	std::vector<char> hallway(11, '.');
	
	std::vector<char> roomA;
	for (int i = 0; i < 2; i++)
		roomA.push_back('A');

	std::vector<char> roomB;
	for (int i = 0; i < 2; i++)
		roomB.push_back('B');

	std::vector<char> roomC;
	for (int i = 0; i < 2; i++)
		roomC.push_back('C');

	std::vector<char> roomD;
	for (int i = 0; i < 2; i++)
		roomD.push_back('D');

	Burrow burrow(0, hallway, roomA, roomB, roomC, roomD);
	return burrow;
}

int64_t getAmphiCost(char c)
{
	switch (c)
	{
		case 'A':
			return 1;

		case 'B':
			return 10;

		case 'C':
			return 100;

		case 'D':
			return 1000;
	}

	return -1;
}

int findAvailableElement(std::vector<char> room)
{
	int index = -1;
	for (int i = 0; i < room.size(); i++)
	{
		if (room[i] != '.')
		{
			index = i;
			break;
		}
	}

	return index;
}

int checkRoomAvailable(char c, Burrow burrow)
{
	int index = -1;
	switch (c)
	{
		case 'A':
		{
			for (int i = 0; i < burrow.roomA.size(); i++)
			{
				if (burrow.roomA[i] == '.')
					index = i;

				else if (burrow.roomA[i] != 'A')
					return -1;
			}
			break;
		}

		case 'B':
		{
			for (int i = 0; i < burrow.roomB.size(); i++)
			{
				if (burrow.roomB[i] == '.')
					index = i;

				else if (burrow.roomB[i] != 'B')
					return -1;
			}
			break;
		}

		case 'C':
		{
			for (int i = 0; i < burrow.roomC.size(); i++)
			{
				if (burrow.roomC[i] == '.')
					index = i;

				else if (burrow.roomC[i] != 'C')
					return -1;
			}
			break;
		}

		case 'D':
		{
			for (int i = 0; i < burrow.roomD.size(); i++)
			{
				if (burrow.roomD[i] == '.')
					index = i;

				else if (burrow.roomD[i] != 'D')
					return -1;
			}
			break;
		}

	}

	return index;
}

std::vector<Burrow> findAvailableMoves(Burrow& currentState)
{
	std::vector<Burrow> possibleBurrows;

	// Room Movements
	// First find index of top amphipod in each room
	int indexA = findAvailableElement(currentState.roomA);
	int indexB = findAvailableElement(currentState.roomB);
	int indexC = findAvailableElement(currentState.roomC);
	int indexD = findAvailableElement(currentState.roomD);

	// Try to move top amphipod of room A
	if (indexA != -1)
	{
		// Attempt to move this pieces into the left hallway
		for (int i = A_START; i >= 0; i--)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexA + 1) + (A_START - i);
			int64_t cost = numTiles * getAmphiCost(currentState.roomA.at(indexA));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomA[indexA];
			b.roomA[indexA] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}

		// Attempt to move this pieces into the right hallway
		for (int i = A_START; i < 11; i++)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexA + 1) + (i - A_START);
			int64_t cost = numTiles * getAmphiCost(currentState.roomA.at(indexA));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomA[indexA];
			b.roomA[indexA] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}
	}

	// Try to move top amphipod of room B
	if (indexB != -1)
	{
		// Attempt to move this pieces into the left hallway
		for (int i = B_START; i >= 0; i--)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexB + 1) + (B_START - i);
			int64_t cost = numTiles * getAmphiCost(currentState.roomB.at(indexB));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomB[indexB];
			b.roomB[indexB] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}

		// Attempt to move this pieces into the right hallway
		for (int i = B_START; i < 11; i++)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexB + 1) + (i - B_START);
			int64_t cost = numTiles * getAmphiCost(currentState.roomB.at(indexB));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomB[indexB];
			b.roomB[indexB] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}
	}

	// Try to move top amphipod of room C
	if (indexC != -1)
	{
		// Attempt to move this pieces into the left hallway
		for (int i = C_START; i >= 0; i--)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexC + 1) + (C_START - i);
			int64_t cost = numTiles * getAmphiCost(currentState.roomC.at(indexC));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomC[indexC];
			b.roomC[indexC] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}

		// Attempt to move this pieces into the right hallway
		for (int i = C_START; i < 11; i++)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexC + 1) + (i - C_START);
			int64_t cost = numTiles * getAmphiCost(currentState.roomC.at(indexC));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomC[indexC];
			b.roomC[indexC] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}
	}

	// Try to move top amphipod of room D
	if (indexD != -1)
	{
		// Attempt to move this pieces into the left hallway
		for (int i = D_START; i >= 0; i--)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexD + 1) + (D_START - i);
			int64_t cost = numTiles * getAmphiCost(currentState.roomD.at(indexD));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomD[indexD];
			b.roomD[indexD] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}

		// Attempt to move this pieces into the right hallway
		for (int i = D_START; i < 11; i++)
		{
			// Blockage
			if (currentState.hallway[i] != '.')
				break;

			// Can't move into this slot 
			if (i == A_START || i == B_START || i == C_START || i == D_START)
				continue;

			// Can move into this spot
			int numTiles = (indexD + 1) + (i - D_START);
			int64_t cost = numTiles * getAmphiCost(currentState.roomD.at(indexD));

			Burrow b = currentState;
			b.energyCost += cost;

			// Swap
			b.hallway[i] = b.roomD[indexD];
			b.roomD[indexD] = '.';

			// Add to buffer
			possibleBurrows.push_back(b);
		}
	}

	// Consider any amphipod currently in a hallway
	// If in a hallway, only valid move is to try to move to its appropriate room if possible
	for (int i = 0; i < currentState.hallway.size(); i++)
	{
		// Only care if it is a amphipod
		if (currentState.hallway[i] != '.')
		{
			// Check if room is available
			int roomInd = checkRoomAvailable(currentState.hallway[i], currentState);

			// Room is not free
			if (roomInd < 0)
				continue;

			// Find Destination
			int destination = 0;
			switch (currentState.hallway[i])
			{
				case 'A':
					destination = A_START;
					break;

				case 'B':
					destination = B_START;
					break;

				case 'C':
					destination = C_START;
					break;

				case 'D':
					destination = D_START;
					break;
			}


			// Check left path
			bool validPath = true;
			if (destination < i)
			{
				for (int j = i - 1; j >= destination; j--)
				{
					if (currentState.hallway[j] != '.')
					{
						validPath = false;
						break;
					}
				}

			}

			// Check right path
			else
			{
				for (int j = i + 1; j <= destination; j++)
				{
					if (currentState.hallway[j] != '.')
					{
						validPath = false;
						break;
					}
				}
			}

			// If path is blocked
			if (validPath == false)
				continue;

			// Calculate cost
			int numTiles = (roomInd + 1) + (std::abs(destination - i));
			int cost = numTiles * getAmphiCost(currentState.hallway[i]);

			// Create new burrow state
			Burrow b = currentState;
			b.energyCost += cost;

			// Switch positions
			switch (currentState.hallway[i])
			{
				case 'A':
				{
					currentState.roomA[roomInd] = 'A';
					currentState.hallway[i] = '.';
					break;
				}
	
				case 'B':
				{
					currentState.roomB[roomInd] = 'B';
					currentState.hallway[i] = '.';
					break;
				}

				case 'C':
				{
					currentState.roomC[roomInd] = 'C';
					currentState.hallway[i] = '.';
					break;
				}

				case 'D':
				{
					currentState.roomD[roomInd] = 'D';
					currentState.hallway[i] = '.';
					break;
				}
			}

			// Add to possible positions
			possibleBurrows.push_back(b);
		}
	}



	return possibleBurrows;
}


int main(int argc, char* argv[])
{
	// Extract starting burrow state from data file
	Burrow startingBurrow = parseDataFile("day23.txt");

	// Create ideal burrow state
	Burrow endingBurrow = createEndingBurrow();

	// Priority queue for dikjastras 
	std::priority_queue<Burrow, std::vector<Burrow>, CompareBurrowState> q;

	// What burrow's we've visited
	std::set<Burrow> visited;

	// Add starting burrow to queue
	q.push(startingBurrow);

	while (!q.empty())
	{
		// Grab top Burrow statee
		Burrow burrow = q.top();
		q.pop();

		// We have reached the end!
		if (burrow == endingBurrow)
		{
			printf("Cost: %llu\n", burrow.energyCost);
			break;
		}

		// If we've visited this burrow already then skip
		if (visited.find(burrow) != visited.end())
			continue;

		// Add to visited
		visited.insert(burrow);

		std::vector<Burrow> possibleMoves = findAvailableMoves(burrow);
		for (int i = 0; i < possibleMoves.size(); i++)
		{
			q.push(possibleMoves[i]);
		}
	}


	return 0;
}
