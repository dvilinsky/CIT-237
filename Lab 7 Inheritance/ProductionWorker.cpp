#include <string>
#include <iostream>
#include "ProductionWorker.h"
#include "Employee.h"
using namespace std;

ProductionWorker::ProductionWorker() : Employee() {
	shift = 1;
	payRate = 0;
}

ProductionWorker::ProductionWorker(int shift, double payRate, string aName, string aDate) :
Employee(aName, aDate) {
	setShift(shift);
	setPayRate(payRate);
}

double ProductionWorker::getPayRate() const {
	return payRate;
}

void ProductionWorker::setPayRate(double payRate) {
	if (!testPayRate(payRate)) {
		throw InvalidPayRate(payRate);
	}
	this->payRate = payRate;
}

string ProductionWorker::getShift() const {
	return shift == 1 ? "Day" : "Night";
}

void ProductionWorker::setShift(int shift) {
	//if we've got an invalid shift number, ie one that is not 1 or not 2, throw exception
	if (shiftIsValid(shift)) {
		this->shift = shift;
	}
	else {
		throw InvalidShift(shift);
	}
}

//tests to see if the user has input a valid payrate, ie one that is either an int, 
//or a double of the form XX.XX. Returns true if payrate is valid, false otherwise
bool ProductionWorker::testPayRate(double payRate) {
	return payRate >= 0; //what are some other invalid payrates to guard against?
}

//returns true if shift is valid, false otherwise 
bool ProductionWorker::shiftIsValid(int shift) {
	return (shift == 1 || shift == 2);
}

