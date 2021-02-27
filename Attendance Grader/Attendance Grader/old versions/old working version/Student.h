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
	string email = "";
	string ID = "(mis-match-name-error)";
	bool passedExercises = 0;


	vector<string> joined;
	vector<string> left;
	vector<int> differences;
	int totTimeInClass = 0;
	bool attendedClass = 0;

	bool creditGiven = 0;

	Student(ifstream& read);

};

