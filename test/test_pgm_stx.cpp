#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm>
#include <chrono>

#include "btree.h"
#include "btree_map.h"
#include "morton_nd.hpp"
#include "pgm_index.hpp"
#include "pgm_index_dynamic.hpp"
#include "pgm_index_variants.hpp"
#include "piecewise_linear_model.hpp"

#include "parameters_bl.h"
#include "load.hpp"
#include "Util.hpp"

/*
Test Script for PGM and STX B+ Tree.
*/
using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

const int maxExecution = WINDOWSIZE + NO_EXECUTION; 

//STX B+ Tree attributes
template<typename Type>
class btree_traits_fanout{
public:
    static const bool selfverify = false;
    static const bool debug = false;
    static const int leafslots = FANOUT;
    static const int innerslots = FANOUT;
    static const size_t binsearch_threshold = 256;
};

template<typename T>
void test_BPtree(vector<T> &data, string filename)
{
	stx::btree_map<T,bool,less<T>, btree_traits_fanout<T>> t;

	int cnt = 0;

	fsec t_insert;
	fsec t_delete;
	fsec t_search;

    for(typename vector<T>::iterator it = data.begin(); it != data.end(); it++)
    {
		cnt++;
		int minT = 0;
		int maxT = 0;

		if( cnt <= WINDOWSIZE){t.insert(make_pair(*it,true)); }
		else
		{	
			auto input_kv = make_pair(*it,true);
			auto t0 = Time::now();
			t.insert(input_kv);
			auto t1 = Time::now();
			t_insert += t1-t0;

			T first_key = (*t.begin()).first;
			auto t2 = Time::now();
			t.erase(first_key);
			auto t3 = Time::now();
			t_delete += t3-t2;

			minT = cnt - WINDOWSIZE;
		 	maxT = cnt;
			int target =  minT + (rand() % ( maxT - minT + 1 ));

			T search_key = data.at(target);
			auto t4 = Time::now();
			t.find(search_key);
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
	

	cout << "Tree=" << "BP" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total << "; " << endl;
}


template<typename T>
void test_pgm(vector<T> &data, string filename)
{
	int cnt = 0;

	fsec t_insert;
	fsec t_delete;
	fsec t_search;

	vector<pair<T,bool>> data_window(WINDOWSIZE);
	for (int i = 0; i < WINDOWSIZE; i++){
		data_window[i] = make_pair(data[i],false);
	}

	auto first = data_window.begin();
	auto last = data_window.end();

	pgm::DynamicPGMIndex<T,bool,pgm::PGMIndex<T,FANOUT>> t(first,last);

    for(typename vector<T>::iterator it = data.begin() + WINDOWSIZE; it != data.end(); it++)
    {
		int minT = 0;
		int maxT = 0;

		auto t0 = Time::now();
		t.insert_or_assign(*it,false);
		auto t1 = Time::now();
		t_insert += t1-t0;

		T first_key = data.at(cnt);
		auto t2 = Time::now();
		t.erase(first_key);
		auto t3 = Time::now();
		t_delete += t3-t2;

		cnt++;

		minT = cnt;
		maxT = cnt+WINDOWSIZE;
		int target =  minT + (rand() % ( maxT - minT + 1 ));

		T search_key = data.at(target);
		auto t4 = Time::now();
		t.find(search_key);
		auto t5 = Time::now();
		t_search += t5-t4;

		if (cnt == NO_EXECUTION){ break; }
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
	

	cout << "Tree=" << "PGM" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE <<"; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total << "; " << endl;
}

int main(int argc, char** argv)
{
	
	string filename = argv[1];
    //string filename = "f_books";

    vector<uint64_t> data;
    load_bin<uint64_t>(filename, data);
	sort(data.begin(), data.end());
	data.erase( unique( data.begin(), data.end() ), data.end() );

	//cout << "File " << filename << "Datasize " << data.size() << endl;
	if (ERROR == 4)
	{
		test_BPtree(data, filename);
		test_pgm(data,filename);
	}
}