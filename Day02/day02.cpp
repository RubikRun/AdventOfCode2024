#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

typedef std::vector<int> List;
typedef std::vector< std::vector<int> > ListOfLists;
#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";

void parseReportsLine(const std::string& line, ListOfLists& reports)
{
	std::istringstream sstream(line);
	reports.emplace_back();
	List& report = reports.back();

	int level;
	while (sstream >> level)
	{
		report.push_back(level);
	}
}

bool readInput(ListOfLists& reports)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		parseReportsLine(line, reports);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

bool isSafe01(const List& report)
{
	if (report.size() < 2)
	{
		return true;
	}

	const int firstDiff = report[0] - report[1];
	for (int i = 0; i < report.size() - 1; i++)
	{
		const int diff = report[i] - report[i + 1];
		const int dist = std::abs(diff);
		if (dist < 1 || dist > 3 || (firstDiff < 0) != (diff < 0))
		{
			return false;
		}
	}

	return true;
}

bool isSafe02(const List& report, int ignoreIdx)
{
	if (report.size() < 3)
	{
		return true;
	}

	int firstDiff = report[0] - report[1];
	if (ignoreIdx == 0)
	{
		firstDiff = report[1] - report[2];
	}
	else if (ignoreIdx == 1)
	{
		firstDiff = report[0] - report[2];
	}

	for (int i = 0; i < report.size() - 1; i++)
	{
		if (i == ignoreIdx)
		{
			continue;
		}

		int diff = report[i] - report[i + 1];
		if (i + 1 == ignoreIdx)
		{
			if (i + 2 >= report.size())
			{
				break;
			}
			diff = report[i] - report[i + 2];
		}
		if ((firstDiff < 0) != (diff < 0))
		{
			return false;
		}

		const int dist = std::abs(diff);
		if (dist < 1 || dist > 3)
		{
			return false;
		}
	}

	return true;
}

bool isSafe02(const List& report)
{
	if (report.size() < 3)
	{
		return true;
	}

	for (int i = 0; i < report.size(); i++)
	{
		if (isSafe02(report, i))
		{
			return true;
		}
	}

	return false;
}

void solve01(const ListOfLists& reports)
{
	int safeCount = 0;
	for (const List& report : reports)
	{
		if (isSafe01(report))
		{
			safeCount++;
		}
	}
	std::cout << safeCount << std::endl;
}

void solve02(const ListOfLists& reports)
{
	int safeCount = 0;
	for (const List& report : reports)
	{
		if (isSafe02(report))
		{
			safeCount++;
		}
	}
	std::cout << safeCount << std::endl;
}

int main()
{
	ListOfLists reports;
	readInput(reports);
	
	solve01(reports);
	solve02(reports);

	return 0;
}