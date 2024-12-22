#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef std::vector<std::string> StringList;
typedef std::vector<int> IntList;
typedef std::vector<IntList> IntMatrix;
typedef std::vector<bool> BoolList;
typedef std::vector<BoolList> BoolMatrix;
typedef long long unsigned BigUInt;

struct Position
{
	int row;
	int col;

	bool operator==(const Position& oth) const
	{
		return row == oth.row && col == oth.col;
	}
};

namespace std
{
	template <>
	struct hash<Position>
	{
		std::size_t operator()(const Position& pos) const
		{
			return std::hash<int>()(pos.row) ^ (std::hash<int>()(pos.col) << 1);
		}
	};
}

typedef std::unordered_set<Position> PositionSet;

bool readInput(IntMatrix& map)
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
		if (!map.empty() && line.length() != map[0].size())
		{
			LOG_ERROR("Invalid map containing lines with different lengths.");
		}

		map.emplace_back(line.size(), -1);
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] < '0' || line[i] > '9')
			{
				LOG_ERROR("Invalid line in map - contains non-digit characters.");
			}
			map.back()[i] = line[i] - '0';
		}
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}
	return true;
}

void printMap(const IntMatrix& map)
{
	for (const IntList& row : map)
	{
		for (int tile : row)
		{
			std::cout << tile;
		}
		std::cout << std::endl;
	}
}

void getTrailheadEndings(const IntMatrix& map, BoolMatrix& usedMap, int row, int col, PositionSet& endings)
{
	if (map[row][col] == 9)
	{
		endings.insert({ row, col });
	}

	usedMap[row][col] = true;

	if (row > 0 && !usedMap[row - 1][col] && map[row - 1][col] == map[row][col] + 1)
	{
		getTrailheadEndings(map, usedMap, row - 1, col, endings);
	}
	if (row + 1 < map.size() && !usedMap[row + 1][col] && map[row + 1][col] == map[row][col] + 1)
	{
		getTrailheadEndings(map, usedMap, row + 1, col, endings);
	}
	if (col > 0 && !usedMap[row][col - 1] && map[row][col - 1] == map[row][col] + 1)
	{
		getTrailheadEndings(map, usedMap, row, col - 1, endings);
	}
	if (col + 1 < map[0].size() && !usedMap[row][col + 1] && map[row][col + 1] == map[row][col] + 1)
	{
		getTrailheadEndings(map, usedMap, row, col + 1, endings);
	}

	usedMap[row][col] = false;
}

void getTrailheadRating(const IntMatrix& map, BoolMatrix& usedMap, int row, int col, BigUInt& rating)
{
	if (map[row][col] == 9)
	{
		rating++;
	}

	usedMap[row][col] = true;

	if (row > 0 && !usedMap[row - 1][col] && map[row - 1][col] == map[row][col] + 1)
	{
		getTrailheadRating(map, usedMap, row - 1, col, rating);
	}
	if (row + 1 < map.size() && !usedMap[row + 1][col] && map[row + 1][col] == map[row][col] + 1)
	{
		getTrailheadRating(map, usedMap, row + 1, col, rating);
	}
	if (col > 0 && !usedMap[row][col - 1] && map[row][col - 1] == map[row][col] + 1)
	{
		getTrailheadRating(map, usedMap, row, col - 1, rating);
	}
	if (col + 1 < map[0].size() && !usedMap[row][col + 1] && map[row][col + 1] == map[row][col] + 1)
	{
		getTrailheadRating(map, usedMap, row, col + 1, rating);
	}

	usedMap[row][col] = false;
}

void solve01(const IntMatrix& map)
{
	if (map.empty() || map[0].empty())
	{
		LOG_ERROR("Map is empty.");
		return;
	}

	BoolMatrix usedMap(map.size(), BoolList(map[0].size(), false));

	int scoreSum = 0;
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[row].size(); col++)
		{
			// If tile is a trailhead
			if (map[row][col] == 0)
			{
				PositionSet endings;
				getTrailheadEndings(map, usedMap, row, col, endings);
				scoreSum += int(endings.size());
			}
		}
	}
	std::cout << scoreSum << std::endl;
}

void solve02(const IntMatrix& map)
{
	if (map.empty() || map[0].empty())
	{
		LOG_ERROR("Map is empty.");
		return;
	}

	BoolMatrix usedMap(map.size(), BoolList(map[0].size(), false));

	BigUInt ratingSum = 0;
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[row].size(); col++)
		{
			// If tile is a trailhead
			if (map[row][col] == 0)
			{
				BigUInt rating = 0;
				getTrailheadRating(map, usedMap, row, col, rating);
				ratingSum += rating;
			}
		}
	}
	std::cout << ratingSum << std::endl;
}

int main()
{
	IntMatrix map;
	readInput(map);

	solve01(map);
	solve02(map);
}