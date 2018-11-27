/*
I may be able to improve this. 

I've written a class template called NamedVec which allows me to give any 
std::vector a name in the form of a std::string. I've given NamedVector some 
operators and methods (such as clear and erase) that allow one to access and
manipulate the underlying std::vector.


The class which implements the data tables is called DataFrame.
DataFrame has three data members:
1. key is a NamedVec<std::string> which represents the column of the data table
to be used for uniting, intersecting, and differencing tables, as well as 
joining two tables with different columns. In our context, key.name would be
something like "Molecule" and key.vec would contain the names of various 
molecules. 
2. numVars is a std::vector<NamedVec<double>>. Each element of numVars 
represents a column of the data table containing numerical values. 
3. stringVars is a std::vector<NamedVec<std::string>>. Each element of 
stringVars represents a column of the data table containing string values.


The << and >> operators are overloaded to allow a DataFrame to be
read from and printed to an istream and an ostream respectively. In order to 
read into a DataFrame from an istream, the istream be formatted in the usual 
comma-separated-value style, with column names (i.e. variable names) not 
containing spaces (though it would not be difficult to edit the code to allow 
names with spaces). I have provided several sample .txt files and a 
sample main function which demonstrates the reading and writing of DataFrames. 
(The DataFrame constructor which takes a reference to an istream calls the same
private DataFrame::read method which is called by the overloaded >> operator.)
I have set the << operator to print numerical values to have width 5. In case
the user wants to change this, I have provided a public DataFrame::display 
method which takes an unsigned int (specifying the width of printed numerical 
variables) and a reference to an ostream. 


I have provided unite, intersect, diff, and left_join functions, all of which 
take two references to DataFrame consts and return DataFrames by value. I 
describe here what these functions do and the restrictions of which the user 
must be aware.


*/

#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "utilities.h"

template <typename T>
class NamedVec
{
	friend class DataFrame;
	friend DataFrame unite(const DataFrame&, const DataFrame&);
	friend DataFrame diff(const DataFrame&, const DataFrame&);
	friend DataFrame intersect(const DataFrame&, const DataFrame&);
	friend DataFrame left_filter_join(const DataFrame&, const DataFrame&);


	NamedVec(const std::string& nm = std::string(),
		std::vector<T> v = std::vector<T>()) : name(nm), vec(v) {}
	T& operator[](int i) { return vec[i]; }
	const T& operator[](int i) const { return vec[i]; }
	void psh_bck(const T& t) { vec.push_back(t); }
	int sz() const { return vec.size(); }
	const std::string& getName() const { return name; }
	std::vector<T>& getVec() { return vec; }
	const std::vector<T>& getVec() const { return vec; }
	void erase(std::ptrdiff_t i) { vec.erase(vec.begin() + i); }
	void permute(const std::vector<std::size_t> permutation) { permute_vec(vec, permutation); }
	void clear() { name = std::string();  vec.clear(); }

	std::string name;
	std::vector<T> vec;
};

class DataFrame
{
	friend std::ostream& operator<<(std::ostream&, const DataFrame&);
	friend std::istream& operator>>(std::istream&, DataFrame&);
	friend DataFrame unite(const DataFrame&, const DataFrame&);
	friend DataFrame intersect(const DataFrame&, const DataFrame&);
	friend DataFrame diff(const DataFrame&, const DataFrame&);
	friend DataFrame left_filter_join(const DataFrame&, const DataFrame&);
public:
	DataFrame() {}
	DataFrame(std::istream& is) { read(is); }


	// The display method is included in the public interface of DataFrame in 
	// case the user wishes to specify the width of the displayed numerical 
	// values. Otherwise, the user may call the overloaded non-member 
	// operator <<, which prints numerical values with width 5.  
	std::ostream& display(unsigned int, std::ostream&) const;

private:
	void read(std::istream&);
	int dfHeight() const { return key.sz(); }
	int dfWidth() const { return numVars.size() + stringVars.size(); }
	void erase_rows(const std::vector<int>); // argument should be a vector of
											 // increasing ints
	const std::vector<std::string>& getKeyVector() const { return key.getVec(); }

// Data members
	NamedVec<std::string> key;
	std::vector<NamedVec<double>> numVars;
	std::vector<NamedVec<std::string>> stringVars;
};


// Functions for combining tables
DataFrame unite(const DataFrame&, const DataFrame&);
DataFrame intersect(const DataFrame&, const DataFrame&);
DataFrame diff(const DataFrame&, const DataFrame&);
DataFrame left_filter_join(const DataFrame&, const DataFrame&);

// Overloading the << and >> operators
std::ostream& operator<<(std::ostream&, const DataFrame&);
std::istream& operator>>(std::istream&, DataFrame&);


#endif