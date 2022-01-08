#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

std::string calcOxygenGen(std::vector<std::string> bitData)
{
	// Got digit by digit
	for(int i = 0; i < bitData.at(0).length(); i++)
	{
		int numZeros = 0;
		int numOnes = 0;

		for (int j = 0; j < bitData.size(); j++)
		{
			if (bitData.at(j).at(i) == '1')
				numOnes++;
			else
				numZeros++;
		}

		// Consider most common digit
		char targetDigit;
		if (numOnes >= numZeros)
			targetDigit = '1';
		else
			targetDigit = '0';

		std::vector<std::string> tmpBuffer;
		for (int j = 0; j < bitData.size(); j++)
		{
			if (bitData.at(j).at(i) == targetDigit)
				tmpBuffer.push_back(bitData.at(j));
		}
		bitData = tmpBuffer;		

		if (bitData.size() < 2)
			return bitData.at(0);
	}

	return bitData[0];
}

std::string calcCO2(std::vector<std::string> bitData)
{
	// Got digit by digit
	for (int i = 0; i < bitData.at(0).length(); i++)
	{
		int numZeros = 0;
		int numOnes = 0;

		for (int j = 0; j < bitData.size(); j++)
		{
			if (bitData.at(j).at(i) == '1')
				numOnes++;
			else
				numZeros++;
		}

		// Consider most common digit
		char targetDigit;
		if (numOnes >= numZeros)
			targetDigit = '0';
		else
			targetDigit = '1';

		std::vector<std::string> tmpBuffer;
		for (int j = 0; j < bitData.size(); j++)
		{
			if (bitData.at(j).at(i) == targetDigit)
				tmpBuffer.push_back(bitData.at(j));
		}
		bitData = tmpBuffer;

		if (bitData.size() < 2)
			return bitData.at(0);
	}

	return bitData[0];
}


int binaryToDec(std::string str)
{
	int result = 0;
	int count = 0;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		if (str.at(i) == '1')
		{
			result += pow(2, count);
		}

		count++;
	}

	return result;
}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::ifstream file("day3.txt");

	// Number of 1's and 0's
	std::vector<int> numOnes;
	std::vector<int> numZeros;

	// Buffer of binary input data
	std::vector<std::string> bitData;

	// Read data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		// Store binary data
		bitData.push_back(str);

		// Initialize digit vector to all zero's based on binary number length if vectors are empty
		if (numOnes.empty() && numZeros.empty())
		{
			for (int i = 0; i < str.length(); i++)
			{
				numOnes.push_back(0);
				numZeros.push_back(0);
			}
		}

		// Consider each binary digit
		for (int i = 0; i < str.length(); i++)
		{
			if (str.at(i) == '0')
			{
				numZeros.at(i)++;
			}
			else if (str.at(i) == '1')
			{
				numOnes.at(i)++;
			}
		}
	}

	// Close data file
	file.close();

	// Gamma and Epsioln binary representations
	std::string gammaStr = "";
	std::string epsilonStr = "";

	for (int i = 0; i < numZeros.size(); i++)
	{
		if (numOnes.at(i) > numZeros.at(i))
		{
			gammaStr += '1';
			epsilonStr += '0';
		}
		else
		{
			gammaStr += '0';
			epsilonStr += '1';
		}
	}

	// Calculate Gamma and Epsilon Rates
	int gammaRate = binaryToDec(gammaStr);
	int epsilonRate = binaryToDec(epsilonStr);
	printf("Gamma Rate: %d Eps Rate: %d Product: %d\n", gammaRate, epsilonRate, gammaRate * epsilonRate);

	// Calculate Oxygen Generation rate and CO2 scrubber rate
	std::string oxygenStr = calcOxygenGen(bitData);
	std::string co2Str = calcCO2(bitData);
	int oxygenRate = binaryToDec(oxygenStr);
	int co2Rate = binaryToDec(co2Str);
	printf("Oxygen Rate: %d CO2 Rate: %d Product: %d\n", oxygenRate, co2Rate, oxygenRate * co2Rate);





	return 0;
}