#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

std::string hexToBinary(char c)
{
	switch (c)
	{
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
	}
}

long long binaryToDec(std::string bin)
{
    long long val = 0;
    long long b = 1;

    for (int i = bin.length() - 1; i >= 0; i--)
    {
        if (bin[i] == '1')
            val += b;

        b *= 2;
    }

    return val;
}

long long packetOperations(std::vector<long long> packetVals, int TID)
{
    switch (TID)
    {
        case 0:
        {
            long long sum = 0;
            for (auto i : packetVals)
                sum += i;

            return sum;
        }
        break;

        case 1:
        {
            long long sol = 1;
            for (auto i : packetVals)
                sol *= i;

            return sol;
        }
        break;

        case 2:
        {
            long long min = packetVals[0];
            for (int i = 1; i < packetVals.size(); i++)
                if (packetVals[i] < min)
                    min = packetVals[i];

            return min;
        }
        break;

        case 3:
        {
            long long max = packetVals[0];
            for (int i = 1; i < packetVals.size(); i++)
                if (packetVals[i] > max)
                    max = packetVals[i];

            return max;
        }
        break;

        case 5:
        {
            return (packetVals[0] > packetVals[1]);
        }
        break;

        case 6:
        {
            return (packetVals[0] < packetVals[1]);
        }
        break;

        case 7:
        {
            return (packetVals[0] == packetVals[1]);
        }
        break;
    }

    return 0;
}

std::pair<long long, int> packetParser(std::string packet, int index, int end)
{
    if (index == end || index > (packet.length() - 4))
        return std::pair<long long, int>(-1, -1);

    long long version = binaryToDec(packet.substr(index, 3));
    long long TID = binaryToDec(packet.substr(index + 3, 3));

    // Literal Packet
    if (TID == 4)
    {
        index += 6;
        std::string dataStr = "";
        bool finished = false;
        while (!finished)
        {
            if (packet[index] == '0')
                finished = true;

            dataStr += packet.substr(index + 1, 4);
            index += 5;
        }

        long long dataVal = binaryToDec(dataStr);
    
        return std::pair<long long, int>(dataVal, index);
    }

    // Operator Packet
    else
    {
        std::vector<long long> subPackValues;
        int nextIndex = -1;

        char LID = packet[index + 6];
        if (LID == '0')
        {
            long long numBits = binaryToDec(packet.substr(index + 7, 15));
            int ending = index + 22 + numBits;
            int i = index + 22;
            int prev = -1;

            while (i != -1)
            {
                prev = i;
                std::pair<long long, int> pkt = packetParser(packet, i, ending);
                subPackValues.push_back(pkt.first);
                i = pkt.second;
            }

            subPackValues.pop_back();
            nextIndex = prev;
        }
        else
        {
            long long numPacks = binaryToDec(packet.substr(index + 7, 11));
            int i = index + 18;
            while (numPacks > 0)
            {
                std::pair<long long, int> pkt = packetParser(packet, i, -1);
                numPacks--;
                subPackValues.push_back(pkt.first);
                i = pkt.second;
            }

            nextIndex = i;
        }

        // Perform operations
        return std::pair<long long, int>(packetOperations(subPackValues, TID), nextIndex);


    }
}

int main(int argc, char* argv[])
{
	// Extract data from data file
	std::fstream file("day16.txt");

	// Get line of data from data file
	std::string hexData;
	std::getline(file, hexData);
	file.close();

	// Convert hex data to binary
	std::string binData = "";
    for (int i = 0; i < hexData.length(); i++)
        binData += hexToBinary(hexData[i]);

    std::pair<long long,int> sol = packetParser(binData, 0, -1);
    printf("Solution: %llu\n", sol.first);


	return 0;
}

