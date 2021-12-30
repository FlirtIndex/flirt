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

#include "parameters_perf.h"
#include "load.hpp"

/*
Test Script for index size
testing the index size for each method
*/
using namespace std;

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

	for(typename vector<T>::iterator it = data.begin(); it != data.begin()+WINDOWSIZE; it++)
    {
		t.insert(make_pair(*it,true));
	}

    size_t index_size = sizeof(t) + sizeof(T)* WINDOWSIZE + sizeof(bool) * WINDOWSIZE; //Incorrect

	cout << "Tree=" << "BP" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE <<  "; DataSize=" << sizeof(T)*WINDOWSIZE << "; IndexSize=" << index_size << "; " << endl;
}


template<typename T>
void test_PGM(vector<T> &data, string filename)
{
	vector<pair<T,bool>> data_window(WINDOWSIZE);
	for (int i = 0; i < WINDOWSIZE; i++){
		data_window[i] = make_pair(data[i],false);
	}

	auto first = data_window.begin();
	auto last = data_window.end();

	pgm::DynamicPGMIndex<T,bool,pgm::PGMIndex<T,FANOUT>> t(first,last);

	size_t index_size = t.size_in_bytes() - sizeof(T)*WINDOWSIZE;

	cout << "Tree=" << "PGM" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << FANOUT
	    << "; WindowSize=" << WINDOWSIZE << "; DataSize=" << sizeof(T)*WINDOWSIZE << "; IndexSize=" << index_size << "; " << endl;
}

template<typename T>
void test_alex(vector<T> &data, string filename)
{
	
    alex::Alex<T, int> index;

    int cnt = 0;

	for(typename vector<T>::iterator it = data.begin(); it != data.begin()+WINDOWSIZE; it++)
    {
		index.insert(*it,cnt);
	}

    size_t index_size = index.model_size();

	cout << "Tree=" << "ALEX" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; DataSize=" << sizeof(T)*WINDOWSIZE << "; IndexSize=" << index_size << "; " << endl;
	
}

template<typename T>
void test_flirt(vector<T> &data, string filename)
{

    Flirt<uint64_t> flirt(MAXSEG);

	for(typename vector<T>::iterator it = data.begin(); it != data.begin()+WINDOWSIZE; it++)
    {
		flirt.enqueue(*it);
	}

    size_t index_size = flirt.get_model_size_in_bytes();

	cout << "Tree=" << "Flirt" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; DataSize=" << sizeof(T)*WINDOWSIZE << "; IndexSize=" << index_size << "; " << endl;
}

template<typename T>
void test_queue(vector<T> &data, string filename)
{
    Queue<T> q(WINDOWSIZE); // the capacity is the window size

	for(typename vector<T>::iterator it = data.begin(); it != data.begin()+WINDOWSIZE; it++)
    {
		q.enqueue(*it);
	}

    size_t index_size = q.get_model_size_in_bytes();

	cout << "Tree=" << "Queue" << "; Data=" << filename << "; Error="  << -1 << "; Fanout=" << -1
	    << "; WindowSize=" << WINDOWSIZE << "; DataSize=" << sizeof(T)*WINDOWSIZE << "; IndexSize=" << index_size << "; " << endl;
	
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
