#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

void file_exists(const std::string& file_path)
{
    struct stat buffer;
    struct stat buffer1;
    printf(" file_exists shared_library_filepath %d\n", stat (file_path.c_str(), &buffer));
    //return (stat (file_path.c_str(), &buffer)==0);
}

// int main(int argc, char*argv[])
// {
// std::string file_path ;//= "/home/kpit/magna/repo/FE_RADAR_MERdrDataCubeMidW/Test/AutoGen/Ros2/src/swc_merdrdatacubemidw/lib/MERdrDataCubeMidW_Algo.so";

// 	std::cout<<"Input file apth\n";
// 	std::cin>>file_path;
	
// 	std::cout<<file_path;
//     struct stat buffer;
//     struct stat buffer1;

// file_exists(file_path);
// //stat (file_path.c_str(), &buffer);

// //printf(" file_exists shared_library_filepath %d\n", stat (file_path.c_str(), &buffer));

// }

/* Read input from STDIN. Print your output to STDOUT*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
using namespace std;
std::string count_the_votes(vector<int> &votes, int votes_needed_to_win_jani)
{
	std::unordered_map<int, vector<int>> unique_votes;

	for (int i{0}; i<votes.size(); ++i)
	{
		vector<int> index;
		if (unique_votes.find(votes[i]) != unique_votes.end())
		{
			index = unique_votes[votes[i]];
			index.push_back(i);
			unique_votes[votes[i]] = index;
		}
		else
		{
			index.push_back(i);
			unique_votes[votes[i]] = index;
		}
	}
	return unique_votes.size() >= votes_needed_to_win_jani ?
		"JANI" : "RAMYA";
}
int main(int argc, char *a[])
{
	int testcases;
	std::cin>>testcases;

    while(testcases--)
    {
        int number_of_votes, votes_needed_to_win_jani;
        std::cin>>number_of_votes>>votes_needed_to_win_jani;

        vector<int> votes(number_of_votes,0);
        for (int i{0}; i<number_of_votes; ++i)
            std::cin>>votes[i];

        std::cout<<count_the_votes(votes, votes_needed_to_win_jani );
    }

}

	


