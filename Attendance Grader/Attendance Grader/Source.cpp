#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "Student.h"

using namespace std;

int countLabs(string);

int main()
{
	//Instructions for use:
	//Download the exercise results as a .csv
	//Download the attendance from teams, open it and save-as .csv, then open the .csv with notebook and save it as a .txt with ANSI encoding.



	//we will need a map of students
	map<string, Student*> Students; //key is the student's full name, value is the student object
	ifstream read("roster.txt");
	if (!read.is_open())
	{
		cout << "roster did not open correctly.\n";
		return 0;
	}
	//else...
	while (!read.eof())
	{
		Student* temp = new Student(read);
		if(temp->name != ",")
			Students[temp->name] = temp;
	}
	read.close();

	read.open("problems.csv");
	if (!read.is_open())
	{
		cout << "problems did not open correctly.\n";
		return 0;
	}
	//else...
	string line;
	getline(read, line); //this will just get the first line of garbage
	int numLabs = countLabs(line);
	while (!read.eof())
	{
		string temp;
		double grade;
		string lName, fName, name;

		//now we get the data
		getline(read, lName, ',');
		getline(read, fName, ',');
		if (fName == "")
			continue;
		if (fName[fName.length() - 2] == ' ') //gets rid of middle initial
			fName = fName.substr(0, fName.length() - 2);

		name = lName + "," + fName;
		
		getline(read, temp, ','); //gets the email address (trash)
		getline(read, temp, ','); //gets the student ID (trash)
		getline(read, temp, ','); //gets total (which we believe is the average) ***********************this may be a false assumption and a source of a bug later on
		grade = stod(temp);
		getline(read, temp); //finishes out the line

		if (grade >= 75)
			Students[name]->passedExercises = 1;
	}

	//for (auto& el1 : Students)
	//{
	//	cout << el1.first << " " << el1.second->passedExercises << endl;
	//}

	read.close();

	read.open("attendance.csv");
	
	if (!read.is_open())
	{
		cout << "attendance did not open correctly.\n";
		return 0;
	}
	//else...

	while (!read.eof())
	{
		string first, last, name, keyword, time, trash;
		getline(read, last, ','); //gets the string ""Abdallah"
		if (last.substr(0, 31) == "Full Name\tUser Action\tTimestamp")
			last = last.substr(32);
		if (last == "")
			continue;
		while(!isalpha(last[0])) //this loop removes the extra characters before the beginning of the last name
			last = last.substr(1, last.length() - 1); //edits it to "Abdallah"
		getline(read, first, '\t'); //gets the string " Zachery"
		while (!isalpha(first[0]))
			first = first.substr(1, first.length() - 1); //edits it to "Zachery"
		while (!isalpha(first[first.length() - 1])) //shaves off the "\"" at the end
			first = first.substr(0, first.length() - 1);
		if (first[first.length() - 2] == ' ') //gets rid of middle initial
			first = first.substr(0, first.length() - 2);
		name = last + "," + first;
		getline(read, keyword, '\t');
		getline(read, trash, ' ');
		getline(read, time, ' ');
		if (time.length() == 7)
			time = time.substr(0, 4); //converts "1:00:00" to "1:00"
		else
			time = time.substr(0, 5);
		getline(read, trash);
		

		//now we update our the student's entry in the map
		//first we have to make sure they were even present in the problems data
		if (Students.find(name) != Students.end())
		{
			if (keyword == "Left")
				Students[name]->left.push_back(time);
			else
				Students[name]->joined.push_back(time);
		}
		//else
			//cout << name << endl;
	}
	read.close();

	for (auto& el1 : Students)
	{
		if (el1.second->joined.size() > el1.second->left.size())
			el1.second->left.push_back("2:30");
		
		//convert students join and leave times that are before 1:00pm to be equal to 1:00pm
		for (int i = 0; i < el1.second->joined.size(); ++i)
		{
			if (el1.second->joined[i][1] == '2') //the time must be 12:xx, i.e. before 1:00pm
				el1.second->joined[i] = "1:00";
			if (el1.second->left[i][1] == '2') //the time must be 12:xx, i.e. before 1:00pm
				el1.second->left[i] = "1:00";
		}

		//compute differences
		for (int i = 0; i < el1.second->joined.size(); ++i)
		{
			int leave = 0;
			if (el1.second->left[i].substr(0,2) == "1:")
				leave += stoi(el1.second->left[i].substr(2, 2));
			else //el1.second->left[i][0] == '2'
				leave += 60 + stoi(el1.second->left[i].substr(2, 2));

			int arrive = 0;
			if (el1.second->joined[i][0] == '1')
				arrive += stoi(el1.second->joined[i].substr(2, 2));
			else //el1.second->joined[i][0] == '2'
				arrive += 60 + stoi(el1.second->joined[i].substr(2, 2));

			int diff = leave - arrive;
			el1.second->differences.push_back(diff);
		}

		//calculate totTimeInClass
		for (int i = 0; i < el1.second->differences.size(); ++i)
			el1.second->totTimeInClass += el1.second->differences[i];

		if (el1.second->totTimeInClass >= 60) //if they were present for at least 60 minutes of class time, they get credit for having been there
			el1.second->attendedClass = 1;

		if (el1.second->passedExercises && el1.second->attendedClass)
			el1.second->creditGiven = 1;
	}

	for (auto& el1 : Students)
		cout << el1.first << " " << el1.second->creditGiven << endl;

	ofstream write("grades.txt");
	for (auto& el1 : Students)
		write << el1.second->lName << "\t" << el1.second->fName << "\t" << el1.second->creditGiven << "\t" << "attended class: " << el1.second->attendedClass << "\t" << "passed exercises: " << el1.second->passedExercises << endl;
	
}


int countLabs(string line) //first we make a vector of all 'words' (characters separated by spaces), then we count how many times the word "Lab" is in the vector and return that count
{
	vector<string> words;
	string temp = "";
	for (int i = 0; i < line.length(); ++i)
	{
		if (line[i] != ' ')
			temp += line[i];
		else //line[i] == ' '
		{
			words.push_back(temp);
			temp = "";
		}
	}

	int labCount = 0;
	for (int i = 0; i < words.size(); ++i)
		if (words[i] == "Lab")
			++labCount;

	return labCount;
}