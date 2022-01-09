#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

#define NUM_STEPS 40

int main(int argc, char* argv[])
{
	// Extract data from a data file
	std::fstream file("day14.txt");

	// Starting polymer template
	std::string polymer = "";

	// Map of polymer insertion rules
	std::map<std::string, std::string> polyRules;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		// Skip empty line
		if (str.length() < 1)
			continue;

		// Grab starting polymer template
		if (polymer.length() < 1)
		{
			polymer = str;
		}		
		// Grab polymer insertion rules
		else 
		{
			std::stringstream ss(str);
			std::string tmp;
			std::getline(ss, tmp, ' ');

			std::string key;
			key = tmp;

			std::getline(ss, tmp, ' ');
			std::getline(ss, tmp, ' ');

			std::string val;
			val = tmp;

			polyRules[key] = val;
		}
	}
	file.close(); 
	
	// Letter Count mapping
	std::map<char, long long> letters;
	for (int i = 0; i < polymer.length(); i++)
	{
		if (letters.find(polymer[i]) == letters.end())
			letters[polymer[i]] = 1;
		else
			letters[polymer[i]]++;
	}

	// Pair count mapping
	std::map<std::string, long long> pairCount;
	for (int i = 0; i < polymer.length() - 1; i++)
	{
		std::string polyPair = polymer.substr(i, 2);
		if (pairCount.find(polyPair) == pairCount.end())
			pairCount[polyPair] = 1;
		else
			pairCount[polyPair]++;
	}

	// Apply insertion rules
	for (int i = 0; i < NUM_STEPS; i++)
	{
		std::map<std::string, long long>::iterator iter;

		// New empty map containing next steps pairs
		std::map<std::string, long long> updatedPairs;

		for (iter = pairCount.begin(); iter != pairCount.end(); iter++)
		{
			// Inserted Letter
			std::string insertedStr = polyRules[iter->first];
			
			// Update letter count
			if (letters.find(insertedStr[0]) == letters.end())
				letters[insertedStr[0]] = iter->second;
			else
				letters[insertedStr[0]] += iter->second;

			// First new pair
			std::string polyPair1 = iter->first[0] + insertedStr;

			// Add first pair to updated pairs mapping
			if (updatedPairs.find(polyPair1) == updatedPairs.end())
				updatedPairs[polyPair1] = iter->second;
			else
				updatedPairs[polyPair1] += iter->second;

			// Second new pair
			std::string polyPair2 = insertedStr + iter->first[1];

			// Add second pair to updated pairs mapping
			if (updatedPairs.find(polyPair2) == updatedPairs.end())
				updatedPairs[polyPair2] = iter->second;
			else
				updatedPairs[polyPair2] += iter->second;

		}

		// Update Pair Count
		pairCount = updatedPairs;
	}

	// Find largest and smallest
	long long largest = -1;
	long long smallest = -1;
	std::map<char, long long>::iterator iter;
	for (iter = letters.begin(); iter != letters.end(); iter++)
	{
		if (smallest < 0)
			smallest = iter->second;

		if (iter->second > largest)
			largest = iter->second;

		if (iter->second < smallest)
			smallest = iter->second;
	}

	printf("Largest: %llu\n", largest);
	printf("Smallest: %llu\n", smallest);
	printf("Difference: %llu\n", largest - smallest);


	return 0;
}


