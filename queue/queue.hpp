//
// Created by lia 梁 on 2021/8/5.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>


using namespace std;

template<class K>
class Queue {

private:

    K * m_arr;

    K m_front;
    K m_rear;
    int m_capacity;
    int m_n; // current size of the queue

public:

    explicit Queue(int size);
    ~Queue();

    int getCurrentSize();
    int getCapacity();
    bool isEmpty();
    bool isFull();

    void enqueue(K newKey);
    void dequeue();

    //int linearSearch(K target, int left, int right);
    int binarySearch(K target, int left, int right);
    int search(K target);

    void printQueue();

    size_t get_model_size_in_bytes();
    size_t get_total_size_in_bytes();

};

template<class K>
Queue<K>::Queue(int size)
{
    m_arr = new K[size];
    m_capacity = size;
    m_front = 0;
    m_rear = -1;
    m_n = 0;

    //cout << "Constructing the queue" << endl;
}

template<class K>
Queue<K>::~Queue<K>()
{
    //cout << "Deconstructing the queue" << endl;
};

template<class K>
int Queue<K>::getCurrentSize()
{
    return m_n;
}

template<class K>
size_t Queue<K>::get_model_size_in_bytes()
{
    return sizeof(int)*2 + sizeof(K)*2 + sizeof(K*);
}

template<class K>
size_t Queue<K>::get_total_size_in_bytes()
{
    return sizeof(int)*2 + sizeof(K)*(2+m_n) + sizeof(K*);
}

template<class K>
int Queue<K>::getCapacity()
{
    return m_capacity;
}

template<class K>
bool Queue<K>::isEmpty()
{
    return (this->getCurrentSize() == 0);
}

template<class K>
bool Queue<K>::isFull()
{
    return (this->getCurrentSize() == m_capacity);
}

template<class K>
void Queue<K>::enqueue(K newKey)
{
    if (this->isFull())
    {
        cout << "The queue is full" << endl;
        return;
    }

    m_rear++;
    m_rear %= m_capacity;
    m_arr[m_rear] = newKey;
    m_n++;
}

template<class K>
void Queue<K>::dequeue()
{
    if(isEmpty())
    {
        cout << "The queue is empty" << endl;
    }

    m_front ++;
    m_front %= m_capacity;
    m_n--;
}


template<class K>
void Queue<K>::printQueue()
{
    for(int i = 0; i < getCapacity(); i++)
    {
        cout << m_arr[i] << "\t";
    }
    cout << endl;
}

template<class K>
int Queue<K>::binarySearch(K target, int left, int right)
{
    while(left <= right)
    {
        int mid = (left + right) / 2;
        if (m_arr[mid] == target)
        {
            return mid;
        }
        if (m_arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return 0;
}

template<class K>
int Queue<K>::search(K target)
{
    if(target > m_arr[m_capacity-1]) // not in second half arr
    {
        // cout << m_arr[m_capacity-1];
        if (target <= m_arr[m_rear]) // maybe in first half arr
            {
            //return linearSearch(target, 0, m_rear);
            return binarySearch(target, 0, m_rear);
            }
        else // not in first half
        {
            return 0;
        }
    }
    else // maybe in second half arr
    {
        //return linearSearch(target, m_rear+1, m_capacity-1);
        return binarySearch(target, m_rear+1, m_capacity-1);
    }
}

//template<class K>
//int Queue<K>::linearSearch(K target, int left, int right)
//{
//    for(int i = left;  i <= right; i++)
//    {
//        if (m_arr[i] == target)
//        {
//            return i;
//        }
//    }
//    return 0;
//}


//        if (m_arr[left] < m_arr[mid])
//        {
//            cout << "all value in the first half of the array are sorted" << endl;
//        }
//        if (m_arr[mid] < m_arr[right])
//        {
//            cout << "all value in the second half of the array are sorted" << endl;
//        }