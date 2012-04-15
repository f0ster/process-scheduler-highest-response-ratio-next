#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class process {
	protected:
	public:
		int pid;
		int burst;
		int arrival;
		int timeRemaining;
		int doneWaiting;
		int finishTime;

		process() {
			this->pid = 0;
			this->burst = 0;
			this->arrival = 0;
			this->timeRemaining =0;
			this->doneWaiting = 0;
			this->finishTime = 0;
		};
		process(int pid, int burst, int arrival) {
			this->pid = pid;
			this->burst = burst;
			this->arrival = arrival;
			this->timeRemaining = burst;
			this->doneWaiting = 0;
			this->finishTime = 0;
		};
		~process() {
			
		};
		process( const process &p) {
			pid = p.pid;
			burst = p.burst;
			arrival = p.arrival;
			timeRemaining = p.timeRemaining;
			doneWaiting = p.doneWaiting;
			finishTime = p.finishTime;
		};
		process& operator= (const process &p){
			pid = p.pid;
			burst = p.burst;
			arrival = p.arrival;
			timeRemaining = p.timeRemaining;
			doneWaiting = p.doneWaiting;
			finishTime = p.finishTime;
			return *this;
		};
		bool operator== (const process &p) {
			return (this->pid==p.pid && this->arrival == p.arrival && this->burst == p.burst);
		}
		bool operator!= (const process &p){
			return !(this->pid==p.pid && this->arrival == p.arrival && this->burst == p.burst);
		}
		friend ostream& operator<< (ostream &os, const process &p) {
			p.display(os);
			return os;
		};
		void display(ostream &os) const {
			os << "\t" << pid;
			os << "\t" << burst;
			os << "\t" << arrival;
			os << "\t\t" << timeRemaining;
		};

};
#endif
