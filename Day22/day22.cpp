#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

// Struct representing a cube in a 3 dimensional plane
struct Cube
{
	bool active;
	int64_t x1, x2;
	int64_t y1, y2;
	int64_t z1, z2;
	int64_t sign = 1; // Whether the cube is applying positively or negatively to the volume computation algorithm

	Cube()
	{
		active = true;
		x1 = 0;
		x2 = 0;
		y1 = 0;
		y2 = 0;
		z1 = 0;
		z2 = 0;
	}

	Cube(bool active, int64_t xStart, int64_t xEnd, int64_t yStart, int64_t yEnd, int64_t zStart, int64_t zEnd, int64_t sign)
	{
		this->active = active;
		this->sign = sign;
		x1 = xStart;
		x2 = xEnd;
		y1 = yStart;
		y2 = yEnd;
		z1 = zStart;
		z2 = zEnd;
	}
};

// Struct representing an instruction provided by the input file indicatign a cube's bounds as well as 
// whether the cube will be on or off
struct CubeInstruction
{
	bool on;
	std::pair<int, int> xRanges;
	std::pair<int, int> yRanges;
	std::pair<int, int> zRanges;

	CubeInstruction(bool on, std::pair<int, int> xRanges, std::pair<int, int> yRanges, std::pair<int, int> zRanges) : on(on), xRanges(xRanges), yRanges(yRanges), zRanges(zRanges) {};
};

// Extract a cube instruction from a line extracted from the data file
CubeInstruction extractInstruction(std::string line)
{
	
	std::stringstream ss(line);
	std::string str;

	// Get on/off status
	bool on;
	std::getline(ss, str, ' ');
	on = (str == "on") ? true : false;

	std::getline(ss, str, ' ');
	std::stringstream dataStr(str);
	std::string strNum;

	// Range buffer
	std::vector<std::pair<int, int>> ranges;
	while (ranges.size() < 3)
	{
		std::getline(dataStr, strNum, ',');
		strNum.erase(0, 2);

		int i;
		std::string num1 = "";
		for (i = 0; i < strNum.length(); i++)
		{
			if (strNum[i] == '.')
				break;

			num1 += strNum[i];
		}

		// Get to next number
		while (i < strNum.length())
		{
			if (strNum[i] != '.')
				break;

			i++;
		}

		std::string num2 = "";
		for (i; i < strNum.length(); i++)
		{
			num2 += strNum[i];
		}

		int n1 = std::stoi(num1);
		int n2 = std::stoi(num2);

		std::pair<int, int> r;
		if (n1 > n2)
			r = std::pair<int, int>(n2, n1);
		else
			r = std::pair<int, int>(n1, n2);

		ranges.push_back(r);
	}

	CubeInstruction instr(on, ranges[0], ranges[1], ranges[2]);
	return instr;
}

// Calculate the volume of a Cube
int64_t calculateVolume(const Cube& cube)
{
	auto data = std::abs((cube.x2 - cube.x1) + 1) * std::abs((cube.y2 - cube.y1) + 1) * std::abs((cube.z2 - cube.z1) + 1);
	data *= cube.sign;
	return data;
}

// Find the intersection betwween two cubes
Cube calculateIntersection(const Cube& cube1, const Cube& cube2)
{
	auto xStart = std::max(cube1.x1, cube2.x1);
	auto xEnd = std::max(std::min(cube1.x2, cube2.x2), xStart - 1);

	auto yStart = std::max(cube1.y1, cube2.y1);
	auto yEnd = std::max(std::min(cube1.y2, cube2.y2), yStart - 1);

	auto zStart = std::max(cube1.z1, cube2.z1);
	auto zEnd = std::max(std::min(cube1.z2, cube2.z2), zStart - 1);

	auto sign = cube1.sign * cube2.sign * -1;

	Cube result(true, xStart, xEnd, yStart, yEnd, zStart, zEnd, sign);

	return result;
}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day22.txt");

	// Buffer of Cube Instructions
	std::vector<CubeInstruction> instructions;

	// Parse data file line by line
	std::string str;
	while (std::getline(file, str))
	{
		instructions.push_back(extractInstruction(str));
	}
	file.close();

	// Number of "lit up" cubes in the data plane
	int64_t numLit = 0;
	
	// Buffer of cubes in the data plane
	std::vector<Cube> cubes;

	// Process every instruction
	for (int i = 0; i < instructions.size(); i++)
	{
		// Current instruction
		CubeInstruction instr = instructions[i];
		
		// Cube representation of the instruction
		Cube c(instr.on, instr.xRanges.first, instr.xRanges.second, instr.yRanges.first, instr.yRanges.second,
			instr.zRanges.first, instr.zRanges.second, 1);

		// New cubes segments added this turn
		std::vector<Cube> tmpCubes;

		// Handle intersections with other cubes
		// For Cubes A and B
		// Volume (A + B) = Volume(A) + Volume(B) - Volume(AB)
		for (int j = 0; j < cubes.size(); j++)
		{

			Cube intersect = calculateIntersection(cubes[j], c); 
			auto intersectVolume = calculateVolume(intersect);

			if (intersectVolume != 0)
			{
				numLit += intersectVolume;
				tmpCubes.push_back(intersect);
			}

		}


		if (instr.on)
		{
			numLit += calculateVolume(c);
			tmpCubes.push_back(c);

		}

		// Append to cube list
		cubes.insert(cubes.end(), tmpCubes.begin(), tmpCubes.end());
	}


	std::cout << "Number of Active Cubes" << std::endl;
	std::cout << numLit << std::endl;


	return 0;
}