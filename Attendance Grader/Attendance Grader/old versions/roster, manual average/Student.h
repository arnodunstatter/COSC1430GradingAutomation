#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
class Student
{
public:
	string fName = "";
	string lName = "";
	string name = "";
	
	
	bool passedExercises = 0;
	vector<double> labGrades;
	double labAverage;
	vector<string> joined;
	vector<string> left;
	vector<int> differences;
	int totTimeInClass = 0;
	bool attendedClass = 0;
	bool creditGiven = 0;

	Student(ifstream& read);
	void gradeLabs();

};

