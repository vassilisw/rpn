#include "rpn.hpp"
#include "helpers.hpp"
#include <iomanip>
#include <cmath>
#include <algorithm>


// ------------------------------------------------------------------
// private

bool Rpn::popStack(int reqSize, const std::string& aError, std::vector<double>& outElems) {
	if (mStack.size() < reqSize)
		throw (std::string("(") + aError + ") stack error").c_str();

	outElems.reserve(reqSize);
	outElems.insert(outElems.begin(), mStack.end() - reqSize, mStack.end());
	mStack.erase(mStack.end() - reqSize, mStack.end());
	return true;
}

bool Rpn::operation(const std::string& aOper) {
	std::vector<double> elems;
	double res, ipart;

	if      (aOper == "%"    && popStack(2, "%", elems))     res = (long long)elems.at(0) % (long long)elems.at(1);
	else if (aOper == "+"    && popStack(2, "+", elems))     res = elems.at(0) + elems.at(1);
	else if (aOper == "-"    && popStack(2, "-", elems))     res = elems.at(0) - elems.at(1);
	else if (aOper == "*"    && popStack(2, "*", elems))     res = elems.at(0) * elems.at(1);
	else if (aOper == "/"    && popStack(2, "/", elems))     res = elems.at(0) / elems.at(1);
	else if (aOper == "!="   && popStack(2, "!=", elems))    res = elems.at(0) != elems.at(1);
	else if (aOper == "++"   && popStack(1, "++", elems))    res = ++elems.at(0);
	else if (aOper == "--"   && popStack(1, "--", elems))    res = --elems.at(0);
	else if (aOper == "!"    && popStack(1, "!", elems))     res = !elems.at(0);
	else if (aOper == "~"    && popStack(1, "~", elems))     res = ~(long long)elems.at(0);
	else if (aOper == "&"    && popStack(2, "&", elems))     res = (long long)elems.at(0) & (long long)elems.at(1);
	else if (aOper == "|"    && popStack(2, "|", elems))     res = (long long)elems.at(0) | (long long)elems.at(1);
	else if (aOper == "^"    && popStack(2, "^", elems))     res = (long long)elems.at(0) ^ (long long)elems.at(1);
	else if (aOper == "<<"   && popStack(2, "<<", elems))    res = (long long)elems.at(0) << (long long)elems.at(1);
	else if (aOper == ">>"   && popStack(2, ">>", elems))    res = (long long)elems.at(0) >> (long long)elems.at(1);
	else if (aOper == "<"    && popStack(2, "<", elems))     res = elems.at(0) < elems.at(1);
	else if (aOper == ">"    && popStack(2, ">", elems))     res = elems.at(0) > elems.at(1);
	else if (aOper == "&&"   && popStack(2, "&&", elems))    res = elems.at(0) && elems.at(1);
	else if (aOper == "||"   && popStack(2, "||", elems))    res = elems.at(0) || elems.at(1);
	else if (aOper == "^^"   && popStack(2, "^^", elems))    res = !elems.at(0) != !elems.at(1);
	else if (aOper == "<="   && popStack(2, "<=", elems))    res = elems.at(0) <= elems.at(1);
	else if (aOper == "=="   && popStack(2, "==", elems))    res = elems.at(0) == elems.at(1);
	else if (aOper == ">="   && popStack(2, ">=", elems))    res = elems.at(0) >= elems.at(1);
	else if (aOper == "pow"  && popStack(2, "pow", elems))   res = pow(elems.at(0), elems.at(1));
	else if (aOper == "acos" && popStack(1, "acos", elems))  res = acos(elems.at(0));
	else if (aOper == "asin" && popStack(1, "asin", elems))  res = asin(elems.at(0));
	else if (aOper == "atan" && popStack(1, "atan", elems))  res = atan(elems.at(0));
	else if (aOper == "cos"  && popStack(1, "cos", elems))   res = cos(elems.at(0));
	else if (aOper == "cosh" && popStack(1, "cosh", elems))  res = cosh(elems.at(0));
	else if (aOper == "sin"  && popStack(1, "sin", elems))   res = sin(elems.at(0));
	else if (aOper == "sinh" && popStack(1, "sinh", elems))  res = sinh(elems.at(0));
	else if (aOper == "tanh" && popStack(1, "tanh", elems))  res = tanh(elems.at(0));
	else if (aOper == "exp"  && popStack(1, "exp", elems))   res = exp(elems.at(0));
	else if (aOper == "sqrt" && popStack(1, "sqrt", elems))  res = sqrt(elems.at(0));
	else if (aOper == "ln"   && popStack(1, "ln", elems))    res = log(elems.at(0));
	else if (aOper == "log"  && popStack(1, "log", elems))   res = log10(elems.at(0));
	else if (aOper == "ceil" && popStack(1, "ceil", elems))  res = ceil(elems.at(0));
	else if (aOper == "fp"   && popStack(1, "fp", elems))    res = modf(elems.at(0), &ipart);
	else if (aOper == "abs"  && popStack(1, "abs", elems))   res = abs(elems.at(0));
	else if (aOper == "max"  && popStack(2, "max", elems))   res = std::max(elems.at(0), elems.at(1));
	else if (aOper == "min"  && popStack(2, "min", elems))   res = std::min(elems.at(0), elems.at(1));
	else if (aOper == "floor" && popStack(1, "floor", elems)) res = floor(elems.at(0));
	else if (aOper == "round" && popStack(1, "round", elems)) res = round(elems.at(0));
	else if (aOper == "ip"   && popStack(1, "ip", elems)) { modf(elems.at(0), &ipart); res = ipart; }
	else return false;

	mStack.emplace_back(res);
	return true;
}

bool Rpn::hasVariable(std::string& aStr) {
	bool res = aStr.size() && (aStr.back() == '=');
	if (res) aStr.pop_back();
	return res;
}

bool Rpn::isVariable(const std::string& aStr, double& value) {
	auto v = mVars.find(aStr);
	bool res = v != mVars.end();
	if (res) value = v->second;
	return res;
}

void Rpn::fCla() {
	mVars.clear();
	mStack.clear();
}

void Rpn::fClr() {
	mStack.clear();
}

void Rpn::fClv() {
	mVars.clear();
}

void Rpn::fSetPrec() {
	if (mStack.size() < 1)
		throw "(prec) stack error";
	auto v1 = ++mStack.back();
	mStack.pop_back();
	std::cout << std::setprecision(std::max(2, (int)v1));
	if (v1 < 3)
		throw "min precision (1) set";
}

void Rpn::fRand() {
	// better randomization needed ?!
	mStack.emplace_back(rand());
}

void Rpn::fFact() {
	if (mStack.size() < 1)
		throw "(fact) stack error";
	auto v1 = mStack.back();
	if (v1 < 1)
		throw "factorial error: negative number";

	std::cerr << "ask me about this" << std::endl;

	mStack.pop_back();
	mStack.emplace_back(std::stod(factorial(v1)));
}

void Rpn::fcE() {
	mStack.emplace_back(M_E);
}

void Rpn::fcPI() {
	mStack.emplace_back(M_PI);
}

void Rpn::fsPick() {
	if (mStack.size() < 1)
		throw "(pick) stack error";
	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1)
		throw "(pick) stack error";

	auto v2 = *(mStack.begin() + v1-1);
	mStack.erase(mStack.begin() + v1-1);
	mStack.emplace_back(v2);
}

void Rpn::fsRepeat() {
	if (mStack.size() < 1)
		throw "(repeat) stack error";
	mRepeat = mStack.back();
	mStack.pop_back();
}

void Rpn::fsDepth() {
	mStack.emplace_back(mStack.size());
}

void Rpn::fsDrop() {
	if (mStack.size() < 1)
		throw "(drop) stack error";
	mStack.pop_back();
}

void Rpn::fsDropn() {
	if (mStack.size() < 1)
		throw "(dropn) stack error";
	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1)
		throw "(dropn) stack error";

	mStack.erase(mStack.end() - v1, mStack.end());
}

void Rpn::fsDup() {
	if (mStack.size() < 1)
		throw "(dup) stack error";
	mStack.emplace_back(mStack.back());
}

void Rpn::fsDupn() {
	if (mStack.size() < 1)
		throw "(dupn) stack error";
	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1)
		throw "(dupn) stack error";

	auto values = mStack;
	mStack.insert(mStack.end(), mStack.end() - v1, mStack.end());
}

void Rpn::fsRoll() {
	if (mStack.size() < 1)
		throw "(roll) stack error";
	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < 2) return;

	for (int i = 0; i < (int)v1; ++i) {
		auto v2 = mStack.front();
		mStack.erase(mStack.begin());
		mStack.emplace_back(v2);
	}
}

void Rpn::fsRolld() {
	if (mStack.size() < 1)
		throw "(roll) stack error";
	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < 2) return;

	for (int i = 0; i < (int)v1; ++i) {
		auto v2 = mStack.back();
		mStack.pop_back();
		mStack.insert(mStack.begin(), v2);
	}
}

void Rpn::fsStack() {
	mVerticalStack = !mVerticalStack;
}

void Rpn::fsSwap() {
	if (mStack.size() < 2)
		throw "(swap) stack error";
	std::iter_swap(mStack.end()-1, mStack.end()-2);
}

void Rpn::fdHex() {
	mMode = rpnmHex;
}

void Rpn::fdDec() {
	mMode = rpnmDec;
}

void Rpn::fdBin() {
	mMode = rpnmBin;
}

void Rpn::fdOct() {
	mMode = rpnmOct;
}

void Rpn::foHelp() {
	std::cout << HelpString << std::endl;

}

// if called with just CMD_MACRO, will just display defined macros
void Rpn::macroDefine(const std::vector<std::string>& elements) {
	std::stringstream macro;
	std::string macroName;
	auto itMacro = std::find(elements.begin(), elements.end(), CMD_MACRO);
	for (auto it = ++itMacro; it != elements.end(); ++it) {
		if (it->empty()) continue;

		// give macro its name
		if (macroName.empty()) {
			if (mFunctions.find(*it) != mFunctions.end())
				throw (*it + ": reserved keyword").c_str();
			macroName = *it;
			continue;
		}

		// building macro string
		macro << *it << " ";
	}

	if (!macroName.empty() && !macro.str().empty())
		mMacros[macroName] = macro.str();

	// just show defined macros
	if (macroName.empty())
		for (auto& s : mMacros)
			std::cout << s.first << ": " << s.second << std::endl;
}

// ------------------------------------------------------------------
// public

Rpn::Rpn() {
	mFunctions.emplace("cla",    &Rpn::fCla);
	mFunctions.emplace("clr",    &Rpn::fClr);
	mFunctions.emplace("clv",    &Rpn::fClv);
	mFunctions.emplace("prec",   &Rpn::fSetPrec);
	mFunctions.emplace("rand",   &Rpn::fRand);
	mFunctions.emplace("fact",   &Rpn::fFact);
	mFunctions.emplace("e",      &Rpn::fcE);
	mFunctions.emplace("pi",     &Rpn::fcPI);
	mFunctions.emplace("pick",   &Rpn::fsPick);
	mFunctions.emplace("repeat", &Rpn::fsRepeat);
	mFunctions.emplace("depth",  &Rpn::fsDepth);
	mFunctions.emplace("drop",   &Rpn::fsDrop);
	mFunctions.emplace("dropn",  &Rpn::fsDropn);
	mFunctions.emplace("dup",    &Rpn::fsDup);
	mFunctions.emplace("dupn",   &Rpn::fsDupn);
	mFunctions.emplace("roll",   &Rpn::fsRoll);
	mFunctions.emplace("rolld",  &Rpn::fsRolld);
	mFunctions.emplace("stack",  &Rpn::fsStack);
	mFunctions.emplace("swap",   &Rpn::fsSwap);
	mFunctions.emplace("hex",    &Rpn::fdHex);
	mFunctions.emplace("dec",    &Rpn::fdDec);
	mFunctions.emplace("bin",    &Rpn::fdBin);
	mFunctions.emplace("oct",    &Rpn::fdOct);
	mFunctions.emplace("help",   &Rpn::foHelp);

}

Rpn::~Rpn() {
	
}

void Rpn::presentPrompt() {
	if (mVars.size() && !mVerticalStack)
		std::cout << varPrefix();
	std::string eline = mVerticalStack ? "\n" : " ";

	// print the stacks
	for (auto& n : mVars) {
		if (mMode == rpnmDec)
			std::cout << std::dec << n.first << "=" << n.second << eline;
		else
		if (mMode == rpnmBin)
			std::cout << std::dec << n.first << "=0b" << std::bitset<16>(n.second).to_string() << eline;
		else
		if (mMode == rpnmHex)
			std::cout << std::hex << n.first << "=0x" << (long long)n.second << eline;
		else // oct
			std::cout << std::oct << n.first << "=0o" << (long long)n.second << eline;
	}

	if (mVars.size() && !mVerticalStack)
		std::cout << varSuffix() << eline;

	for (auto& n : mStack) {
		if (mMode == rpnmDec)
			std::cout << std::dec << n << eline;
		else
		if (mMode == rpnmBin)
			std::cout << std::dec << "0b" << std::bitset<16>(n).to_string() << eline;
		else
		if (mMode == rpnmHex)
			std::cout << std::hex << "0x" << (long long)n << eline;
		else // oct
			std::cout << std::oct << "0o" << (long long)n << eline;
	}

	// end lines, cli prompt, reset to dec
	std::cout << std::dec;
	if (interactive)
		std::cout << cliSuffix();
	else
		if (mVars.size() || mStack.size())
			std::cout << std::endl;
}

void Rpn::parse(const std::string& aStr) {
	std::vector<std::string> vElements = splitString(aStr, ' ');
	double value;
	for (auto& elm : vElements) {
		if (elm.empty()) continue;

		auto s = handleInputBase(elm);

		// input in functions
		auto funcIt = mFunctions.find(s);
		if (funcIt != mFunctions.end()) {
			int repeat = mRepeat;
			mRepeat = 1;
			auto func = funcIt->second;
			for (int i = 0; i < repeat; ++i)
				(this->*func)();
		}
		else
		// alternative approach (without func pointers) - more readable
		if (operation(s)) {
			int repeat = mRepeat;
			mRepeat = 1;
			for (int i = 1; i < repeat; ++i)
				operation(s);
		}
		else
		// input is number
		if (isNumber(s)) {
			for (int i = 0; i < mRepeat; ++i)
				mStack.emplace_back(std::stod(s));
			mRepeat = 1;
		}
		else
		// input is variable assignment (e.g. x=)
		if (hasVariable(s)) {
			if (mStack.size() < 1)
				throw "variable stack error";
			if (mFunctions.find(s) != mFunctions.end())
				throw "reserved keyword";

			mVars[s] = mStack.back();
			mStack.pop_back();
		}
		else
		// input in variables stack
		if (isVariable(s, value)) {
			mStack.emplace_back(value);
		}
		else
		// input is macro definition
		if (s == CMD_MACRO) {
			macroDefine(vElements);
			break;
		}
		else
		// input in macros stack
		if (mMacros.find(s) != mMacros.end()) {
			std::string m1;
			std::string macroStr = m1 = mMacros.find(s)->second;
			for (int i = 1; i < mRepeat; ++i)
				macroStr.append(m1);
			mRepeat = 1;
			parse(macroStr);
		}
		else {
			throw "unknown command";
		}
	}

	if (mRepeat != 1) {
		mRepeat = 1;
		throw "(repeat) stack error";
	}
}
