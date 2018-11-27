#include "utilities.h"

bool isSpaceOrComma(char c)
{
	if (isspace(c) || c == ',')
		return true;
	else
		return false;
}

std::vector<std::string> splitStr(const std::string& str)
{
	std::vector<std::string> retVec;
	std::string::size_type i = 0;
	auto arg_size = str.size();

	while (i != arg_size)
	{
		while (i != arg_size && (isSpaceOrComma(str[i])))
			++i;
		std::string::size_type j = i;
		while (j != arg_size && !isSpaceOrComma(str[j]))
			++j;
		if (i != j)
		{
			retVec.push_back(str.substr(i, j - i));
			i = j;
		}
	}

	return retVec;
}

std::string::size_type strVecWidth(const std::vector<std::string> vec)
{
	unsigned int ret = 0;
	for (auto& s : vec)
	{
		if (s.size() > ret)
			ret = s.size();
	}
	return ret;
}