#include "DataFrame.h"

void DataFrame::read(std::istream& is)
{
	key.clear();
	stringVars.clear();
	numVars.clear();

	std::vector<std::vector<std::string>> vsv;
	std::string s;
	while (std::getline(is, s))
		vsv.push_back(splitStr(s));
	is.clear();
	is.seekg(0);


	int dfW = vsv[0].size();
	int dfH = vsv.size(); // represents the height of the data frame, 
						  // including the rows with names and types
						  // of variables

	for (int j = 0; j < dfW; ++j)
	{
		if (vsv[1][j] == "string")
		{
			NamedVec<std::string> temp(vsv[0][j]);
			for (int i = 2; i < dfH; ++i)
				temp.psh_bck(vsv[i][j]);

			if (j == 0)
				key = temp;
			else
				stringVars.push_back(temp);
		}
		else if (vsv[1][j] == "double")
		{
			NamedVec<double> temp(vsv[0][j]);;
			for (int i = 2; i < dfH; ++i)
				temp.psh_bck(std::stod(vsv[i][j]));

			numVars.push_back(temp);
		}
	}
}

void DataFrame::erase_rows(const std::vector<int> indices_to_remove) // argument should be an increasing vector of ints
{
	int number_of_trips_through_loop = 0;
	for (auto i : indices_to_remove)
	{
		key.erase(i - number_of_trips_through_loop);
		for (unsigned int j = 0; j < stringVars.size(); ++j)
		{
			stringVars[j].erase(i - number_of_trips_through_loop);
		}
		for (unsigned int j = 0; j < numVars.size(); ++j)
		{
			numVars[j].erase(i - number_of_trips_through_loop);
		}
		++number_of_trips_through_loop;
	}
}

std::ostream& DataFrame::display(unsigned int doublew, std::ostream& os) const
{
	int num_string_vars = stringVars.size();
	int num_doub_vars = numVars.size();
	int dfH = dfHeight();


	auto key_width = std::max(key.getName().size(),
		strVecWidth(key.getVec()));

	std::vector<std::size_t> string_column_widths;
	for (const auto& v : stringVars)
		string_column_widths.push_back(std::max(v.getName().size(), strVecWidth(v.getVec())));

	std::vector<std::size_t> num_column_widths;
	for (const auto& v : numVars)
		num_column_widths.push_back(std::max((std::string::size_type) doublew, v.getName().size()));

	os << key.getName() << std::string(1 + key_width - key.getName().size(), ' ');
	for (unsigned int i = 0; i < stringVars.size(); ++i)
		os << stringVars[i].getName() << std::string(1 + string_column_widths[i] - stringVars[i].getName().size(), ' ');
	for (unsigned int i = 0; i < numVars.size(); ++i)
		os << numVars[i].getName() << std::string(1 + num_column_widths[i] - numVars[i].getName().size(), ' ');
	os << "\n\n";


	std::streamsize orig = os.width();

	for (int i = 0; i < dfH; ++i)
	{
		os << key[i] << std::string(1 + std::max(key[i].size(), strVecWidth(key.getVec())) - key[i].size(), ' ');
		for (unsigned int j = 0; j < stringVars.size(); ++j)
			os << stringVars[j][i] << std::string(1 + string_column_widths[j] - stringVars[j][i].size(), ' ');
		os << std::setw(doublew);
		for (unsigned int j = 0; j < numVars.size(); ++j)
			os << numVars[j][i] << std::string(1 + num_column_widths[j] - doublew, ' ');
		os << '\n';
		os << std::setw(orig);
	}
	return os;
}

// ----------------------------------------------------------------------------

DataFrame unite(const DataFrame& left, const DataFrame& right)
{
	const std::vector<std::string>& lv = left.getKeyVector();
	const std::vector<std::string>& rv = right.getKeyVector();

	DataFrame ret = left;

	std::vector<int> indices_to_add;
	for (unsigned int i = 0; i < rv.size(); ++i)
		if (!(belongs(rv[i], lv)))
			indices_to_add.push_back(i);

	for (auto i : indices_to_add)
	{
		ret.key.psh_bck((right.key)[i]);
		for (unsigned int j = 0; j < left.stringVars.size(); ++j)
		{
			ret.stringVars[j].psh_bck(right.stringVars[j][i]);
		}
		for (unsigned int j = 0; j < left.numVars.size(); ++j)
		{
			ret.numVars[j].psh_bck(right.numVars[j][i]);
		}
	}

	return ret;
}

DataFrame intersect(const DataFrame& left, const DataFrame& right)
{
	const std::vector<std::string>& lv = left.getKeyVector();
	const std::vector<std::string>& rv = right.getKeyVector();

	DataFrame ret = left;

	std::vector<int> indices_to_remove;
	for (unsigned int i = 0; i < lv.size(); ++i)
		if (!belongs(lv[i], rv))
			indices_to_remove.push_back(i);

	ret.erase_rows(indices_to_remove);

	return ret;
}

DataFrame diff(const DataFrame& left, const DataFrame& right)
{
	const std::vector<std::string>& lv = left.getKeyVector();
	const std::vector<std::string>& rv = right.getKeyVector();

	DataFrame ret = left;

	std::vector<int> indices_to_remove;
	for (unsigned int i = 0; i < lv.size(); ++i)
		if (belongs(lv[i], rv))
			indices_to_remove.push_back(i);
	ret.erase_rows(indices_to_remove);

	return ret;
}

DataFrame left_filter_join(const DataFrame& left, const DataFrame& right)
{
	const std::vector<std::string>& lv = left.getKeyVector();
	const std::vector<std::string>& rv = right.getKeyVector();

	DataFrame ret = left;

	std::vector<int> left_indices_to_remove;
	for (unsigned int i = 0; i < lv.size(); ++i)
		if (!belongs(lv[i], rv))
			left_indices_to_remove.push_back(i);
	ret.erase_rows(left_indices_to_remove);

	DataFrame other = right;
	std::vector<int> right_indices_to_remove;
	for (unsigned int i = 0; i < rv.size(); ++i)
		if (!belongs(rv[i], lv))
			right_indices_to_remove.push_back(i);
	other.erase_rows(right_indices_to_remove);


	std::vector<std::string> ret_stringVarNames;
	for (const auto& v : ret.stringVars)
		ret_stringVarNames.push_back(v.getName());
	std::vector<std::string> ret_numVarNames;
	for (const auto& v : ret.numVars)
		ret_numVarNames.push_back(v.getName());
	std::vector<std::string> other_stringVarNames;
	for (const auto& v : other.stringVars)
		other_stringVarNames.push_back(v.getName());
	std::vector<std::string> other_numVarNames;
	for (const auto& v : other.numVars)
		other_numVarNames.push_back(v.getName());

	std::vector<std::size_t> permutation;
	for (const auto& v : ret.getKeyVector())
		permutation.push_back(position(v, other.getKeyVector()));

	std::vector<NamedVec<std::string>> stringVarsNeeded;
	for (unsigned int i = 0; i < other_stringVarNames.size(); ++i)
		if (!belongs(other_stringVarNames[i], ret_stringVarNames))
		{
			other.stringVars[i].permute(permutation);
			stringVarsNeeded.push_back(other.stringVars[i]);
		}
	for (const auto& v : stringVarsNeeded)
		ret.stringVars.push_back(v);

	std::vector<NamedVec<double>> numVarsNeeded;
	for (unsigned int i = 0; i < other_numVarNames.size(); ++i)
		if (!belongs(other_numVarNames[i], ret_numVarNames))
		{
			other.numVars[i].permute(permutation);
			numVarsNeeded.push_back(other.numVars[i]);
		}
	for (const auto& v : numVarsNeeded)
		ret.numVars.push_back(v);

	// now we have the columns to be added, but first we must put the elements 
	// of the columns in the correct order


	return ret;
}

std::ostream& operator<<(std::ostream& os, const DataFrame& df)
{
	return df.display(5, os);
}

std::istream& operator>>(std::istream& is, DataFrame& df)
{
	df.read(is);
	return is;
}