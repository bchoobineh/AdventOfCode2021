#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <queue>

// For finding how long this mess takes
#include <chrono>

// Struct representing a Point (x,y,z) in 3 dimensions
struct Point
{
	int x, y, z;

	// Point constructor
	Point(int x, int y, int z) :
		x(x),
		y(y),
		z(z)
	{}

	// Print the point
	void printPoint()
	{
		printf("(%d,%d,%d)\n", x, y, z);
	}
	
	// Point equality op
	bool operator == (const Point& pt) const
	{
		return (x == pt.x && y == pt.y && z == pt.z);
	}

	// Point addition op	
	Point operator + (const Point& pt) const
	{
		return Point(x + pt.x, y + pt.y, z + pt.z);
	}

	// Point subtraction op
	Point operator - (const Point& pt) const
	{
		return Point(x - pt.x, y - pt.y, z - pt.z);
	}

	// Point less than op
	// Go by z, y, and x in order
	bool operator < (const Point& pt) const
	{
		return
			(z < pt.z) ||
			(z == pt.z && y < pt.y) ||
			(z == pt.z && y == pt.y && x < pt.x);
	}

};

// Struct for hashing the 3 dimensional point struct
struct pointHash
{
	std::size_t operator () (const Point& pt) const
	{
		return ( (pt.z * 3000000) + (pt.y * 5000) + (pt.x)); // Random hash numbs
	}
};

// Struct representing a Scanner
struct Scanner
{
	Point location = Point(0, 0, 0); // Location of Scanner initialized to zero as initially unknown
	int ID = -1; // Scanner number

	// Input data recieved from this scanner about nearby beacons
	std::vector<Point> inputs;

	// Vector distances between beacons for each type of 3D orientation transform
	std::vector<std::unordered_set<Point, pointHash>> beaconDeltas;

	std::vector<Point> originalDeltas;

	// Orientation scanner is facing
	int orientation = -1;
};


using PT = Point(const Point&); // Point Transform shorthand

// Reference to: https://imgur.com/Ff1vGT9
const auto pointTransforms = std::vector<PT*>{
  [](const Point& p) { return Point(p.x, p.y, p.z); },
  [](const Point& p) { return Point(p.x, -p.z, p.y); },
  [](const Point& p) { return Point(p.x, -p.y, -p.z); },
  [](const Point& p) { return Point(p.x, p.z, -p.y); },
  [](const Point& p) { return Point(-p.y, p.x, p.z); },
  [](const Point& p) { return Point(p.z, p.x, p.y); },
  [](const Point& p) { return Point(p.y, p.x, -p.z); },
  [](const Point& p) { return Point(-p.z, p.x, -p.y); },
  [](const Point& p) { return Point(-p.x, -p.y, p.z); },
  [](const Point& p) { return Point(-p.x, -p.z, -p.y); },
  [](const Point& p) { return Point(-p.x, p.y, -p.z); },
  [](const Point& p) { return Point(-p.x, p.z, p.y); },
  [](const Point& p) { return Point(p.y, -p.x, p.z); },
  [](const Point& p) { return Point(p.z, -p.x, -p.y); },
  [](const Point& p) { return Point(-p.y, -p.x, -p.z); },
  [](const Point& p) { return Point(-p.z, -p.x, p.y); },
  [](const Point& p) { return Point(-p.z, p.y, p.x); },
  [](const Point& p) { return Point(p.y, p.z, p.x); },
  [](const Point& p) { return Point(p.z, -p.y, p.x); },
  [](const Point& p) { return Point(-p.y,-p.z, p.x); },
  [](const Point& p) { return Point(-p.z, -p.y, -p.x); },
  [](const Point& p) { return Point(-p.y, p.z, -p.x); },
  [](const Point& p) { return Point(p.z, p.y, -p.x); },
  [](const Point& p) { return Point(p.y, -p.z, -p.x); }
};

const auto pointInverseTransforms = std::vector<PT*>{
  [](const Point& p) { return Point(p.x, p.y, p.z); },//1
  [](const Point& p) { return Point(p.x, p.z, -p.y); },//2
  [](const Point& p) { return Point(p.x, -p.y, -p.z); },//3
  [](const Point& p) { return Point(p.x, -p.z, p.y); },//4
  [](const Point& p) { return Point(p.y, -p.x, p.z); },//5
  [](const Point& p) { return Point(p.y, p.z, p.x); },//6
  [](const Point& p) { return Point(p.y, p.x, -p.z); },//7
  [](const Point& p) { return Point(p.y, -p.z, -p.x); },//8
  [](const Point& p) { return Point(-p.x, -p.y, p.z); },//9
  [](const Point& p) { return Point(-p.x, -p.z, -p.y); },//10
  [](const Point& p) { return Point(-p.x, p.y, -p.z); },//11
  [](const Point& p) { return Point(-p.x, p.z, p.y); },//12
  [](const Point& p) { return Point(-p.y, p.x, p.z); },//13
  [](const Point& p) { return Point(-p.y,-p.z, p.x); },//14
  [](const Point& p) { return Point(-p.y, -p.x, -p.z); },//15
  [](const Point& p) { return Point(-p.y, p.z, -p.x); },//16
  [](const Point& p) { return Point(p.z, p.y, -p.x); },//17
  [](const Point& p) { return Point(p.z, p.x, p.y); },//18
  [](const Point& p) { return Point(p.z, -p.y, p.x); },//19
  [](const Point& p) { return Point(p.z, -p.x, -p.y); },//20
  [](const Point& p) { return Point(-p.z, -p.y, -p.x); },//21
  [](const Point& p) { return Point(-p.z, -p.x, p.y); },//22
  [](const Point& p) { return Point(-p.z, p.y, p.x); },//23
  [](const Point& p) { return Point(-p.z, p.x, -p.y); }// 24
};

Point convertOriginalOrientation(Scanner& scanner, const Point& pt)
{
	// Do reverse transform to get original
	auto originalVector = pointInverseTransforms[scanner.orientation](pt);
		
	// Search for original point in original inputs that maches the input
	bool foundPoint = false;
	int index = 0;
	for (int i = 0; i < scanner.inputs.size(); i++)
	{
		for (int j = i + 1; j < scanner.inputs.size(); j++)
		{
			const auto deltaPoint = Point(
				scanner.inputs[i].x - scanner.inputs[j].x,
				scanner.inputs[i].y - scanner.inputs[j].y,
				scanner.inputs[i].z - scanner.inputs[j].z
			);
			
			if (deltaPoint == originalVector)
			{
				foundPoint = true;
				index = i;
				break;
			}

			if (foundPoint)
				break;
		}
	}

	Point result = pointTransforms[scanner.orientation](scanner.inputs[index]);
	return result;
}

Point findScannerCoord(Scanner scan1, Scanner scan2, const Point& commonPoint)
{
	Point p1 = convertOriginalOrientation(scan1, commonPoint);
	Point p2 = convertOriginalOrientation(scan2, commonPoint);

	return (scan1.location + p1 - p2);
}


int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();

	// Extract data from data file
	std::fstream file("day19.txt");

	// Buffer of scanners we have in our system
	std::vector<Scanner> scanners;

	// Parse data file line by line
	std::string str;
	int idCount = 0;
	while (std::getline(file, str))
	{
		Scanner sc;
		sc.ID = idCount;

		// Parse data points scanner records
		while (std::getline(file, str))
		{
			// Stop reading if we have an empty line
			if (str.length() < 1)
				break;

			// Delimit string by ','
			std::stringstream ss(str);
			std::string tmp;
			
			std::getline(ss, tmp, ',');
			int x = std::stoi(tmp);

			std::getline(ss, tmp, ',');
			int y = std::stoi(tmp);

			std::getline(ss, tmp, ',');
			int z = std::stoi(tmp);

			// Add point to the buffer
			sc.inputs.push_back(Point(x, y, z));
		}

		scanners.push_back(sc);
	}
	file.close();


	// Calculate beacon deltas from each other as well as their deltas
	// with regard to the 24 different 3D space transform orientations the scanner
	// could be oriented in
	for (int scanNum = 0; scanNum < scanners.size(); scanNum++) {

		// Find the original deltas between points detected by the scanner
		for (int i = 0; i < scanners[scanNum].inputs.size(); i++) {
			for (int j = i + 1; j < scanners[scanNum].inputs.size(); j++) {
				scanners[scanNum].originalDeltas.emplace_back(
					scanners[scanNum].inputs[i].x - scanners[scanNum].inputs[j].x,
					scanners[scanNum].inputs[i].y - scanners[scanNum].inputs[j].y,
					scanners[scanNum].inputs[i].z - scanners[scanNum].inputs[j].z);
			}
		}

		// convert the deltas computed into the 24 different transforms
		for (const auto& transform : pointTransforms) {
			scanners[scanNum].beaconDeltas.emplace_back();
			auto& beacon_at_orinetation_s = scanners[scanNum].beaconDeltas.back();
			for (const auto& p : scanners[scanNum].originalDeltas) {
				const auto temp = transform(p);
				beacon_at_orinetation_s.insert(transform(p));
			}
		}
	}


	// Find 12 ore more overlaps
	int numOverlap = 12;

	// Start looking for overlap starting with scanner 0 set to origin with orientation 0
	scanners[0].orientation = 0;
	std::queue<int> scanQueue;
	std::unordered_set<int> scanSet;
	
	scanQueue.push(0);
	scanSet.insert(0);

	while (!scanQueue.empty())
	{
		// Grab front scanner element from queue
		//int scanNum = scanQueue.front();
		auto& scanner1 = scanners[scanQueue.front()];
		scanQueue.pop();

		for (int i = 0; i < scanners.size(); i++)
		{
			// Don't check against itself
			if (i == scanner1.ID)
				continue;

			// We have already verified this scanner
			if (scanSet.find(i) != scanSet.end())
				continue;

			// Get the comparison scanner
			auto& scanner2 = scanners[i];
			bool gotOrientation = false;

			// Check all possible transformations
			for (int j = 0; j < pointTransforms.size(); j++)
			{
				int overlapCount = 0;
				for (const auto& node : scanner2.beaconDeltas[j])
				{
					// Match!
					if (scanner1.beaconDeltas[scanner1.orientation].find(node) != scanner1.beaconDeltas[scanner1.orientation].end())
						overlapCount++;
					

					// We have found more than 12!
					if (overlapCount >= numOverlap)
					{
						gotOrientation = true;
						scanner2.orientation = j;
						Point scanCoord = findScannerCoord(scanner1, scanner2, node);
						scanner2.location = scanCoord;
						scanQueue.push(i);
						scanSet.insert(i);
						break;
					}
				}
			}
		}
	}


	// Find all beacons
	std::unordered_set<Point, pointHash> beacons;
	for (auto& scanner : scanners)
	{
		const auto transform = pointTransforms[scanner.orientation];
		
		// Calculate beacon locations and insert to set
		for (const auto& pt : scanner.inputs)
		{
			const auto beaconPt = transform(pt) + scanner.location;
			beacons.insert(beaconPt);
		}
	}

	printf("Number of Beacons: %d\n", beacons.size());


	// Find Distance apart from scanners
	int longestDistance = -1;
	for (int i = 0; i < scanners.size(); i++)
	{
		for (int j = i + 1; j < scanners.size(); j++)
		{
			Point ptDistance = scanners[i].location - scanners[j].location;
			if ((std::abs(ptDistance.x) + std::abs(ptDistance.y) + std::abs(ptDistance.z)) > longestDistance)
				longestDistance = (std::abs(ptDistance.x) + std::abs(ptDistance.y) + std::abs(ptDistance.z));
		}
	}

	printf("Distance: %d\n", longestDistance);

	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	printf("%llu\n", duration);

	return 0;
}