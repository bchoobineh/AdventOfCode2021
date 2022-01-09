#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <set>

void findPaths(std::map<std::string, std::vector<std::string>> adjmap, int* numPaths, 
	std::set<std::string> visited, std::string currentNode, bool cavePass)
{
	// Found a path
	if (currentNode == "end")
	{
		*numPaths = *numPaths + 1;
		return;
	}

	// Add room to visited
	visited.insert(currentNode);

	// Movement options
	std::vector<std::string> options = adjmap[currentNode];

	// Consider each option
	for (int i = 0; i < options.size(); i++)
	{
		// Don't ever revisit the start
		if (options[i] == "start")
			continue;

		// We have visited this room before
		if (visited.find(options[i]) != visited.end())
		{
			// If this room is a Capital letter room then we can revisit
			if (std::isupper(options[i][0]))
			{
				findPaths(adjmap, numPaths, visited, options[i], cavePass);
			}

			// Do we have small cave pass?
			else if(cavePass)
			{
				findPaths(adjmap, numPaths, visited, options[i], false);
			}
		}
		
		// We have not visited this room before
		else
		{
			findPaths(adjmap, numPaths, visited, options[i], cavePass);
		}


	}

}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day12.txt");

	// Adjacency Map
	std::map<std::string, std::vector<std::string>> adjmap;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);
		std::string tmp;

		// Get Source
		std::getline(ss, tmp, '-');
		std::string src = tmp;

		// Get Dest
		std::getline(ss, tmp, '-');
		std::string dst = tmp;

		// Add to adj map
		adjmap[src].push_back(dst);
		adjmap[dst].push_back(src);
	}
	file.close();

	// Number of paths found
	int numPaths = 0;

	// Visited rooms
	std::set<std::string> visited;

	// Find Paths starting from "start" and ending at "end"
	findPaths(adjmap, &numPaths, visited, "start", true);

	printf("Number of Paths: %d\n", numPaths);



	return 0;
}