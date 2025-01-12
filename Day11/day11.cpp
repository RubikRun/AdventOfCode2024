#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input01.txt";
typedef std::vector<int> IntList;

bool readInput(IntList& stones)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	std::string stoneStr;
	while (file >> stoneStr)
	{
		try
		{
			const int stone = std::stoi(stoneStr);
			stones.push_back(stone);
		}
		catch (...)
		{
			LOG_ERROR("Invalid non-integer stone");
		}
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

void printStones(const IntList& stones)
{
	if (stones.empty())
	{
		return;
	}
	std::cout << stones[0];
	for (int i = 1; i < int(stones.size()); i++)
	{
		std::cout << ", " << stones[i];
	}
	std::cout << std::endl;
}

// Fills digits list with digits of given number.
void getDigits(int number, IntList& digits)
{
	if (number == 0)
	{
		digits.push_back(0);
		return;
	}

	while (number != 0)
	{
		digits.push_back(number % 10);
		number /= 10;
	}

	std::reverse(digits.begin(), digits.end());
}

int getNumberFromDigits(const IntList& digits, int beginIdx, int endIdx)
{
	int number = 0;
	for (int i = beginIdx; i < endIdx; i++)
	{
		number *= 10;
		number += digits[i];
	}
	return number;
}

void blink(IntList& stones)
{
	IntList ogStones = stones;
	stones.clear();

	for (int i = 0; i < ogStones.size(); i++)
	{
		if (ogStones[i] == 0)
		{
			stones.push_back(1);
			continue;
		}
		IntList digits;
		getDigits(ogStones[i], digits);
		const int digitsSize = int(digits.size());
		if (digitsSize % 2 == 0)
		{
			const int leftNumber = getNumberFromDigits(digits, 0, digitsSize % 2);
			const int rightNumber = getNumberFromDigits(digits, digitsSize % 2, digitsSize);
			// TODO
		}
	}
}

int main()
{
	IntList stones;
	readInput(stones);


}