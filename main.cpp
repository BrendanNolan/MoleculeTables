#include <iostream>
#include <fstream>
#include <string>


#include "DataFrame.h"

int main()
{	
	std::ifstream infile0("test_csv0.txt");
	std::ifstream infile1("test_csv1.txt");
	std::ifstream infile2("test_csv2.txt");
	std::ifstream infile3("test_csv3.txt");


	DataFrame df0(infile0);
	DataFrame df1(infile1);
	DataFrame df2(infile2);

	DataFrame df3;
	infile2 >> df3;

	auto united_df = unite(df0, df1);
	auto intersected_df = intersect(df0, df1);
	auto diffed_df = diff(df0, df1);

	std::cout << std::endl << "df0 ---------------------------" << std::endl
		<< df0
		<< std::endl << "df1 ---------------------------" << std::endl
		<< df1
		<< std::endl << "united_df ---------------------------" << std::endl
		<< united_df
		<< std::endl << "intersected_df ---------------------------" << std::endl
		<< intersected_df
		<< std::endl << "diffed_df ---------------------------" << std::endl
		<< diffed_df
		<< std::endl << "df0 post erase ---------------------------" << std::endl;
	std::cout << df0 
	<< std::endl << "joined_df---------------------------" << std::endl;

	auto joined_df = left_filter_join(df0, df2);
	std::cout << joined_df;

	std::cout << std::endl << "df3---------------------------" << std::endl;
	std::cout << df3 << std::endl;

	std::cin.get();
}