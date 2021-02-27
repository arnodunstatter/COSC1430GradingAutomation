#include "Student.h"


Student::Student(ifstream& read)
{
	string temp;
	double grade;

	//now we get the data
	getline(read, lName, ',');
	getline(read, fName, ',');
	name = lName + "," + fName;
	if (name == ",")
		return;
	getline(read, email, ',');
	getline(read, ID, ',');
	getline(read, temp, ',');
	grade = stod(temp);
	getline(read, temp); //finishes out the line, moving read-stream marker to next line for next student

	if (grade >= 75)
		passedExercises = 1;

}