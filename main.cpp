#include "consts.hpp"
#include "helpers.hpp"
#include "rpn.hpp"
#include <iostream>


void usage() {
	std::cout <<
		"USAGE:\n"
		"  rpn                Launch in interactive mode\n"
		"  rpn [expression]   Evaluate an one-line expression\n\n"
		"RC FILE:\n"
		"  rpn will execute the contents of ~/.rpnrc at startup if it exists.\n";
}

void executeFile(Rpn& aRpn, std::string aFilePath) {
	std::vector<std::string> fileLines;

	if (!readFileToVector(aFilePath, fileLines)) {
		std::cout << "file: '" << aFilePath << "' not found\n";
		return;
	}

	std::cout << "executing file: '" << aFilePath << "'\n";
	for (auto& s : fileLines) {
		try {
			aRpn.parse(s);
		}
		catch (const char* e) {
			std::cout << e << std::endl;
		}
	}
}

void interactive(Rpn& aRpn) {
	std::string userInput;
	while (true) {
		try {
			aRpn.presentPrompt();
			std::getline(std::cin, userInput);
			if (userInput == CMD_EXIT)
				break;
			aRpn.parse(userInput);
		}
		catch (const char* e) {
			std::cout << e << std::endl;
		}
	}
}

int main(int argc, char *argv[]) {
	initRandom();

	Rpn rpn;
	auto rcfile = userDir() + separator() + ".rpnrc";
	if (fileExists(rcfile))
		executeFile(rpn, rcfile);

	if (argc == 1) {
		interactive(rpn);
	}
	else
	if (strcmp(argv[1], "-h") == 0) {
		usage();
		exit(EXIT_SUCCESS);
	}
	else {
		// one shot
		rpn.interactive = false;
		rpn.parse(argv[1]);
		rpn.presentPrompt();
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}