#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
const char* INPUT_FILENAME = "data/input.txt";

struct Rule
{
	int aPage = -1;
	int bPage = -1;
};

typedef std::vector<Rule> RuleList;
typedef std::vector<int> Update;
typedef std::vector<Update> UpdateList;

Rule parseRule(const std::string& str)
{
	Rule rule;

	const size_t sepIdx = str.find('|');
	if (sepIdx == std::string::npos)
	{
		LOG_ERROR("Failed to parse invalid rule, not containing a | separator.");
		return rule;
	}
	if (sepIdx < 1 || sepIdx + 1 >= str.length())
	{
		LOG_ERROR("Failed to parse invalid rule, having its | separator at the beginning or end.");
		return rule;
	}

	try
	{
		rule.aPage = std::stoi(str.substr(0, sepIdx));
		rule.bPage = std::stoi(str.substr(sepIdx + 1));
	}
	catch (...)
	{
		LOG_ERROR("Failed to parse invalid rule. Page numbers are non-integer.");
	}

	return rule;
}

Update parseUpdate(const std::string& str)
{
	Update update;

	std::string buf = str;
	size_t sepIdx = std::string::npos;
	while ((sepIdx = buf.find(',')) != std::string::npos)
	{
		try
		{
			const int page = std::stoi(buf.substr(0, sepIdx));
			update.push_back(page);
		}
		catch (...)
		{
			LOG_ERROR("Failed to parse update. Non-integer page number.");
		}
		buf = buf.substr(sepIdx + 1);
	}

	try
	{
		const int page = std::stoi(buf);
		update.push_back(page);
	}
	catch (...)
	{
		LOG_ERROR("Failed to parse update. Last page number is non-integer.");
	}

	return update;
}

bool readInput(RuleList& ruleList, UpdateList& updateList)
{
	std::ifstream file(INPUT_FILENAME);
	if (!file.is_open())
	{
		LOG_ERROR("Cannot open input file.");
		return false;
	}

	std::string line;
	// Read and parse rules
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			break;
		}
		const Rule rule = parseRule(line);
		ruleList.push_back(rule);
	}
	// Read and parse updates
	while (std::getline(file, line))
	{
		const Update update = parseUpdate(line);
		updateList.push_back(update);
	}

	file.close();
	if (file.is_open())
	{
		LOG_ERROR("Cannot close input file.");
	}

	return true;
}

bool doesUpdateSatisfyRule(const Update& update, const Rule& rule)
{
	for (int i = 0; i < update.size(); i++)
	{
		if (update[i] != rule.bPage)
		{
			continue;
		}
		for (int j = i + 1; j < update.size(); j++)
		{
			if (update[j] == rule.aPage)
			{
				return false;
			}
		}
	}
	return true;
}

bool isUpdateOrdered(const Update& update, const RuleList& ruleList)
{
	for (const Rule& rule : ruleList)
	{
		if (!doesUpdateSatisfyRule(update, rule))
		{
			return false;
		}
	}
	return true;
}

void solve01(const RuleList& ruleList, const UpdateList& updateList)
{
	int middlePageSum = 0;
	for (const Update& update : updateList)
	{
		if (isUpdateOrdered(update, ruleList))
		{
			if (update.size() % 2 == 0)
			{
				LOG_ERROR("One of the correctly ordered updates has an even number of pages, meaning it has no middle page.");
				continue;
			}
			middlePageSum += update[update.size() / 2];
		}
	}
	std::cout << middlePageSum << std::endl;
}

void solve02(const RuleList& ruleList, const UpdateList& updateList)
{
	int middlePageSum = 0;
	for (Update update : updateList)
	{
		if (!isUpdateOrdered(update, ruleList))
		{
			if (update.size() % 2 == 0)
			{
				LOG_ERROR("One of the correctly ordered updates has an even number of pages, meaning it has no middle page.");
				continue;
			}

			std::sort(update.begin(), update.end(), [&ruleList](int i, int j) {
				for (const Rule& rule : ruleList)
				{
					if (rule.bPage == i && rule.aPage == j)
					{
						return false;
					}
				}
				return true;
			});

			middlePageSum += update[update.size() / 2];
		}
	}
	std::cout << middlePageSum << std::endl;
}

int main()
{
	RuleList ruleList;
	UpdateList updateList;
	readInput(ruleList, updateList);

	solve01(ruleList, updateList);
	solve02(ruleList, updateList);
}