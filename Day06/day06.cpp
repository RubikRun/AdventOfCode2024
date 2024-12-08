#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef std::vector<std::string> ListOfString;

struct Guard
{
	Guard(int row, int col) : row(row), col(col), dirRow(-1), dirCol(0) {}
	int row = -1;
	int col = -1;
	int dirRow = 0;
	int dirCol = 0;

	void rotate()
	{
		if ((dirRow == 0) == (dirCol == 0))
		{
			LOG_ERROR("Trying to rotate guard with invalid direction.");
			return;
		}
		if (dirRow > 0)
		{
			dirRow = 0;
			dirCol = -1;
		}
		else if (dirRow < 0)
		{
			dirRow = 0;
			dirCol = 1;
		}
		else if (dirCol > 0)
		{
			dirRow = 1;
			dirCol = 0;
		}
		else if (dirCol < 0)
		{
			dirRow = -1;
			dirCol = 0;
		}
		else
		{
			LOG_ERROR("Trying to rotate guard with invalid direction.");
		}
	}
};

bool readInput(ListOfString& map)
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
		map.push_back(line);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

Guard findGuard(const ListOfString& map)
{
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map.size(); col++)
		{
			if (map[row][col] == '^')
			{
				return Guard(row, col);
			}
		}
	}
	return Guard(-1, -1);
}

bool isInsideMap(int row, int col, const ListOfString& map)
{
	return row >= 0 && row < map.size() && col >= 0 && col < map[row].length();
}

// Moves the guard one step.
// Returns true on success, false if next step would be out of map.
bool moveGuard(Guard& guard, const ListOfString& map)
{
	const int nextRow = guard.row + guard.dirRow;
	const int nextCol = guard.col + guard.dirCol;
	if (!isInsideMap(nextRow, nextCol, map))
	{
		return false;
	}

	if (map[nextRow][nextCol] == '#')
	{
		guard.rotate();
	}
	else
	{
		guard.row = nextRow;
		guard.col = nextCol;
	}

	return true;
}

int countXs(const ListOfString& map)
{
	int count = 0;
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[row].length(); col++)
		{
			if (map[row][col] == 'X')
			{
				count++;
			}
		}
	}
	return count;
}

bool doesLoop(Guard guard, ListOfString map)
{
	if (map.empty())
	{
		return false;
	}
	const int cellsCount = int(map.size() * map[0].size());

	int stepsCount = 0;
	do
	{
		map[guard.row][guard.col] = 'X';
		stepsCount++;
		if (stepsCount > cellsCount)
		{
			return true;
		}
	} while (moveGuard(guard, map));

	return false;
}

void solve01(ListOfString map)
{
	Guard guard = findGuard(map);
	map[guard.row][guard.col] = '.';

	do
	{
		map[guard.row][guard.col] = 'X';
	} while (moveGuard(guard, map));

	std::cout << countXs(map) << std::endl;
}

void solve02(ListOfString map)
{
	const Guard guard = findGuard(map);
	map[guard.row][guard.col] = '.';

	int countLoopCells = 0;
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[row].length(); col++)
		{
			if (map[row][col] == '.')
			{
				map[row][col] = '#';
				if (doesLoop(guard, map))
				{
					countLoopCells++;
				}
				map[row][col] = '.';
			}
		}
	}

	std::cout << countLoopCells << std::endl;
}

int main()
{
	ListOfString map;
	readInput(map);
	
	solve01(map);
	solve02(map);
}