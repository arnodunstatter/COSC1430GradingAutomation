#include "Student.h"

Student::Student(ifstream& read)
{
	getline(read, lName, '\t');
	getline(read, fName);
	name = lName + ',' + fName;
}

