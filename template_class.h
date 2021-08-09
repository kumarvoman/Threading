#pragma once
#include <iostream>
#include <queue>
using namespace std;

template <typename mytype>
class Blocking_Queue
{
private:
	queue<mytype> _store;
public:
	void push(mytype e)
	{
		_store.push(e);
	}

	mytype pop()
	{
		
	}

	//mytype operator = (mytype const &a)
	//{
	//	return a;
	//};
};


