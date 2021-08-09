// CppOnly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib,"Ws2_32.lib")
#pragma once
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <iomanip>
#include <future>
#include <exception>
#include <cmath>
#include <condition_variable>
#include "template_class.h"

using namespace std;



void work(int &count, std::mutex &_lock)
{
	for (int i = 0; i < 1E6; i++)
	{
		std::lock_guard<std::mutex> _guard(_lock);
		//_lock.lock();  // locking just before the data update
		count++;
		//_lock.unlock(); //unlocking the data
	}
}

void threading()
{
	int count = 0;
	std::mutex _lock;

	/* //This approach is defining threads with their behavior, without locks, so doesn't guarantee data safety
	thread t1([&count, ITERATIONS](){
		for (int i = 0; i < ITERATIONS; i++)
			count++;
	});
	thread t2([&count, ITERATIONS]() {
		for (int i = 0; i < ITERATIONS; i++)
			count++;
	});
	
	t1.join();
	t2.join();
	*/

	//Or instead of defining as above, define as below

	/*auto func = [&]() {
		for (int i = 0; i < ITERATIONS; i++)
		{
			_lock.lock();
			count++;
			_lock.unlock();
		}
	};
	thread t3(func);  //using one declaration within multiple threads
	thread t4(func);
	t3.join();	
	t4.join();

	*/


	//thread trandom(work, count, _lock); // pass by value
	thread t5(work, std::ref(count), std::ref(_lock)); // calling fn in thread with parameters pass by reference
	thread t6(work, std::ref(count), std::ref(_lock));
	
	t5.join();
	t6.join();
	cout << count << endl;

	// To get the no of cores in system
	//	unsigned int c = std::thread::hardware_concurrency();
	//  std::cout << " number of cores: " << c << endl;;
}

//demo for callback function
class App
{
private:
	int count;
	std::mutex _lock;
public:
	App()
	{
		count = 0;
	}
	void operator()() //callback function - overloading the call operator
	{
		for (int i = 0; i < 1E6; i++)
		{
			const std::lock_guard<std::mutex> _guard(_lock);
			++count;
		}
	}

	int getcount()
	{
		return count;
	}
};

//to get the response back from thread
double calculatePi(int terms)
{
	double sum = 0.0;
	if (terms < 1)
	{
		throw runtime_error("terms can't be less than 1");
	}
	for (int i = 0; i < terms; i++)
	{
		int sign = pow(-1, i);
		double term = 1.0 / (i * 2 + 1);
		sum += sign * term;
	}
	return sum * 4;
}

int main(int argc, char *argv[])
{
	Blocking_Queue<int> _Store;



	condition_variable condition;


	//demo for packaged_task 
	packaged_task<double(int)> ptask(calculatePi);
	future<double> future1 = ptask.get_future();
	thread t8(move(ptask), 0);  //can use ref as well
	
	try {
		double res = future1.get();
		cout << setprecision(15) << res << endl;
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
	}

	t8.join();

	//explaination of promise and future and how they works to get value back from thread
	promise<double> promise;
	auto cal = [&](int terms) 
	{
		try {
			auto result = calculatePi(terms);
			promise.set_value(result);
		}
		catch (...)
		{
			promise.set_exception(current_exception());
		}		
	};

	thread tgetResultBack(cal , 1E6);
	future<double> future = promise.get_future();
	try
	{
		cout << std::setprecision(15) << future.get() << endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << endl;
	}
	
	tgetResultBack.join();
  	return 0;

	//cout << std::setprecision(15) << _future.get() << endl;
	//using callback in a thread
	App app;
	app();
	thread t1(std::ref(app));
	thread t2(std::ref(app));
	t1.join();
	t2.join();

	cout << app.getcount() << endl;
	
	threading();

	return 0;
}

