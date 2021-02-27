#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Student
{
public:
	string fName;
	string lName;
	string name;

	vector<string> joined; 
	//vector<double> joinedMinutesFromLabStart;
	vector<string> left; //this won't be used for anything; it's a holdover from an older version
	vector<int> differences; //this won't be used for anything; it's a holdover from an older version
	int totTimeInLab = 0; //this won't be used for anything; it's a holdover from an older version
	bool joinedOnTime = 0;
	bool reconvenedOnTime = 0;
	bool attendedLab = 0;

	Student(string);
};

