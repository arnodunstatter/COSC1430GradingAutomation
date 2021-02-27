#pragma once
#include <string>
using namespace std;
class Student
{
public:
	string first, last;
	bool passed = false;
	bool tried = false;

	Student(string f, string l)
		:first(f), last(l) {};
	~Student()
	{
		delete this;
	}
};

