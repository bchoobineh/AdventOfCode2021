#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>


#define NUM_PASS 50

// So I don't have to type it out every time
using Image = std::vector<std::string>;

// Background of the image
char imageBackground = '.';

// For Debugging
void printImage(std::vector<std::string> image)
{
	printf("\nPrinting Image:\n");
	printf("---------------\n");
	for (int i = 0; i < image.size(); i++)
	{
		for (int j = 0; j < image[i].length(); j++)
		{
			printf("%c", image[i].at(j));
		}
		printf("\n");
	}
	printf("\n");
}

// To account for infinite background affecting image
void padImage(Image* image)
{
	// Add top level of padding
	image->insert(image->begin(), std::string(image->at(0).length(), imageBackground));

	// Add bot level of padding
	image->push_back(std::string(image->at(0).length(), imageBackground));

	// Add Left padding
	for (int i = 0; i < image->size(); i++)
		image->at(i).insert(image->at(i).begin(), imageBackground);

	// Add Right padding
	for (int i = 0; i < image->size(); i++)
		image->at(i).push_back(imageBackground);

}

std::string pixelsToBinary(std::string pixels)
{
	std::string result(pixels);
	for (int i = 0; i < result.length(); i++)
	{
		if (result[i] == '.')
			result[i] = '0';
		else
			result[i] = '1';
	}

	return result;
}

int binaryToDec(std::string binary)
{
	int val = 0;
	int b = 1;

	for (int i = binary.length() - 1; i >= 0; i--)
	{
		if (binary[i] == '1')
			val += b;

		b *= 2;
	}

	return val;
}

int findLitPixels(std::vector<std::string> image)
{
	int count = 0;
	for (int i = 0; i < image.size(); i++)
	{
		for (int j = 0; j < image[i].length(); j++)
		{
			if (image[i][j] == '#')
				count++;
		}
	}

	return count;
}

void imageEnhancement(Image* image, std::string imageEnhance)
{
	// Make copy of image
	Image out = *image;

	// Iterate through every point in the image
	for (int i = 0; i < image->size(); i++)
	{
		std::string row = image->at(i);
		for (int j = 0; j < row.length(); j++)
		{
			// Find all 8 possible neighboring pixels
			std::string pixelGroup = "";

			for (int x = i - 1; x <= i + 1; x++)
			{
				for (int y = j - 1; y <= j + 1; y++)
				{
					// If out of bounds get the background element
					if (x < 0 || y < 0 || x >= image->size() || y >= image->at(x).size())
						pixelGroup += imageBackground;
					else
						pixelGroup += image->at(x).at(y);
				}
			}

			std::string binary = pixelsToBinary(pixelGroup);
			out[i][j] = imageEnhance.at(binaryToDec(binary));
		}
	}
	
	// Set output image
	*image = out;

	// Change background since it flickers
	if (imageBackground == '#')
		imageBackground = '.';
	else
		imageBackground = '#';

}


int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day20.txt");

	// Parse out image enhancement algorithm
	std::string imageEnhance;
	std::getline(file, imageEnhance);
	
	// Skip empty line
	std::string str;
	std::getline(file, str);

	// Get input image
	Image image;
	while (std::getline(file, str))
	{
		image.push_back(str);
	}
	file.close();

	// Perform NUM_PASS amounts of image enhancments on the input
	for (int i = 0; i < NUM_PASS; i++)
	{
		padImage(&image);
		imageEnhancement(&image, imageEnhance);
	}

	printf("Number of Lights: %d\n", findLitPixels(image));


	return 0;
}