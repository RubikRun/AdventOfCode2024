#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef std::vector<std::string> ListOfString;

bool readInput(ListOfString& lines)
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
		lines.push_back(line);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

int countXmasHorizontal(const ListOfString& lines)
{
	if (lines.empty())
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = 0; j + 4 <= lines[i].length(); j++)
		{
			const std::string sub = lines[i].substr(j, 4);
			if (sub == "XMAS" || sub == "SAMX")
			{
				count++;
			}
		}
	}
	return count;
}

int countXmasVertical(const ListOfString& lines)
{
	if (lines.empty())
	{
		return 0;
	}

	int count = 0;
	for (int j = 0; j < lines[0].length(); j++)
	{
		for (int i = 0; i + 4 <= lines.size(); i++)
		{
			const std::string sub = std::string(1, lines[i][j]) + std::string(1, lines[i + 1][j])
			                        + std::string(1, lines[i + 2][j]) + std::string(1, lines[i + 3][j]);
			if (sub == "XMAS" || sub == "SAMX")
			{
				count++;
			}
		}
	}
	return count;
}

int countXmasDiagonal(const ListOfString& lines)
{
	if (lines.empty())
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i + 4 <= lines.size(); i++)
	{
		for (int j = 0; j + 4 <= lines[i].length(); j++)
		{
			const std::string sub = std::string(1, lines[i][j]) + std::string(1, lines[i + 1][j + 1])
				+ std::string(1, lines[i + 2][j + 2]) + std::string(1, lines[i + 3][j + 3]);
			if (sub == "XMAS" || sub == "SAMX")
			{
				count++;
			}
		}
	}
	for (int i = 3; i < lines.size(); i++)
	{
		for (int j = 0; j + 4 <= lines[i].length(); j++)
		{
			const std::string sub = std::string(1, lines[i][j]) + std::string(1, lines[i - 1][j + 1])
				+ std::string(1, lines[i - 2][j + 2]) + std::string(1, lines[i - 3][j + 3]);
			if (sub == "XMAS" || sub == "SAMX")
			{
				count++;
			}
		}
	}
	return count;
}

void solve01(const ListOfString& lines)
{
	const int count = countXmasHorizontal(lines) + countXmasVertical(lines) + countXmasDiagonal(lines);
	std::cout << count << std::endl;
}

void solve02(const ListOfString& lines)
{
	if (lines.empty())
	{
		std::cout << 0 << std::endl;
		return;
	}

	int count = 0;
	for (int i = 0; i + 3 <= lines.size(); i++)
	{
		for (int j = 0; j + 3 <= lines[i].length(); j++)
		{
			const std::string aSub = std::string(1, lines[i][j]) + std::string(1, lines[i + 1][j + 1]) + std::string(1, lines[i + 2][j + 2]);
			const std::string bSub = std::string(1, lines[i + 2][j]) + std::string(1, lines[i + 1][j + 1]) + std::string(1, lines[i][j + 2]);
			if ((aSub == "MAS" || aSub == "SAM") && (bSub == "MAS" || bSub == "SAM"))
			{
				count++;
			}
		}
	}
	std::cout << count << std::endl;
}

int main()
{
	ListOfString lines;
	readInput(lines);

	solve01(lines);
	solve02(lines);
}