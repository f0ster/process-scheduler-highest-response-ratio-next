#ifndef HRN_H
#define HRN_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include "process.h"

using namespace std;


class hrn {
	protected:
		vector<process> proc_list;
		int clock;
		vector<int> cpu_hist;
	public:
		hrn() { clock =0; };		
		~hrn() { };
		
		class proxyCompare {
			hrn& that;
			public:
			proxyCompare(hrn &h) : that(h) {}
			bool operator()(process const &p1,process const &p2) const {
				double hrn1 = double ((that.clock-p1.arrival) + p1.burst) / double(p1.burst);
				double hrn2 = double ((that.clock-p2.arrival) + p2.burst) / double(p2.burst);
				//future checking
				bool oneIsInFuture = false;
				bool twoIsInFuture = false;
				if (that.clock < p1.arrival)
					oneIsInFuture = true;
				if (that.clock < p2.arrival)
					twoIsInFuture = true;
				if (twoIsInFuture && !oneIsInFuture)
					return true;
				if (!twoIsInFuture && oneIsInFuture)
					return false;
				else
					return hrn1 > hrn2; 
			}
		};
		
		int loadinput(char *filename) {
			ifstream testfile(filename); 
			//make sure file exists before calling sed
			if (!testfile) {
				cerr << "error loading input file!\n";
				return 0;
			}
			testfile.close();
			string cmd = "sed \'s/\t/ /g\' " + string(filename) + " > sample.clean";
			system(cmd.c_str());
			ifstream infile("sample.clean");
			if (!infile){
				cerr << "error loading file after clean-up!\n";
				return 0;
			}

			vector<string> input;
			string tmp_str;
			while( !infile.eof() )
			{
				getline(infile, tmp_str);
				input.push_back(tmp_str);
			}
			infile.close();
			system("rm sample.clean");

			//all lines loaded into string vector :input:
			//now parse through string vector and make process vector
			for(unsigned int i=0; i < input.size() ; i++) {
				int pid=0,burst=0,arrv=0;
				char *pch;
				/*char str[ strlen(input[i].c_str())+1 ];
				strcpy (str,input[i].c_str());*/
				char *str = new char[strlen(input[i].c_str())+1];
				strcpy (str,input[i].c_str());
				
				pch = strtok (str," ");
				int count = 1;
				while(pch!=NULL && count < 4){
					if ( atoi(pch) != 0 ) { // make sure it is a number
						if (count == 1)
							pid=atoi(pch);
						if (count == 2)
							burst=atoi(pch);
						if (count == 3)
							arrv=atoi(pch);
					}
					count++;
					pch = strtok(NULL," ");
				}
				delete[] str;
				if (pid != 0)
					proc_list.push_back(process(pid,burst,arrv));
			}
			print_all(0);
			
			return 0;
		};

		int run_sched(){
			bool doneWithOne = true;
			bool doneWithLast = false;
			vector<process>::iterator first = proc_list.begin();
			while(!doneWithLast) {
				if(doneWithOne) {
					sort( first , proc_list.end() , proxyCompare(*this) );
					doneWithOne = false;
				}
				if( first->arrival <= clock) {
					if(first->burst == first->timeRemaining) {
					// just finished waiting.. non preemptive.
						first->doneWaiting = clock;	
					}
					cpu_hist.push_back(first->pid);
					first->timeRemaining--;
					if(first->timeRemaining <= 0){
						first->finishTime = clock;
						if(first == proc_list.end()){
							doneWithLast = true;
						}
						else{
							first++;
							doneWithOne = true;		
						}
					}
				} else {
					cpu_hist.push_back(0);
				}
				this->clock++;
				
			} 
			print_all(1);
			return 0;
		};
		 void stats() {
			double avgTurn = 0;
			double avgWait = 0;
			double count = 0;
			vector<process>::iterator it;
			for(it = proc_list.begin(); it != proc_list.end(); it++){
				count++;
				avgTurn += (it->finishTime+1 - it->arrival);
				avgWait += (it->doneWaiting - it->arrival);
			}
			avgTurn /= count;
			avgWait /= count;
			cout << "Average Turnaround Time: " << avgTurn << endl;
			cout << "Average Waiting Time: " << avgWait << endl;

		};
		void print_all(int endresult) {
			if (endresult !=1){
				cout << this->clock <<"\n\tpid\tburst\tarrival\t\ttime remaining\n";
				for(unsigned int i=0; i < proc_list.size() ; i++) {
					cout << proc_list.at(i) << endl;
				}
			}
			else {
				for(unsigned int i=0; i<cpu_hist.size(); i++) {
					cout << cpu_hist.at(i) << " ";
				}
			}
			cout << endl;
		};
		
};



#endif
