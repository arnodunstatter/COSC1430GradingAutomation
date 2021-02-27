#include "Student.h"

Student::Student(ifstream& read)
{
	getline(read, lName, '\t');
	getline(read, fName);
	name = lName + ',' + fName;
}

Student::Student(string name) //name is in format "last,first"
{
	fName = name.substr(name.find(',')+1);
	lName = name.substr(0, name.length() - fName.length() - 1);
}