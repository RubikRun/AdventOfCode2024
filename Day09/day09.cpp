#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef std::vector<int> IntList;
typedef long long unsigned BigUInt;

int charToInt(char c)
{
	if (c < '0' || c > '9')
	{
		return -1;
	}
	return c - '0';
}

bool readInput(IntList& diskMap)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	char c;
	while (file.get(c))
	{
		const int cInt = charToInt(c);
		if (cInt == -1)
		{
			LOG_ERROR("Input file contains a non-digit character: " << c);
			continue;
		}
		diskMap.push_back(cInt);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

void printDiskMap(const IntList& diskMap)
{
	for (int d : diskMap)
	{
		std::cout << d;
	}
	std::cout << std::endl;
}

void printBlockMap(const IntList& blockMap)
{
	for (int b : blockMap)
	{
		if (b == -1)
		{
			std::cout << " . ";
		}
		else
		{
			std::cout << b << " ";
		}
	}
	std::cout << std::endl;
}

void addBlock(IntList& blockMap, int value, int count)
{
	for (int j = 0; j < count; j++)
	{
		blockMap.push_back(value);
	}
}

void getBlockMap(const IntList& diskMap, IntList& blockMap)
{
	int fileId = 0;
	for (int i = 0; i + 1 < int(diskMap.size()); i += 2, fileId++)
	{
		addBlock(blockMap, fileId, diskMap[i]);
		addBlock(blockMap, -1, diskMap[i + 1]);
	}
	addBlock(blockMap, fileId, diskMap.back());
}

int findLastFileBlock(const IntList& blockMap)
{
	for (int i = int(blockMap.size()) - 1; i >= 0; i--)
	{
		if (blockMap[i] != -1)
		{
			return i;
		}
	}
	return -1;
}

int findFirstEmptyBlock(const IntList& blockMap)
{
	for (int i = 0; i < blockMap.size(); i++)
	{
		if (blockMap[i] == -1)
		{
			return i;
		}
	}
	return -1;
}


bool moveLastFileBlock(IntList& blockMap)
{
	const int lastFileBlockIdx = findLastFileBlock(blockMap);
	if (lastFileBlockIdx < 0)
	{
		LOG_ERROR("Trying to move last file block, but there are no file blocks.");
		return false;
	}
	const int firstEmptyBlockIdx = findFirstEmptyBlock(blockMap);
	if (firstEmptyBlockIdx < 0)
	{
		LOG_ERROR("Trying to move last file block, but there are no empty blocks.");
		return false;
	}

	// All empty blocks are after all file blocks,
	// meaning that compression is complete.
	if (firstEmptyBlockIdx >= lastFileBlockIdx)
	{
		return false;
	}

	blockMap[firstEmptyBlockIdx] = blockMap[lastFileBlockIdx];
	blockMap[lastFileBlockIdx] = -1;
	return true;
}

int findFileBegin(const IntList& blockMap, int fileId)
{
	for (int i = 0; i < blockMap.size(); i++)
	{
		if (blockMap[i] == fileId)
		{
			return i;
		}
	}
	return -1;
}

int getFileSize(const IntList& blockMap, int fileBegin)
{
	int fileEnd = fileBegin + 1;
	while (fileEnd < blockMap.size() && blockMap[fileEnd] == blockMap[fileBegin])
	{
		fileEnd++;
	}
	return fileEnd - fileBegin;
}

int findFirstEmptyInterval(const IntList& blockMap, int intervalSize, int maxEnd)
{
	int consecutive = 0;
	for (int i = 0; i < maxEnd; i++)
	{
		if (blockMap[i] == -1)
		{
			consecutive++;
		}
		else
		{
			consecutive = 0;
		}

		if (consecutive >= intervalSize)
		{
			return i - consecutive + 1;
		}
	}
	return -1;
}

void moveFile(IntList& blockMap, int fileId)
{
	const int fileBegin = findFileBegin(blockMap, fileId);
	if (fileBegin < 0)
	{
		LOG_ERROR("Trying to move file to first empty interval, but there are no file blocks with given file ID.");
		return;
	}
	const int fileSize = getFileSize(blockMap, fileBegin);
	const int intervalBegin = findFirstEmptyInterval(blockMap, fileSize, fileBegin);
	if (intervalBegin < 0 || intervalBegin > fileBegin)
	{
		return;
	}
	for (int i = intervalBegin; i < intervalBegin + fileSize; i++)
	{
		if (blockMap[i] != -1)
		{
			LOG_ERROR("Logic error. We found an interval that should be empty but apparently it's not. Debug here.");
		}
		blockMap[i] = fileId;
	}
	for (int i = fileBegin; i < fileBegin + fileSize; i++)
	{
		if (blockMap[i] != fileId)
		{
			LOG_ERROR("Logic error. Trying to move a file but there are other blocks in file's interval. Debug here.");
		}
		blockMap[i] = -1;
	}
}

void compressBlockMap01(IntList& blockMap)
{
	while (moveLastFileBlock(blockMap));
}

void compressBlockMap02(IntList& blockMap)
{
	const int lastFileBlockIdx = findLastFileBlock(blockMap);
	if (lastFileBlockIdx < 0)
	{
		LOG_ERROR("Trying to compress a block map but there are no file blocks.");
		return;
	}
	const int maxFileId = blockMap[lastFileBlockIdx];

	for (int fileId = maxFileId; fileId >= 0; fileId--)
	{
		moveFile(blockMap, fileId);
	}
}

BigUInt getChecksum(const IntList& blockMap)
{
	BigUInt checksum = 0;
	for (int i = 0; i < blockMap.size(); i++)
	{
		if (blockMap[i] < 0)
		{
			continue;
		}
		checksum += BigUInt(i * blockMap[i]);
	}
	return checksum;
}

void solve01(const IntList& diskMap)
{
	if (diskMap.size() % 2 != 1)
	{
		LOG_ERROR("Invalid disk map with an even size.");
		return;
	}

	IntList blockMap;
	getBlockMap(diskMap, blockMap);
	compressBlockMap01(blockMap);
	std::cout << getChecksum(blockMap) << std::endl;
}

void solve02(const IntList& diskMap)
{
	if (diskMap.size() % 2 != 1)
	{
		LOG_ERROR("Invalid disk map with an even size.");
		return;
	}

	IntList blockMap;
	getBlockMap(diskMap, blockMap);
	compressBlockMap02(blockMap);
	std::cout << getChecksum(blockMap) << std::endl;
}

int main()
{
	IntList diskMap;
	readInput(diskMap);

	solve01(diskMap);
	solve02(diskMap);
}