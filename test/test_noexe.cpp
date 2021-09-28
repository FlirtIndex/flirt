#include<iostream>
#include<vector>
#include<iterator>
#include <algorithm>
#include <chrono>

#include "btree.h"
#include "btree_map.h"
#include "morton_nd.hpp"
#include "pgm_index.hpp"
#include "pgm_index_dynamic.hpp"
#include "pgm_index_variants.hpp"
#include "piecewise_linear_model.hpp"
#include "alex.h"
#include "queue.hpp"

#include "flirt.hpp"

#include "parameters_noexe.h"
#include "load.hpp"
#include "PerfEvent.hpp"

/*
Test Script for varying number of executions.
REMEMBER TO CHANGE THE INCLUDE FILE IN FLIRT (uncomment #include "parameters_noexe.h" and comment out others)
*/
using namespace std;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

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

const int maxExecution = WINDOWSIZE + NO_EXECUTION;

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
	

	cout << "Tree=" << "BP" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total
        << "; NoExecution=" << NO_EXECUTION << "; " << endl;
}


template<typename T>
void test_PGM(vector<T> &data, string filename)
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
	

	cout << "Tree=" << "PGM" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE <<"; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total
        << "; NoExecution=" << NO_EXECUTION << "; " << endl;
}

template<typename T>
void test_alex(vector<T> &data, string filename)
{
    alex::Alex<T, int> index;

	int cnt = 0;

	fsec t_insert;
	fsec t_delete;
	fsec t_search;

	for(typename vector<T>::iterator it = data.begin(); it != data.begin()+WINDOWSIZE; it++)
    {
		index.insert(*it,cnt);
	}

	for(typename vector<T>::iterator it = data.begin()+WINDOWSIZE; it != data.end(); it++)
    {
		auto t0 = Time::now();
		index.insert(*it,cnt);
		auto t1 = Time::now();
		t_insert += t1-t0;

		T first_key = data.at(cnt);

		auto t2 = Time::now();
		index.erase(first_key);
		auto t3 = Time::now();
		t_delete += t3-t2;

		cnt++;

		int minT = cnt;
		int maxT = cnt+WINDOWSIZE;

		int target =  minT + (rand() % ( maxT - minT + 1 ));
		uint64_t search_key = data.at(target);
		auto t4 = Time::now();
		index.find(search_key);
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
	

	cout << "Tree=" << "ALEX" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total
        << "; NoExecution=" << NO_EXECUTION << "; " << endl;
}

template<typename T>
void test_flirt(vector<T> &data, string filename)
{

    Flirt<uint64_t> flirt(MAXSEG);

	int cnt = 0;
	fsec t_insert;
	fsec t_delete;
	fsec t_search;

	for(typename vector<uint64_t>::iterator it = data.begin(); it != data.end(); it++)
    {
		cnt++;
		int minT = 0;
		int maxT = 0;

		if( cnt < WINDOWSIZE)
        {
            flirt.enqueue(*it);
            
        }
		else
		{
			auto t0 = Time::now();
			flirt.enqueue(*it);
			auto t1 = Time::now();
			t_insert += t1-t0;

			auto t2 = Time::now();
			flirt.dequeue();
			auto t3 = Time::now();
			t_delete += t3-t2;
			

            minT = cnt - WINDOWSIZE;
			maxT = cnt;

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
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total
        << "; NoExecution=" << NO_EXECUTION << "; " << endl;
}

template<typename T>
void test_queue(vector<T> &data, string filename)
{
    Queue<uint64_t> q(WINDOWSIZE); // the capacity is the window size

	int cnt = 0;
	fsec t_insert;
	fsec t_delete;
	fsec t_search;

	for(typename vector<uint64_t>::iterator it = data.begin(); it != data.end(); it++)
    {
		cnt++;
		int minT = 0;
		int maxT = 0;

		if( cnt < WINDOWSIZE)
        {
            q.enqueue(*it);
            
        }
		else
		{
            auto t0 = Time::now();
			q.enqueue(*it);
			auto t1 = Time::now();
			t_insert += t1-t0;


			auto t2 = Time::now();
			q.dequeue();
			auto t3 = Time::now();
			t_delete += t3-t2;


            minT = cnt - WINDOWSIZE;
			maxT = cnt;

			int target =  minT + (rand() % ( maxT - minT + 1 ));
			T search_key = data.at(target);

            auto t4 = Time::now();
			q.search(search_key);
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
	
	cout << "Tree=" << "Queue" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << t_avg_insert << "; Search=" << t_avg_search 
		<< "; Dequeue=" << t_avg_delete << "; Throughput=" << throughput << "; TotalTime=" << t_total 
        << "; NoExecution=" << NO_EXECUTION << "; " << endl;
}


int main(int argc, char** argv)
{
	string filename = argv[1];

    vector<uint64_t> data;
    load_bin<uint64_t>(filename, data);
	sort(data.begin(), data.end());
	data.erase( unique( data.begin(), data.end() ), data.end() );

	if (RUNFLAG == 0)
	{
		test_flirt(data, filename);
	}
	else if (RUNFLAG == 1)
	{
		test_BPtree(data, filename);
	}
	else if (RUNFLAG == 2)
	{
		test_PGM(data, filename);
	}
	else if (RUNFLAG == 3)
	{
		test_alex(data,filename);
	}
	else if (RUNFLAG == 4)
	{
		test_queue(data,filename);
	}
}
