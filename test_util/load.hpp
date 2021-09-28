#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*
Data loading functions
*/
using namespace std;

template<typename T>
void load_bin(string filenmane, vector<T> &v)
{
    ifstream ifs(filenmane, ios::in | ios::binary);

    if (!ifs.is_open())
    {
        cout << "Fail to open\n" << endl;
    }

    T value;
    T size;

    ifs.read(reinterpret_cast<char*>(&size), sizeof(T));
    v.resize(size);
    ifs.read(reinterpret_cast<char*>(v.data()), size * sizeof(T));

    ifs.close();
}

template<typename T>
void printV(vector<T> &v)
{
    for(typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << endl;
    }
}
