#include<iostream>
#include<vector>
#include <algorithm>
#include <chrono>

#include "alex.h"

#include "load.hpp"
#include "parameters_alex.h"

/*
Test Script for ALEX
*/
using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

const int maxExecution = WINDOWSIZE + NO_EXECUTION;

template<typename T>
void test_alex(vector<T> &data, string filename)
{
	
    alex::Alex<T, int> index;

	int cnt = 0;

	fsec t_insert;
	fsec t_delete;
	fsec t_search;

    for(typename vector<T>::iterator it = data.begin(); it != data.end(); it++)
    {
		cnt++;
		int minT = 0;
		int maxT = 0;

		if( cnt <= WINDOWSIZE){index.insert(*it,cnt);}
		else
		{	
			auto t0 = Time::now();
			index.insert(*it,cnt);
			auto t1 = Time::now();
			t_insert += t1-t0;

			// T first_key = (*index.begin()).first;
            uint64_t first_key =  index.begin().key();

			auto t2 = Time::now();
			index.erase(first_key);
			auto t3 = Time::now();
			t_delete += t3-t2;

			minT = cnt - WINDOWSIZE;
		 	maxT = cnt;
			int target =  minT + (rand() % ( maxT - minT + 1 ));
			uint64_t search_key = data.at(target)
;
			auto t4 = Time::now();
			index.find(search_key);
			auto t5 = Time::now();
			t_search += t5-t4;
		}

		if (cnt == maxExecution){ break; }
    }


	double t_avg_insert = t_insert.count()/NO_EXECUTION;
	double t_avg_delete = t_delete.count()/NO_EXECUTION;
	double t_avg_search = t_search.count()/NO_EXECUTION;

	double t_total = t_avg_insert + t_avg_delete + t_avg_search;

	double throughput_insert = NO_EXECUTION/t_insert.count();
	double throughput_search = NO_EXECUTION/t_search.count();
	double throughput_delete = NO_EXECUTION/t_delete.count();
	double throughput = throughput_insert < throughput_search ? throughput_insert : throughput_search;
	throughput = throughput_delete < throughput ? throughput_delete : throughput;
	

	cout << "Tree=" << "ALEX" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total << "; " << endl;
}


int main(int argc, char** argv)
{
	string filename = argv[1];
    // string filename = "f_books";

    vector<uint64_t> data;
    load_bin<uint64_t>(filename, data);
	sort(data.begin(), data.end());
	data.erase( unique( data.begin(), data.end() ), data.end() );

	//cout << "File " << filename << "Datasize " << data.size() << endl;
	test_alex(data, filename);
}
