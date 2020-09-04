#ifndef __RPN_HPP__
#define __RPN_HPP__

#include "consts.hpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#define CMD_EXIT    "exit"
#define CMD_MACRO   "macro"

class Rpn;

typedef void (Rpn::*RpnFunction)(void);
typedef std::unordered_map<std::string, RpnFunction> RpnFuncMap;

// calculator base modes
enum RpnMode { rpnmHex, rpnmDec, rpnmBin, rpnmOct };


class Rpn {
  private:
  	bool mVerticalStack = false;
	RpnMode mMode = rpnmDec;
	RpnFuncMap mFunctions;
	std::map<std::string, double> mVars;
	std::map<std::string, std::string> mMacros;
	std::vector<double> mStack;
	// helper for 'repeat' command
	int mRepeat = 1;
	// aStr has the form <var>=
	bool hasVariable(std::string& aStr);
	// aStr is in mVars map, returns its value
	bool isVariable(const std::string& aStr, double& value);
	// pops reqSize elements from mStack and stores them in outElems
	bool popStack(unsigned int reqSize, const std::string& aError, std::vector<double>& outElems);

	// prefix and suffix for command line when in interactive mode
	std::string varPrefix() { return std::string((stdoutTerminal ? CLR : "")) + "[ " + (stdoutTerminal ? RST : ""); };
	std::string varSuffix() { return std::string((stdoutTerminal ? CLR : "")) + "]" + (stdoutTerminal ? RST : ""); };
	std::string cliSuffix() { return std::string((stdoutTerminal ? CLR : "")) + "> " + (stdoutTerminal ? RST : ""); };

	// an alternative approach handling operations
	bool operation(const std::string& aOper);

	// functions
	void fCla();
	void fClr();
	void fClv();
	void fSetPrec();
	void fRand();
	void fFact();

	// constants
	void fcE();
	void fcPI();

	// stack manipulation
	void fsPick();
	void fsRepeat();
	void fsDepth();
	void fsDrop();
	void fsDropn();
	void fsDup();
	void fsDupn();
	void fsRoll();
	void fsRolld();
	void fsStack();
	void fsSwap();

	// display
	void fdHex();
	void fdDec();
	void fdBin();
	void fdOct();

	// other
	void foPrintKeywords();
	void foHelp();

	// special
	void macroDefine(const std::vector<std::string>& elements);

  public:
  	// stdin is the terminal (vs pipe)
	bool interactive = false;
	// stdout is the terminal (vs pipe)
	bool stdoutTerminal = true;

	// gain attention just in case
	Rpn(const Rpn&) = delete;
	Rpn& operator=(const Rpn&) = delete;

	Rpn();
	virtual ~Rpn();

	virtual void presentPrompt();
	void parse(const std::string& aStr);
};

#endif