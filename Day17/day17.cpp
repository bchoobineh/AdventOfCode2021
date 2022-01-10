#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct Rectangle
{
	std::pair<int, int> topLeft;
	int width;
	int height;
};

void parseDataStr(std::string str, int* xStart, int* xEnd, int* yStart, int* yEnd)
{
	// Iterate until first '='
	int index;
	for (index = 0; index < str.length(); index++)
		if (str[index] == '=')
			break;
		
	// Grab Starting X
	index++;
	std::string xStr = "";
	while (index < str.length())
	{
		if (str[index] == '.')
			break;

		xStr += str[index];
		index++;
	}
	*xStart = std::stoi(xStr);


	// Grab Ending X
	index += 2;
	xStr = "";
	while (index < str.length())
	{
		if (str[index] == ',')
			break;

		xStr += str[index];
		index++;
	}
	*xEnd = std::stoi(xStr);


	// Iterate until second '='
	while (index < str.length())
	{
		if (str[index] == '=')
			break;
		index++;
	}

	// Grab Y Start
	index++;
	std::string yStr = "";
	while (index < str.length())
	{
		if (str[index] == '.')
			break;

		yStr += str[index];
		index++;
	}

	*yStart = std::stoi(yStr);



	// Grab Ending Y
	index += 2;
	yStr = "";
	while (index < str.length())
	{
		if (str[index] == ',')
			break;

		yStr += str[index];
		index++;
	}
	*yEnd = std::stoi(yStr);

}

bool pointWithinRectangle(std::pair<int, int> point, Rectangle rect)
{
	int x = point.first;
	int y = point.second;

	if (x >= rect.topLeft.first && x < (rect.topLeft.first + rect.width) && y <= (rect.topLeft.second) && y > (rect.topLeft.second - rect.height))
		return true;

	return false;
}

void simulateVelocity(Rectangle rect, std::pair<int, int> velocity, int* numVel)
{
	// Always start at (0,0)
	std::pair<int, int> position(0, 0);

	// Only end if position is under or past the rectangle
	while (position.first < (rect.topLeft.first + rect.width) && position.second > (rect.topLeft.second - rect.height))
	{
		// Update Position
		position.first += velocity.first;
		position.second += velocity.second;

		// Update Velocity
		if (velocity.first > 0)
			velocity.first--;
		else if (velocity.first < 0)
			velocity.first++;

		// Gravity
		velocity.second--;

		// Check if we hit the square
		if (pointWithinRectangle(position, rect))
		{			
			*numVel = *numVel + 1;
			return;
		}

	}

}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day17.txt");

	// Get data file line
	std::string str;
	std::getline(file, str);
	file.close();

	// X and Y ranges
	int xStart = 0;
	int xEnd = 0;
	int yStart = 0;
	int yEnd = 0;

	// Parse X and Y ranges
	parseDataStr(str, &xStart, &xEnd, &yStart, &yEnd);

	// Probe Origin (0,0)
	std::pair<int, int> origin(0, 0);


	std::pair<int, int> yVelRange(yStart, std::abs(yStart)-1);
	
	std::pair<int, int> xVelRange;	
	// Calculate minimum x velocity needed
	int n = 0;
	int index = 0;
	for (int i = 0; i <= xEnd; i++)
	{
		n += i;
		if (n >= xStart)
		{
			index = i;
			break;
		}
	}
	xVelRange.first = index;
	xVelRange.second = xEnd;

	// Create rectangle representing target area
	Rectangle rec;
	rec.topLeft = std::pair<int, int>(xStart, yEnd);
	rec.width = xEnd - xStart + 1;
	rec.height = std::abs(yEnd - yStart) + 1;


	// Simulate all possibilities
	int numVel = 0;
	for (int i = yVelRange.first; i <= yVelRange.second; i++)
	{
		for (int j = xVelRange.first; j <= xVelRange.second; j++)
		{

			std::pair<int, int> velocity(j, i);
			simulateVelocity(rec, velocity, &numVel);			
		}
	}

	// Number of Velocities
	printf("Number of Velocities: %d\n", numVel);



	return 0;
}