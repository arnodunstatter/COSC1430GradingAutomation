#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "Student.h"

using namespace std;

int main()
{
	//Instructions for use:
	//Download the attendance from teams, open it and save-as .csv, then open the .csv with notebook and save it as a .txt with ANSI encoding.
	//rename it to "attendance.csv"



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
		if (temp->name != ",")
			Students[temp->name] = temp;
	}
	read.close(); 
	
	read.open("attendance.csv");
	if (!read.is_open())
	{
		cout << "attendance did not open correctly.\n";
		return 0;
	}
	//else...
	string line;
	getline(read, line); //this will just get the first line of garbage
	while (!read.eof())
	{
		//lets parse the line
		string first, last, name, keyword, time, trash;
		getline(read, last, ','); //gets the string ""Abdallah"
		if (last == "") //handles empty line at the end
			continue;
		if (last.substr(0, 31) == "Full Name\tUser Action\tTimestamp")
			last = last.substr(32);
		while (!isalpha(last[0])) //this loop removes the extra characters before the beginning of the last name
			last = last.substr(1, last.length() - 1); //edits it to "Abdallah"
		getline(read, first, '\t'); //gets the string " Zachery"
		first = first.substr(1, first.length() - 1); //edits it to "Zachery"
		if (first[first.length() - 2] == ' ') //gets rid of middle initial
			first = first.substr(0, first.length() - 2);
		name = last + "," + first;
		
		getline(read, keyword, '\t');
		getline(read, trash, ' ');
		getline(read, time, ' ');
		time = time.substr(0, 4); //converts "1:00:00" to "1:00"
		getline(read, trash);

		if (name != "Dunstatter,Arno" && name != "Sanchez,Violeta" && name != "Dendukuri,Manasa")
		{
			if (name != "Dunstatter,Arno" && Students.find(name) == Students.end()) //then the student isn't already in our map, we must add them
			{
				cout << "The following student was not found in the map: " << name << endl;
			}
			else //we must change it
			{
				if (keyword == "Left")
					Students[name]->left.push_back(time); //this won't really be used for anything
				else
					Students[name]->joined.push_back(time);
			}
		}
	}
	read.close();

	//for (auto& el1 : Students) //this for-loop is from an older iteration of the program
	//{
		//if (el1.second->joined.size() > el1.second->left.size())
			//el1.second->left.push_back("4:00");
		//compute differences
		//for (int i = 0; i < el1.second->joined.size(); ++i)
		//{
		//	int leave = 0;
		//	if (el1.second->left[i][0] == '2') //lab starts at 2:30
		//		leave += (stoi(el1.second->left[i].substr(2, 2)) - 30);
		//	else if (el1.second->left[i][0] == '3')
		//		leave += (stoi(el1.second->left[i].substr(2, 2)) + 30);
		//	else if (el1.second->left[i][0] == '4')
		//		leave += 90;
		//	int arrive = 0;
		//	if (el1.second->joined[i][0] == '2')
		//		arrive += (stoi(el1.second->joined[i].substr(2, 2)) - 30);
		//	else //el1.second->joined[i][0] == '3'
		//		arrive += (stoi(el1.second->joined[i].substr(2, 2)) + 30);
		//	int diff = leave - arrive;
		//	el1.second->differences.push_back(diff);
		//}
		//calculate totTimeInClass
	//	for (int i = 0; i < el1.second->differences.size(); ++i)
	//		el1.second->totTimeInLab += el1.second->differences[i];
	//	if (el1.second->totTimeInLab >= 45) //if they were present for at least 60 minutes of class time, they get credit for having been there
	//		el1.second->attendedLab = 1;
	//	ofstream write("lab_attendance_grades.txt");
	//	for (auto& el1 : Students)
	//		write << el1.second->lName << "\t" << el1.second->fName << "\t" << el1.second->attendedLab << endl;
	//	write.close();
	//}

	string labStart, reconvened; 
	double mercy;
	cout << "What time did lab start? (h:mm)\n";
	cin >> labStart;
	cout << "What time did lab reconvene? (h:mm)\n";
	cin >> reconvened;
	cout << "How many minutes late can the students be?\n";
	cin >> mercy;
	//now we convert reconvened to a double, i.e. the number of minutes from the labStart
	double doubRecon;
	if (reconvened[0] == labStart[0])
		doubRecon = stod(reconvened.substr(2, 2));
	else
	{
		int hours = reconvened[0] - labStart[0]; //this will serve as a factor to multiply by 60
		--hours;
		doubRecon = 60 - stod(labStart.substr(2,2)) + 60 * hours + stod(reconvened.substr(2, 2));
	}

	double joinedMinutesFromLabStart; //this will be temp variable that holds the number of minutes after lab start, at which the student joined
	for (auto& el1 : Students)
	{
		for (int i = 0; i < el1.second->joined.size(); ++i) //cycles through all join times and determines if they are close enough to the start of lab or the start of the explanation time to change the student's bool values
		{
			if (el1.second->joined[i][0] == labStart[0]) //same hour
			{
				joinedMinutesFromLabStart = stod(el1.second->joined[i].substr(2, 2)) - stod(labStart.substr(2, 2));
			}
			else
			{
				int hours = el1.second->joined[i][0] - labStart[0]; //this will serve as a factor to multiply by 60
				--hours;
				joinedMinutesFromLabStart = 60 - stod(labStart.substr(2, 2)) + 60 * hours + stod(el1.second->joined[i].substr(2, 2));
			}
			//el1.second->joinedMinutesFromLabStart.push_back(joinedMinutesFromLabStart);

			if (joinedMinutesFromLabStart <= mercy)
				el1.second->joinedOnTime = true;
			else if (joinedMinutesFromLabStart <= doubRecon + mercy)
				el1.second->reconvenedOnTime = true;
		}
		//once that student has had all their joined times checked we can determine if they get credit
		if (el1.second->joinedOnTime && el1.second->reconvenedOnTime)
			el1.second->attendedLab = true;
	}

	ofstream write("lab_attendance_grades.txt");
	for (auto& el1 : Students)
	{
		write << el1.second->lName << "\t" << el1.second->fName << "\t" << el1.second->attendedLab << endl;
	}
	write.close();

	//troubleshooting begins
	//for (auto& el1 : Students)
	//{
	//	cout << endl;
	//	cout << el1.second->fName << " " << el1.second->lName << " was present in lab for " << el1.second->totTimeInLab << endl;
	//	cout << "Joined: ";
	//	for (int i = 0; i < el1.second->joined.size(); ++i)
	//		cout << el1.second->joined[i] << " ";
	//	cout << endl << "Left: ";
	//	for (int i = 0; i < el1.second->left.size(); ++i)
	//		cout << el1.second->left[i] << " ";
	//	cout << endl;
	//}
	//troubleshooting ends
}