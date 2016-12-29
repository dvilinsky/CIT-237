// Implementation file for the Employee class

#include "Employee.h"
#include <string>
#include <iostream>
using namespace std;

int Employee::lastEmployeeNumberIssued=0;   // Sequential employee number

	// Default constructor
	Employee::Employee()
	{
		lastEmployeeNumberIssued++;
		employeeNumber = lastEmployeeNumberIssued;
		employeeName = "";
		setHireDate("");
	}


	// Constructor
	Employee::Employee(string aName, string aDate)
	{
		lastEmployeeNumberIssued++;
		employeeNumber = lastEmployeeNumberIssued;
		setEmployeeName(aName);
		setHireDate(aDate);
	}

	// Mutators
	void Employee::setEmployeeName(string n)
	{
		employeeName = n;
	}


	void Employee::setHireDate(string date)
	{
		if (date.at(2) != "/"[0] && date.at(5) != "/"[0]) {
			throw InvalidHireDateEmployeeNumber(date);
		}
		else {
			hireDate = date;
		}
	}

	// Accessors
	string Employee::getEmployeeName() const
	{
		return employeeName;
	}

	int Employee::getEmployeeNumber() const
	{
		return employeeNumber;
	}

	string Employee::getHireDate() const
	{
		return hireDate;
	}

	int Employee::getLastEmployeeNumberIssued()
	{
		return lastEmployeeNumberIssued;
	}

	
