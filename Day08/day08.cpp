#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef std::vector<std::string> StringList;
typedef std::vector<int> IntList;
typedef std::vector<IntList> IntMatrix;

bool readInput(StringList& map)
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
		if (!map.empty() && line.length() != map[0].length())
		{
			LOG_ERROR("Invalid map containing lines with different lengths.");
		}
		map.push_back(line);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

void printMap(const StringList& map)
{
	for (const std::string& line : map)
	{
		std::cout << line << std::endl;
	}
	std::cout << std::endl;
}

void printMapAndAntMap(const StringList& map, const IntMatrix& antMap)
{
	for (int row = 0; row < int(map.size()); row++)
	{
		std::cout << map[row] << "    ";
		for (int col = 0; col < int(antMap[row].size()); col++)
		{
			std::cout << antMap[row][col];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

bool isValidPosition(int row, int col, const StringList& map)
{
	return row >= 0 && row < int(map.size()) && col >= 0 && col < int(map[row].length());
}

bool findFreq(const StringList& map, char freq, int startRow, int startCol, int& resultRow, int& resultCol)
{
	resultRow = resultCol = -1;
	for (int row = startRow; row < int(map.size()); row++)
	{
		const std::string& line = map[row];
		for (int col = (row == startRow) ? startCol + 1 : 0; col < int(line.length()); col++)
		{
			if (line[col] == freq)
			{
				resultRow = row;
				resultCol = col;
				return true;
			}
		}
	}
	return false;
}

void getTwoAntinodesPositions(int aRow, int aCol, int bRow, int bCol, int& aAntRow, int& aAntCol, int& bAntRow, int& bAntCol)
{
	if (aRow == bRow || aCol == bCol)
	{
		LOG_ERROR("Invalid positions of A and B node given when calculating antinodes positions.");
		aAntRow = aAntCol = bAntRow = bAntCol = -1;
		return;
	}

	aAntRow = aRow * 2 - bRow;
	aAntCol = aCol * 2 - bCol;
	bAntRow = bRow * 2 - aRow;
	bAntCol = bCol * 2 - aCol;
}

void getAntinodeMap01(const StringList& map, IntMatrix& antMap)
{
	for (const std::string& line : map)
	{
		antMap.emplace_back(line.length(), 0);
	}

	for (int aRow = 0; aRow < int(map.size()); aRow++)
	{
		const std::string& line = map[aRow];
		for (int aCol = 0; aCol < int(line.length()); aCol++)
		{
			if (line[aCol] != '.')
			{
				const char freq = line[aCol];
				
				int bRow = aRow, bCol = aCol;
				while (findFreq(map, freq, bRow, bCol, bRow, bCol))
				{
					int aAntRow = -1, aAntCol = -1, bAntRow = -1, bAntCol = -1;
					getTwoAntinodesPositions(aRow, aCol, bRow, bCol, aAntRow, aAntCol, bAntRow, bAntCol);
					if (isValidPosition(aAntRow, aAntCol, map))
					{
						antMap[aAntRow][aAntCol]++;
					}
					if (isValidPosition(bAntRow, bAntCol, map))
					{
						antMap[bAntRow][bAntCol]++;
					}
				}
			}
		}
	}
}

void getAntinodeMap02(const StringList& map, IntMatrix& antMap)
{
	for (const std::string& line : map)
	{
		antMap.emplace_back(line.length(), 0);
	}

	for (int aRow = 0; aRow < int(map.size()); aRow++)
	{
		const std::string& line = map[aRow];
		for (int aCol = 0; aCol < int(line.length()); aCol++)
		{
			if (line[aCol] != '.')
			{
				const char freq = line[aCol];

				int bRow = aRow, bCol = aCol;
				while (findFreq(map, freq, bRow, bCol, bRow, bCol))
				{
					const int vecRow = bRow - aRow;
					const int vecCol = bCol - aCol;

					for (int coef = 0; ; coef++)
					{
						const int cRow = aRow + vecRow * coef;
						const int cCol = aCol + vecCol * coef;
						if (!isValidPosition(cRow, cCol, map))
						{
							break;
						}
						if (coef > map.size())
						{
							LOG_ERROR("Running a coefficient for an antenna vector for too long! Something's wrong.");
							break;
						}

						antMap[cRow][cCol]++;
					}

					for (int coef = -1; ; coef--)
					{
						const int cRow = aRow + vecRow * coef;
						const int cCol = aCol + vecCol * coef;
						if (!isValidPosition(cRow, cCol, map))
						{
							break;
						}
						if (-coef > map.size())
						{
							LOG_ERROR("Running a coefficient for an antenna vector for too long! Something's wrong.");
							break;
						}

						antMap[cRow][cCol]++;
					}
				}
			}
		}
	}
}

int countAntinodes(const IntMatrix& antMap)
{
	int count = 0;
	for (int row = 0; row < int(antMap.size()); row++)
	{
		const IntList& line = antMap[row];
		for (int col = 0; col < int(line.size()); col++)
		{
			if (antMap[row][col] > 0)
			{
				count++;
			}
		}
	}
	return count;
}

void solve01(const StringList& map)
{
	IntMatrix antMap;
	getAntinodeMap01(map, antMap);
	std::cout << countAntinodes(antMap) << std::endl;
}

void solve02(const StringList& map)
{
	IntMatrix antMap;
	getAntinodeMap02(map, antMap);
	std::cout << countAntinodes(antMap) << std::endl;
}

int main()
{
	StringList map;
	readInput(map);

	solve01(map);
	solve02(map);
}