#ifndef __RPN_HPP__
#define __RPN_HPP__

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#define CMD_EXIT    "exit"

class Rpn;

typedef void (Rpn::*RpnFunction)(void);
typedef std::unordered_map<std::string, RpnFunction> RpnFuncMap;

enum RpnMode { rpnmHex, rpnmDec, rpnmBin, rpnmOct };


class Rpn {
  private:
	RpnMode mMode = rpnmDec;
	RpnFuncMap mFunctions;
	std::map<std::string, double> mVars;
	std::vector<double> mStack;
	int mRepeat = 1;
	bool hasVariable(std::string& aStr);
	bool isVariable(const std::string& aStr, double& value);

	// functions
	void fAdd();
	void fSub();
	void fMul();
	void fDiv();
	void fCla();
	void fClr();
	void fClv();
	void fBlnot();
	void fNoteq();
	void fMod();
	void fInc();
	void fDec();
	void fRand();

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


  public:
  	bool interactive = true;

	Rpn(const Rpn&) = delete;
	Rpn& operator=(const Rpn&) = delete;

	Rpn();
	virtual ~Rpn();

	void presentPrompt();
	void parse(const std::string& aStr);
};

#endif