#include<iostream>
#include<algorithm>
#include<iterator>
#include<time.h>

#include "parameters_flirt_mt.h"
#include "Util.hpp"

using namespace std;

template<class K> class Flirt;

/*LP Segments*/
template<class K>
class Segment
{
	int n;    //current number of keys
	int nDelete; //number of keys deleted from keys
	K keyStart; //starting key
	double slope;
	double slopeLow;  //Low Slope of Segment
	double slopeHigh; //High Slope of Segment

	vector<Key<K>> keys;   //Vector of keys
	Segment<K>* leftSibling = nullptr; //Pointer to the left sibling (leaf only)
	Segment<K>* rightSibling = nullptr; //Pointer to the right sibling (leaf only)
public:
	Segment(K keyStart, double slopeLow = 0, double slopeHigh = numeric_limits<double>::max()) {
		this->slopeLow = slopeLow;
		this->slopeHigh = slopeHigh;
		this->slope = 1;
		this->keyStart = keyStart;
		this->keys = { {keyStart,false} };
		this->n = 1;
		this->nDelete = 0;
	}

	bool search_segment(K key);
	Segment<K>* push_back(K key);
    void pop_front();

    friend class Flirt<K>;
};

/*
Search
*/
template<class K>
bool Segment<K>::search_segment(K key) {
	if (nDelete == n) {
		return false;
	}

	if (n == 0) {
		return (keys[0].key == key);
	}

	int pos = (key - keyStart) * slope;
	return binarySearchVectorKey(keys, pos - ERROR, pos + ERROR, key);
}

/*
Push Back
*/
template<class K>
Segment<K>* Segment<K>::push_back(K key) {

	if (key == keys[n - 1].key) { return nullptr; }
	if (key < keys[n - 1].key) {
		throw invalid_argument("Key must be largest to be appended");
	}

	//If less than error than just add to leaf
	if (slopeLow <= (double)(n) / (double)(key - keyStart) &&
		(double)(n) / (double)(key - keyStart) <= slopeHigh) {
		double slopeHighTemp = (double)((n + ERROR)) / (double)(key - keyStart);
		double slopeLowTemp = (double)((n - ERROR)) / (double)(key - keyStart);

		slopeHigh = min(slopeHigh, slopeHighTemp);
		slopeLow = max(slopeLow, slopeLowTemp);
		slope = (slopeHigh + slopeLow) / 2;
		Key<K> temp = { key,false };
		keys.push_back(temp);
		n++;
		return nullptr;
	}
	//Else create new segment
	else {
		Segment<K>* segPtr = new Segment<K>(key);
		rightSibling = segPtr;
		segPtr->leftSibling = this;
		return segPtr;
	}
}

/*
Pop Front
*/
template<class K>
void Segment<K>::pop_front(){
    keys[nDelete++].deleted = true;
}


/*local FLIRT for each partition*/
template<class K>
class Flirt {
private:
    pair<K,Segment<K>*>* queue; //queue which is the SummaryList

    Segment<K>* first = nullptr; //first segment in queue
    Segment<K>* last = nullptr; //last segment in queue

    int first_index;  //front index
    int last_index;  //last index
    int capacity; //capacity
    int n; // current size of the queue
    int nKeys; //Number of keys in queue

    K first_key; //First key in Flirt
    K last_key; //Last key in Flirt

public:
    Flirt(int size)
    {
        queue = new pair<K,Segment<K>*>[size];
        capacity = size;
        first_index = 0;
        last_index = 0;
        n = 0;
    };
    ~Flirt()
    {
        if (n != 0)
        {
            Segment<K>* temp = first->rightSibling;
            delete first;
            while (temp != NULL)
            {
                Segment<K>* temp2 = temp->rightSibling;
                delete temp;
                temp = temp2;
            }
        }
        delete queue;
        queue = nullptr;
    };

    int get_no_keys(){return nKeys;}

    void enqueue(K key);
    void dequeue();

    bool linearSearch(K target, int left, int right);
    bool binarySearch(K target, int left, int right);
    bool search(K target);
    bool search_forced(K target);

    void printQueue();

};

/*
Enqueue 
*/
template<class K>
void Flirt<K>::enqueue(K key)
{
    if (n == capacity)
    {
        cout << "The queue is full" << endl;
        return;
    }

    if (n == 0){
        Segment<K>* segPtr = new Segment<K>(key);
        queue[n] = make_pair(key,segPtr);
        first = segPtr;
        last = segPtr;
        n++;
        nKeys++;
        first_key = key;
        last_key = key;
        return;
    }

    Segment<K>* newSeg = last->push_back(key);
    nKeys++;
    if (newSeg != NULL){
        last = newSeg;
        queue[n] = make_pair(key,newSeg);
        last_index++;
        n++;
    }
    last_key = key;
    return;
}

/*
Dequeue 
*/
template<class K>
void Flirt<K>::dequeue()
{
    if(n == 0)
    {
        cout << "The queue is empty" << endl;
    }
    
    if (first->nDelete == first->n-1){
        if (first == last){
            first_index = 0;
            last_index = 0;
            n = 0;
            delete first;
            first = nullptr;
            last = nullptr;
            return;
        }
        first_index ++;
        n--;
        Segment<K>*  temp = first->rightSibling;
        delete first;
        first = temp;
        first->leftSibling = nullptr;
        first_key = temp->keyStart;
        return;
    }

    first->pop_front();
    return;
}

/*
Print Content of Queue
*/
template<class K>
void Flirt<K>::printQueue()
{
    
    // Print first key
    for(int i = 0; i < n; i++)
    {
        cout << queue[i].first << "\t";
    }
    cout << endl;


    // //Print all keys
    // for(int i = 0; i < n; i++)
    // {
    //     for (auto &keys: queue[i].second->keys){
    //         if (!keys.deleted){
    //             cout << keys.key << " ";
    //         }
    //     }
    //     cout << " | ";
    // }
    // cout << endl;
}

/*
Linear Search Method used in Search Operation
For finding the segment in SummaryList
*/
template<class K>
bool Flirt<K>::linearSearch(K target, int left, int right)
{
    int index = left;
    while (index < right && queue[index+1].first <= target) {index++;}
    return queue[index].second->search_segment(target);
}

/*
Binary Search Method used in Search Operation
For finding the segment in SummaryList
*/
template<class K>
bool Flirt<K>::binarySearch(K target, int left, int right)
{
    
    //edge case (left  boundary)
    if (target < queue[left+1].first)
    {
        if (queue[left].first <= target)
        {
            return queue[left].second->search_segment(target);
        }
        else
        {
            return false;
        }
    }

    //edge case (right boundary)
    if (target >= queue[right-1].first)
    {
        if (target < queue[right].first)
        {
            return queue[right-1].second->search_segment(target);
        }
        else
        {
            return queue[right].second->search_segment(target);
        }
    }

    while(left <= right)
    {
        int mid = (left + right) / 2;
        if (queue[mid].first == target)
        {
            return queue[mid].second->search_segment(target);
        }
        if (queue[mid].first < target)
        {
            if (queue[mid-1].first >= target)
            {
                return queue[mid-1].second->search_segment(target);  
            }
            left = mid + 1;
        }
        else
        {
            if (queue[mid+1].first < target)
            {
                return queue[mid].second->search_segment(target);  
            }

            right = mid - 1;
        }
    }

    return false;
}

/*
Search
*/
template<class K>
bool Flirt<K>::search(K target)
{
    if (n == 0)
    {
        // cout << "Empty Queue" << endl;
        return false;
    }

    if (n < 256)
    {
        return linearSearch(target,first_index,last_index);
    }

    return binarySearch(target,first_index,last_index);
}

/*
Search Method for testing throughput of system.
Same random key method as flirt_mt.
*/
template<class K>
bool Flirt<K>::search_forced(K target)
{
    
    K rand_key =  0.5*(last_key+first_key) + target;
    
    if (n == 0)
    {
        // cout << "Empty Queue" << endl;
        return false;
    }

    if (n < 256)
    {
        return linearSearch(rand_key,first_index,last_index);
    }

    return binarySearch(rand_key,first_index,last_index);
}