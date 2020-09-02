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

enum RpnMode { rpnmHex, rpnmDec, rpnmBin, rpnmOct };


class Rpn {
  private:
  	bool mVerticalStack = false;
	RpnMode mMode = rpnmDec;
	RpnFuncMap mFunctions;
	std::map<std::string, double> mVars;
	std::map<std::string, std::string> mMacros;
	std::vector<double> mStack;
	int mRepeat = 1;
	bool hasVariable(std::string& aStr);
	bool isVariable(const std::string& aStr, double& value);
	bool popStack(int reqSize, const std::string& aError, std::vector<double>& outElems);

	std::string varPrefix() { return std::string((stdoutTerminal ? CLR : "")) + "[ " + (stdoutTerminal ? RST : ""); };
	std::string varSuffix() { return std::string((stdoutTerminal ? CLR : "")) + "]" + (stdoutTerminal ? RST : ""); };
	std::string cliSuffix() { return std::string((stdoutTerminal ? CLR : "")) + "> " + (stdoutTerminal ? RST : ""); };

	// an alternative approach handling operations
	bool operation(const std::string& aOper);

	// functions
	void fCla();
	void fClr();
	void fClv();
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

	void macroDefine(const std::vector<std::string>& elements);

  public:
	bool interactive = false;
	bool stdoutTerminal = true;

	Rpn(const Rpn&) = delete;
	Rpn& operator=(const Rpn&) = delete;

	Rpn();
	virtual ~Rpn();

	virtual void presentPrompt();
	void parse(const std::string& aStr);
};

#endif