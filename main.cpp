#include "consts.hpp"
#include "helpers.hpp"
#include "rpn.hpp"
#include <iostream>


void usage() {
	std::cout <<
		"Usage:\n"
		"  –w <fname>  File to write to\n"
		"  –h          Show help\n";
}

void interactive() {
	Rpn rpn;
	std::string userInput;
	while (true) {
		try {
			rpn.presentPrompt();
			std::getline(std::cin, userInput);
			if (userInput == CMD_EXIT)
				break;
			rpn.parse(userInput);
		}
		catch (const char* e) {
			std::cout << e << std::endl;
		}
	}
}

int main(int argc, char *argv[]) {
	initRandom();

	if (argc == 1) {
		interactive();
	}
	else
	if (strcmp(argv[1], "-h") == 0) {
		usage();
		exit(EXIT_SUCCESS);
	}
	else {
		// one shot
		Rpn rpn;
		rpn.interactive = false;
		rpn.parse(argv[1]);
		rpn.presentPrompt();
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}