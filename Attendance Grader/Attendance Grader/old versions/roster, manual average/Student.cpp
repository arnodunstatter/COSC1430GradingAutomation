#include "Student.h"


Student::Student(ifstream& read)
{
	getline(read, lName, '\t');
	getline(read, fName);
	name = lName + ',' + fName;
}

void Student::gradeLabs()
{
	double labsTotal = 0;
	for (int i = 0; i < labGrades.size(); ++i)
		labsTotal += labGrades[i];
	double average = labsTotal / labGrades.size();

	if (average >= 75)
		passedExercises = 1;
}