#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <string>
#include <vector>


template <typename T>
bool belongs(const T& e, const std::vector<T>& vec)
{
	return (std::find(vec.begin(), vec.end(), e) != vec.end());
}

template <typename T>
unsigned int position(const T& e, const std::vector<T>& vec)
{
	if (!belongs(e, vec))
		throw std::domain_error("element not present in vector");
	else
		return std::find(vec.begin(), vec.end(), e) - vec.begin();
}


template <typename T>
void permute_vec(std::vector<T>& vec, std::vector<std::size_t> const& permutation)
{
	for (unsigned int i = 1, j; i < permutation.size(); ++i)
	{
		for (j = permutation[i]; j < i; j = permutation[j])
			;
		if (j == i)
			while (j = permutation[j], j != i)
				std::swap(vec[i], vec[j]);
	}
}

bool isSpaceOrComma(char);

std::vector<std::string> splitStr(const std::string&);

std::string::size_type strVecWidth(const std::vector<std::string>);

#endif