#include "Solver.h"


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Invalid command line option" << std::endl;
		exit(1);
	}
	// decimal precision for output
	std::cout << std::setprecision(4) << std::fixed;
	if(strcmp(argv[1], "s") == 0) Solver s;
	return 0;
}