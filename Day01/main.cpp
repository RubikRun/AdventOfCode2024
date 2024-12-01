#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

typedef std::vector<int> List;
#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";

bool readInput(List& leftList, List& rightList)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	int leftElement, rightElement;
	while (file >> leftElement >> rightElement)
	{
		leftList.push_back(leftElement);
		rightList.push_back(rightElement);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	if (leftList.size() != rightList.size())
	{
		LOG_ERROR("Left list and right list are not the same size.");
		return false;
	}

	return true;
}

// Solves first part of puzzle.
// Expects that given lists are sorted.
void solve01(const List& leftList, const List& rightList)
{
	int distSum = 0;
	for (int i = 0; i < leftList.size(); i++)
	{
		distSum += std::abs(leftList[i] - rightList[i]);
	}

	std::cout << distSum << std::endl;
}

// Solves second part of puzzle.
// Expects that given lists are sorted.
void solve02(const List& leftList, const List& rightList)
{
	int lp = 0;
	int rp = 0;

	int simil = 0;
	while (lp < leftList.size())
	{
		// Find next right element that's not smaller than the left element
		while (rp < rightList.size() && leftList[lp] > rightList[rp])
		{
			rp++;
		}
		if (rp >= rightList.size())
		{
			break;
		}

		// Find next right element that's different than the left element
		int rFirst = rp;
		while (rp < rightList.size() && leftList[lp] == rightList[rp])
		{
			rp++;
		}

		simil += (rp - rFirst) * leftList[lp];
		if (rp >= rightList.size())
		{
			break;
		}

		lp++;
	}

	std::cout << simil << std::endl;
}

int main()
{
	List leftList;
	List rightList;
	if (!readInput(leftList, rightList))
	{
		return 0;
	}

	std::sort(leftList.begin(), leftList.end());
	std::sort(rightList.begin(), rightList.end());

	solve01(leftList, rightList);
	solve02(leftList, rightList);
}