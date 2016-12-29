#ifndef PRODUCTIONWORKER_H
#define PRODUCTIONWORKER_H
#include "Employee.h"
#include <string>

class ProductionWorker : public Employee {
	private:
		int shift; //1 is day, 2 is night shift
		double payRate;
	public:
		class InvalidShift {
			private:
				int shift;
			public:
				InvalidShift(int shift) {
					this->shift = shift;
				}
				int getShift() const {
					return this->shift;
				}
		};
		class InvalidPayRate {
		private:
			double payRate;
		public:
			InvalidPayRate(double rate) {
				payRate = rate;
			}
			double getPayRate() const {
				return payRate;
			}
		};
		ProductionWorker();
		ProductionWorker(int, double, string, string); //shift, payrate, name, hire date
		double getPayRate() const;
		void setPayRate(double);
		string getShift() const;
		void setShift(int);
		bool shiftIsValid(int);
		bool testPayRate(double);
};

#endif // !PRODUCTIONWORKER_H
