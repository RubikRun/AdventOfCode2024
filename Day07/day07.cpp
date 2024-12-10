#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";

typedef long long BigInt;
typedef std::vector<BigInt> BigIntList;

enum class Operator
{
	Add,
	Multiply,
	Concatenate
};
typedef std::vector<Operator> OperatorList;

static BigInt concatenate(BigInt left, BigInt right)
{
	if (right == 0)
	{
		return left * 10;
	}

	std::vector<int> rDigits;

	while (right != 0)
	{
		rDigits.push_back(int(right % 10));
		right /= 10;
	}

	for (int i = int(rDigits.size()) - 1; i >= 0; i--)
	{
		left *= 10;
		left += BigInt(rDigits[i]);
	}

	return left;
}

struct Equation
{
	BigInt testValue = -1;
	BigIntList terms;

	bool isSolvable(bool enableConcatenate, OperatorList& operators, BigInt resBuf) const
	{
		if (terms.empty())
		{
			LOG_ERROR("Checking if an equation is solvable, but equation has no terms.");
			return false;
		}
		if (operators.size() + 1 > terms.size())
		{
			LOG_ERROR("Checking if an equation is solvable with more operators than needed.");
			return false;
		}

		if (operators.size() + 1 == terms.size())
		{
			return resBuf == testValue;
		}

		if (operators.empty())
		{
			resBuf = terms[0];
		}

		// A small optimization:
		// If our result is already greater than the test value,
		// there is no way we can reach the test value by adding/multiplying more numbers.
		if (resBuf > testValue)
		{
			return false;
		}

		operators.push_back(Operator::Add);
		if (isSolvable(enableConcatenate, operators, resBuf + terms[operators.size()]))
		{
			return true;
		}
		operators.pop_back();
		operators.push_back(Operator::Multiply);
		if (isSolvable(enableConcatenate, operators, resBuf * terms[operators.size()]))
		{
			return true;
		}
		operators.pop_back();
		if (enableConcatenate)
		{
			operators.push_back(Operator::Concatenate);
			if (isSolvable(enableConcatenate, operators, concatenate(resBuf, terms[operators.size()])))
			{
				return true;
			}
			operators.pop_back();
		}
		return false;
	}
};
typedef std::vector<Equation> ListOfEquation;

void printEquation(const Equation& equation)
{
	std::cout << equation.testValue << ": ";
	const BigIntList& terms = equation.terms;
	if (terms.empty())
	{
		return;
	}
	std::cout << terms[0];
	for (int i = 1; i < terms.size(); i++)
	{
		std::cout << " " << terms[i];
	}
	std::cout << std::endl;
}

void printEquations(const ListOfEquation& equations)
{
	for (const Equation& equation : equations)
	{
		printEquation(equation);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void parseTerms(std::string str, BigIntList& terms)
{
	while (!str.empty())
	{
		std::string termStr = str;
		const size_t sepIdx = str.find(' ');
		if (sepIdx != std::string::npos)
		{
			termStr = str.substr(0, sepIdx);
			str = str.substr(sepIdx + 1);
		}
		else
		{
			str = std::string();
		}

		try
		{
			const BigInt term = std::stoll(termStr);
			terms.push_back(term);
		}
		catch (...)
		{
			LOG_ERROR("Trying to parse an invalid list of terms, containing a non-integer term.");
		}
	}
}

void parseEquation(const std::string& str, Equation& equation)
{
	size_t sepIdx = str.find(": ");
	if (sepIdx == std::string::npos)
	{
		LOG_ERROR("Trying to parse an invalid equation without a \": \" separator.");
		return;
	}
	if (sepIdx < 1 || sepIdx + 2 >= str.length())
	{
		LOG_ERROR("Trying to parse an invalid equation where \": \" separator is at the beginning or end.");
		return;
	}

	try
	{
		equation.testValue = std::stoll(str.substr(0, sepIdx));
	}
	catch (...)
	{
		LOG_ERROR("Trying to parse an invalid equation with a non-integer test value.");
	}

	parseTerms(str.substr(sepIdx + 2), equation.terms);
}

bool readInput(ListOfEquation& equations)
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
		Equation equation;
		parseEquation(line, equation);
		equations.push_back(equation);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

void solve01(const ListOfEquation& equations)
{
	BigInt sum = 0;
	for (const Equation& equation : equations)
	{
		OperatorList operators;
		if (equation.isSolvable(false, operators, 0))
		{
			sum += equation.testValue;
		}
	}
	std::cout << sum << std::endl;
}

void solve02(const ListOfEquation& equations)
{
	BigInt sum = 0;
	for (const Equation& equation : equations)
	{
		OperatorList operators;
		if (equation.isSolvable(true, operators, 0))
		{
			sum += equation.testValue;
		}
	}
	std::cout << sum << std::endl;
}

int main()
{
	ListOfEquation equations;
	readInput(equations);

	solve01(equations);
	solve02(equations);
}