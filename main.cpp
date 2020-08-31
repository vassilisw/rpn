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
		std::cerr << "file: '" << aFilePath << "' not found\n";
		return;
	}

	std::cerr << "executing file: '" << aFilePath << "'\n";
	for (auto& s : fileLines) {
		try {
			aRpn.parse(s);
		}
		catch (const char* e) {
			std::cerr << e << std::endl;
		}
	}
}

void loopParse(Rpn& aRpn) {
	aRpn.interactive = stdinIsTerminal();
	std::string userInput;
	while (!std::cin.eof()) {
		try {
			aRpn.presentPrompt();
			std::getline(std::cin, userInput);
			if (userInput == CMD_EXIT)
				break;
			aRpn.parse(userInput);
		}
		catch (const char* e) {
			std::cerr << e << std::endl;
		}
	}
}

int main(int argc, char *argv[]) {
	// no need for getopts()
	if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
		usage();
		return EXIT_SUCCESS;
	}

	initRandom();

	Rpn rpn;
	rpn.stdoutTerminal = isatty(fileno(stdout));
	auto rcfile = userDir() + separator() + ".rpnrc";
	if (fileExists(rcfile))
		executeFile(rpn, rcfile);

	if (argc == 1 || !stdinIsTerminal()) {
		// i.e. interactice or pipe input
		loopParse(rpn);
	}
	else {
		// one shot
		try {
			rpn.parse(argv[1]);
		}
		catch (const char* e) {
			std::cerr << e << std::endl;
			return EXIT_FAILURE;
		}

		rpn.presentPrompt();
	}

	return EXIT_SUCCESS;
}