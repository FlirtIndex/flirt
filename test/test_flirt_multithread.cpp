#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <boost/thread/barrier.hpp>

#include "parameters_flirt_mt.h"
#include "load.hpp"
#include "flirt_multithread.hpp"

/*
Test Script for Flirt multithread
*/
using namespace std;

bool RUN_FLAG = false;

const int no_threads = NO_THREADS;
const int local_capacity = WINDOWSIZE/(no_threads-1); //Assume Whole Number for now

int enqueue_thread = no_threads-1;
int dequeue_thread = (enqueue_thread + 1)%no_threads;

int start_array[no_threads] = {0};

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<double> fsec;

fsec duration;

// mutex enqueue_lock;
// condition_variable enqueue_cv;
// bool enqueue_done = false;

boost::barrier checkpoint(2); 

mutex count_time_lock;
long long total_count = 0;
clock_t total_time;

bool change_duration = true;

int current_pos = WINDOWSIZE;

void run_partitioned_queue(int id, vector<uint64_t>& data)
{ 
    Flirt<uint64_t> flirt(local_capacity); //Number of maximum segments

    if(id != enqueue_thread)
    {
        vector<uint64_t> data_temp = vector<uint64_t>(data.begin()+local_capacity*(id),data.begin()+local_capacity*(id+1));
        // cout << id << ": " << data_temp.front() << " , " << data_temp.back() << endl;
        for(typename vector<uint64_t>::iterator it = data_temp.begin(); it != data_temp.end(); it++)
        {
            flirt.enqueue(*it);
        }
    }
    // else
    // {
    //     cout << id << endl;
    // }
    
    start_array[id] = 1;
    int sum = 0;
    do
    {
        sum = 0;
        for(auto& num : start_array)
        {
            sum += num;
        }
    }while(sum != no_threads);
    
    // cout << id << " : Working"<< endl;

    while(true)
    {
        if(RUN_FLAG)
        {
            break;
        }
    }

    int cnt = 0;
    // clock_t t_start = clock();
    auto t0 = Time::now();
    while(RUN_FLAG)
    {
        if (id == enqueue_thread)
        {           
            // printf("enqueue id: %d \n",id);
            // unique_lock<mutex> lock(enqueue_lock);
            for (int i = 0; i < local_capacity; i++)
            {
                flirt.enqueue(data.at(current_pos));
                flirt.search_forced(cnt*no_threads);
                current_pos++;
                cnt++;
            }
            
            int temp = enqueue_thread;
            temp++;
            temp%= no_threads;
            enqueue_thread = -1;

            checkpoint.wait();
            enqueue_thread = temp;

            // enqueue_thread ++;
            // enqueue_thread %= no_threads;
            // enqueue_done = true;
            // lock.unlock();
            // enqueue_cv.notify_one();

            // cnt++;
            
        }
        else if (id == dequeue_thread)
        {
            // printf("dequeue id: %d \n",id);
            for (int i = 0; i < local_capacity; i++)
            {
                flirt.search_forced(cnt*no_threads);
                flirt.dequeue();
                cnt++;
            }

            checkpoint.wait();
            dequeue_thread ++;
            dequeue_thread %= no_threads;

            // unique_lock<mutex> lock(enqueue_lock);
            // while (!enqueue_done) {enqueue_cv.wait(lock);}

            // cnt++;
            
        }
        else
        {
            flirt.search_forced(cnt*no_threads);
            cnt++;
        }

        if (current_pos - WINDOWSIZE > NO_EXECUTION) 
        {
            // cout<< cnt << endl;
            break;
        }
    }

    auto t1 = Time::now();

    count_time_lock.lock();
    // total_time = clock()-t_start;
    if (change_duration)
    {
        duration = t1 - t0;
        change_duration = false;
    }
    total_count += cnt;
    count_time_lock.unlock();

    // cout << "id: " << id << " ,cnt: " << cnt << " ,current_pos: "<< current_pos << endl;
    // printf("id: %d, cnt: %d, current_pos: %d \n",id,cnt,current_pos);

}

void placeholder_func(){this_thread::sleep_for(chrono::microseconds(1));}

int main(int argc, char** argv)
{

    string filename = argv[1];
    //string filename = "f_books";
    // string filename = "/Users/admin/Desktop/data/f_books";

    vector<uint64_t> data;
    load_bin<uint64_t>(filename, data);
	sort(data.begin(), data.end());
	data.erase( unique( data.begin(), data.end() ), data.end());


    thread threads[no_threads];
    for (int i=0; i <no_threads; i++)
    {
        // threads[i] = thread(placeholder_func);
        threads[i] = thread(run_partitioned_queue,i,ref(data));
    }

    RUN_FLAG = true;

    //Clean up
    for (auto& t:threads)
    {
        t.join();
    }

    // long long th_search = (total_count)/(((double)total_time)/CLOCKS_PER_SEC);
    // long long th_update = (NO_EXECUTION)/(((double)total_time)/CLOCKS_PER_SEC);

    long long th_search = total_count/duration.count();
    long long th_update = NO_EXECUTION/duration.count();
    // double time_search = duration.count()/total_count;
    // double time_update = duration.count()/NO_EXECUTION;

    cout << "Tree=" << "ParrallelFlirt" << "; Data=" << filename << "; Error="  << ERROR << "; Fanout=" << -1
    << "; WindowSize=" << WINDOWSIZE << "; Enqueue=" << th_update << "; Search=" << th_search 
    << "; Dequeue=" << th_update  << "; no_threads=" << NO_THREADS << "; " << endl;
    // << "; UpdateUnitTime=" << time_update << "; SearchUnitTime=" << time_search << "; " << endl;

    // cout << total_count << "\n";
    
    // cout << ((double)total_time)/CLOCKS_PER_SEC/(NO_EXECUTION) << "\n";

}