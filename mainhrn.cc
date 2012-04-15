#include "hrn.h"

int main(int argc, char **argv){
	if(argc != 2) {
		cout << "Usage: hrn <input_file>\n";
		return 0;
	}
		hrn sched;
		sched.loadinput(argv[1]);
		sched.run_sched();
		sched.stats();

	return 0;
}
