#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Student.h"

using namespace std;

int countLabs(string);

int main()
{
	//declare map, read roster and create it
	map<string, Student*> Students;
	string first, last, name;
	ifstream read("roster.txt");
	if (!read.is_open())
	{
		cout << "roster.txt did not open properly. Program ending.\n";
		return 0;
	}
	//else...
	while (!read.eof())
	{
		getline(read, last, '\t');
		if (read.eof()) break; //empty line at the end
		getline(read, first);
		name = last + ',' + first;
		Student* studentToAdd = new Student(first, last);
		Students[name] = studentToAdd;
	}
	read.close();

	//read grades into the map
	read.open("attempt.csv");
	if (!read.is_open())
	{
		cout << "attempt.csv did not open properly. Program ending.\n";
		return 0;
	}
	//else...
	string key, trash, grade;
	string totalTime;
	vector<double> labGrades;
	getline(read, key);//first line is trash
	int labCount = countLabs(key);
	while (!read.eof())
	{
		//this program is to be run on documents containing information about time spent on exercises
		getline(read, last, ',');
		getline(read, first, ',');
		getline(read, trash, ','); //gets email
		getline(read, trash, ','); //gets student ID
		getline(read, grade, ','); //gets grade
		getline(read, trash, ','); //participation total
		getline(read, trash, ','); //challenge total
		getline(read, trash, ','); //lab total
		for(int i = 0; i < labCount; ++i) //lab grades
			getline(read, trash, ',');
		getline(read, totalTime, ','); //gets total time spent on lab
		getline(read, trash); //finishes out the line

		if (first[first.length() - 2] == ' ') first = first.substr(0, first.length() - 2); //gets rid of middle initials

		name = last + ',' + first;
		if (name[0] == ',') break; //for empty line at the end

		if(stod(totalTime) > 0)
			Students[name]->tried = true; //will set this value to true so long as they are in the attempt.csv
		if (stod(grade) >= 75)
			Students[name]->passed = true;
	}
	read.close();

	//write output
	int passed = 0;
	int tried = 0;
	ofstream write("checkHere.txt");
	for (auto& elem : Students)
	{
		if (elem.second->last == "Berry")
			cout << "found him\n";
		write << elem.second->last << '\t' << elem.second->first << '\t';
		if (elem.second->passed)
		{
			write << "check here";
			++passed;
			++tried;
		}
		else if (elem.second->tried)
		{
			write << "\tthey tried";
			++tried;
		}
		write << endl;
	}

	cout << "Out of " << tried << " students who tried this unit there were " << passed << " who passed it.\n";
	write.close();
}

int countLabs(string key)
{
	string word = "";
	int count = 0;
	for (int i = 0; i < key.length(); ++i)
	{
		if (key[i] == ' ')
		{
			if (word == "Lab")
				++count;
			word = "";
		}
		else
			word += key[i];
	}
	return count / 2; 
}