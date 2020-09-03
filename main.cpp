#include "consts.hpp"
#include "helpers.hpp"
#include "rpn.hpp"
#include "termcharbuf.hpp"
#include <iostream>
#include <cstring>


// user's commands history
std::vector<std::string> History;
long HistoryIndex = -1;

void usage() {
	std::cout <<
		"USAGE:\n"
		"  rpn                Launch in interactive mode\n"
		"  rpn [expression]   Evaluate an one-line expression\n\n"
		"RC FILE:\n"
		"  rpn will execute the contents of ~/.rpnrc at startup if it exists.\n";
}

void onKeyPressed(std::string& outStr, int key, void* aRpn/*Rpn* aRpn*/) {
	// the line is clear at this point, so..
	((Rpn*)aRpn)->presentPrompt();
	HistoryIndex = (key == KEY_UP) ? std::max((long)0, --HistoryIndex) :
	                                 std::min((long)History.size() - 1, ++HistoryIndex);

	if (key == KEY_UP) {
		outStr = History.at(HistoryIndex);
	}
	else
	if (key == KEY_DOWN) {
		outStr = History.at(HistoryIndex);
	}
	else {
		outStr = "";
	}
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
	if (aRpn.interactive) {
		History.reserve(100);
		History.emplace_back("");
		terminalCharBufInit();
	}

	std::string userInput;
	while (!std::cin.eof()) {
		try {
			aRpn.presentPrompt();

			if (aRpn.interactive)
				userInput = wsdGetline(onKeyPressed, &aRpn);
			else
				std::getline(std::cin, userInput);

			if (userInput == CMD_EXIT)
				break;

			if (aRpn.interactive) {
				if (!userInput.empty())
					History.emplace_back(userInput);
				HistoryIndex = History.size();
			}

			aRpn.parse(userInput);
		}
		catch (const char* e) {
			std::cerr << e << std::endl;
		}
	}

	if (aRpn.interactive)
		terminalRestore();
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
			rpn.parse((joinStrings(std::vector<std::string>(argv + 1, argv + argc), " ")));
			rpn.presentPrompt();
		}
		catch (const char* e) {
			std::cerr << e << std::endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}