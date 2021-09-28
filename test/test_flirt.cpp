#include<iostream>
#include<vector>
#include <algorithm>
#include <chrono>
#include "load.hpp"
#include "flirt.hpp"

/*
Test Script for Flirt.
*/
using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

const int maxExecution = WINDOWSIZE + NO_EXECUTION; 

template<typename T>
void test_flirt(vector<T> &data, string filename)
{

    Flirt<uint64_t> flirt(MAXSEG);

	int cnt = 0;
    int pos = 0;
	fsec t_insert;
	fsec t_delete;
	fsec t_search;

    for(typename vector<uint64_t>::iterator it = data.begin(); it != data.end(); it++)
    {
		cnt++;
        pos++;
		int minT = 0;
		int maxT = 0;

		if( cnt < WINDOWSIZE)
        {
            flirt.enqueue(*it);
            
			// if (flirt.get_n() >= maxSegment)
			// {
			// 	maxSegment = flirt.get_n();
			// }
        }
		else
		{
    
			auto t0 = Time::now();
			flirt.enqueue(*it);
			auto t1 = Time::now();
			t_insert += t1-t0;

			// if (flirt.get_n() >= maxSegment)
			// {
			// 	maxSegment = flirt.get_n();
			// }

			auto t2 = Time::now();
			flirt.dequeue();
			auto t3 = Time::now();
			t_delete += t3-t2;
			
			// if (flirt.get_n() >= maxSegment)
			// {
			// 	maxSegment = flirt.get_n();
			// }

            minT = pos - WINDOWSIZE;
			maxT = pos;

			int target =  minT + (rand() % ( maxT - minT + 1 ));
			T search_key = data.at(target);

			auto t4 = Time::now();
			flirt.search(search_key);
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
	
	cout << "Tree=" << "Flirt" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total << "; " << endl;
		//<< "; MaxSegment=" << MAXSEG << "; " << endl;
	// cout << "Complete" << endl;

}



int main(int argc, char** argv)
{
	
	string filename = argv[1];

	// cout << "Tree=" << "Flirt" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << 4
	//     << "; WindowSize=" << WINDOWSIZE << "; MaxSegment=" << MAXSEG << "; " << endl;

    vector<uint64_t> data;
    load_bin<uint64_t>(filename, data);
	sort(data.begin(), data.end());
	data.erase( unique( data.begin(), data.end() ), data.end() );

	test_flirt(data, filename);

}

