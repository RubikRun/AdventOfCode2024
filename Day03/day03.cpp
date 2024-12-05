#include <iostream>
#include <fstream>
#include <sstream>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";
typedef long long unsigned BigUInt;

bool readInput(std::string& memory)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	std::ostringstream sstream;
	sstream << file.rdbuf();
	memory = sstream.str();

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

int parseDigit(char c)
{
	if (c < '0' || c > '9')
	{
		LOG_ERROR("Trying to parse a digit from a character that is not a digit.");
		return -1;
	}
	return c - '0';
}

// Parses a number from memory at a given index.
// The number is expected to be a positive integer with at most 3 digits.
// Returns the resulting number, and a -1 if there is no such number at that index.
// If there is a number with more digits at that index, a number with the first 3 digits will NOT be parsed, -1 will be returned.
// Fills the digitsCount parameter with the number of digits of the parsed number.
int parseNumber(const std::string& mem, int i, int& digitsCount)
{
	if (i < 0 || i >= mem.length() || !std::isdigit(mem[i]))
	{
		return -1;
	}
	int endIdx = i + 1;
	while (endIdx < mem.length() && std::isdigit(mem[endIdx]))
	{
		endIdx++;
		if (endIdx - i > 3)
		{
			return -1;
		}
	}

	int num = 0;
	for (int j = i; j < endIdx; j++)
	{
		num *= 10;
		num += parseDigit(mem[j]);
	}

	digitsCount = endIdx - i;
	if (digitsCount < 1 || digitsCount > 3)
	{
		LOG_ERROR("Unexpected number of digits when parsing a number.");
	}
	return num;
}

// Calculates a mul(X,Y) instruction from memory at a given index.
// Returns -1 if there is no valid mul(X,Y) instruction at that index.
int calcMul(const std::string& mem, int i, int& mulLength)
{
	// A mul instruction is at least 8 characters long
	if (i < 0 || i + 8 > mem.length())
	{
		return -1;
	}
	if (mem[i] != 'm' || mem[i + 1] != 'u' || mem[i + 2] != 'l' || mem[i + 3] != '(')
	{
		return -1;
	}

	const int xi = i + 4;
	int xDigitsCount;
	const int x = parseNumber(mem, xi, xDigitsCount);
	if (x < 0)
	{
		return -1;
	}
	// After X there should be at least 3 characters: ,Y)
	if (xi + xDigitsCount + 2 >= mem.length() || mem[xi + xDigitsCount] != ',')
	{
		return -1;
	}
	const int yi = xi + xDigitsCount + 1;
	int yDigitsCount;
	const int y = parseNumber(mem, yi, yDigitsCount);
	if (y < 0)
	{
		return -1;
	}
	if (yi + yDigitsCount >= mem.length() || mem[yi + yDigitsCount] != ')')
	{
		return -1;
	}

	mulLength = yi + yDigitsCount + 1 - i;
	const int result = x * y;
	return result;
}

bool isDoInstruction(const std::string& mem, int i)
{
	// A "do" instruction is exactly 4 characters long: do()
	return (i >= 0 && i + 3 < mem.length() && mem[i] == 'd' && mem[i + 1] == 'o' && mem[i + 2] == '(' && mem[i + 3] == ')');
}

bool isDontInstruction(const std::string& mem, int i)
{
	// A "don't" instruction is exactly 7 characters long: don't()
	return (i >= 0 && i + 7 < mem.length()
		&& mem[i] == 'd' && mem[i + 1] == 'o' && mem[i + 2] == 'n' && mem[i + 3] == '\'' && mem[i + 4] == 't' && mem[i + 5] == '(' && mem[i + 6] == ')');
}

void solve01(const std::string& memory)
{
	BigUInt sum = 0;
	for (int i = 0; i < memory.length(); i++)
	{
		int mulLength;
		int mulResult = calcMul(memory, i, mulLength);
		if (mulResult >= 0)
		{
			sum += BigUInt(mulResult);
			i += mulLength - 1;
		}
	}
	std::cout << sum << std::endl;
}

void solve02(const std::string& memory)
{
	bool enabled = true;
	BigUInt sum = 0;
	for (int i = 0; i < memory.length(); i++)
	{
		if (enabled)
		{
			int mulLength;
			int mulResult = calcMul(memory, i, mulLength);
			if (mulResult >= 0)
			{
				sum += BigUInt(mulResult);
				i += mulLength - 1;
				continue;
			}
		}
		if (isDoInstruction(memory, i))
		{
			enabled = true;
		}
		else if (isDontInstruction(memory, i))
		{
			enabled = false;
		}
	}
	std::cout << sum << std::endl;
}

int main()
{
	std::string memory;
	if (!readInput(memory))
	{
		return 0;
	}

	solve01(memory);
	solve02(memory);
}