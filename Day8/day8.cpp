#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <unordered_map>

std::string strDiff(std::string a, std::string b)
{
	std::set<char> letters;
	for (int i = 0; i < a.length(); i++)
		letters.insert(a[i]);
	
	for (int i = 0; i < b.length(); i++)
		letters.erase(b[i]);

	std::string result = "";
	std::set<char>::iterator iter;
	for (iter = letters.begin(); iter != letters.end(); iter++)
		result += *iter;

	return result;
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day8.txt");

	// Data buffers
	std::vector<std::vector<std::string>> inputData;
	std::vector<std::vector<std::string>> outputData;

	long long totalValue = 0;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		bool delimSeen = false;
		std::vector<std::string> inputRow;
		std::vector<std::string> outputRow;

		// Parse each line by blank space delimiter
		std::stringstream ss(str);
		std::string spaceStr;
		while (std::getline(ss, spaceStr, ' '))
		{
			if (spaceStr == "|")
			{
				delimSeen = true;
				continue;
			}

			// Grab Inputs
			if (!delimSeen)
			{
				inputRow.push_back(spaceStr);
			}
			// Grab Outputs
			else
			{
				outputRow.push_back(spaceStr);
			}
		}

		inputData.push_back(inputRow);
		outputData.push_back(outputRow);
	}
	file.close();

	// Handle each row of data
	for (int i = 0; i < inputData.size(); i++)
	{
		std::vector<std::string> dataMapping(10, "");
		std::vector<std::string> unknownMapping;

		std::vector<std::string> sourceData = inputData[i];
		for (int j = 0; j < sourceData.size(); j++)
		{
			std::string str = sourceData[j];
			std::sort(str.begin(), str.end());
			if (str.length() == 2)
				dataMapping[1] = str;
			else if (str.length() == 4)
				dataMapping[4] = str;
			else if (str.length() == 3)
				dataMapping[7] = str;
			else if (str.length() == 7)
				dataMapping[8] = str;
			else
				unknownMapping.push_back(str);
			
		}

		// Find mapping for number 9
		for (int j = 0; j < unknownMapping.size(); j++)
		{
			std::string transform1 = strDiff(unknownMapping.at(j), dataMapping[4]);
			std::string transform2 = strDiff(transform1, dataMapping[7]);

			if (transform2.length() == 1 && unknownMapping[j].length() == 6)
			{
				dataMapping[9] = unknownMapping[j];
				unknownMapping.erase(unknownMapping.begin() + j);
				break;				
			}
		}

		// Find mapping for number 0
		for (int j = 0; j < unknownMapping.size(); j++)
		{
			std::string transform1 = strDiff(unknownMapping[j], dataMapping[1]);
			if (transform1.length() == 4 && unknownMapping[j].length() == 6)
			{
				dataMapping[0] = unknownMapping[j];
				unknownMapping.erase(unknownMapping.begin() + j);
				break;
			}
		}

		// Find mapping for number 3
		for (int j = 0; j < unknownMapping.size(); j++)
		{
			std::string transform1 = strDiff(unknownMapping[j], dataMapping[7]);
			if (transform1.length() == 2)
			{
				dataMapping[3] = unknownMapping[j];
				unknownMapping.erase(unknownMapping.begin() + j);
				break;
			}
		}

		// Find mapping for number 5
		for (int j = 0; j < unknownMapping.size(); j++)
		{
			std::string transform1 = strDiff(dataMapping[9], unknownMapping[j]);
			if (transform1.length() == 1 && unknownMapping[j].length() == 5)
			{
				dataMapping[5] = unknownMapping[j];
				unknownMapping.erase(unknownMapping.begin() + j);
				break;
			}
		}

		// Find mapping for number 6
		for (int j = 0; j < unknownMapping.size(); j++)
		{
			std::string transform1 = strDiff(unknownMapping[j], dataMapping[5]);
			if (transform1.length() == 1)
			{
				dataMapping[6] = unknownMapping[j];
				unknownMapping.erase(unknownMapping.begin() + j);
				break;
			}
		}

		// Find mapping for number 2
		dataMapping[2] = unknownMapping[0];


		std::vector<std::string> codeData = outputData[i];
		std::vector<int> outValBuffer;
		for (int j = 0; j < codeData.size(); j++)
		{
			std::string tmp = codeData[j];
			std::sort(tmp.begin(), tmp.end());

			for (int k = 0; k < 10; k++)
			{
				if (dataMapping[k] == tmp)
					outValBuffer.push_back(k);
			}
		}

		// Convert val buffer to output value		
		int outputValue = 0;
		int count = 0;
		for (int j = outValBuffer.size() - 1; j >= 0; j--)
		{
			outputValue += outValBuffer[j] * std::pow(10, count);
			count++;
		}

		totalValue += outputValue;
	}

	printf("Total Value: %llu\n", totalValue);

	return 0;
}