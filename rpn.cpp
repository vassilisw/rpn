#include "rpn.hpp"
#include "helpers.hpp"
#include <iomanip>


// ------------------------------------------------------------------
// private

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

void Rpn::fAdd() {
	if (mStack.size() < 2) throw "(+) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back(values.at(0) + values.at(1));
}

void Rpn::fSub() {
	if (mStack.size() < 2) throw "(-) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back(values.at(0) - values.at(1));
}

void Rpn::fMul() {
	if (mStack.size() < 2) throw "(*) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back(values.at(0) * values.at(1));
}

void Rpn::fDiv() {
	if (mStack.size() < 2) throw "(/) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back(values.at(0) / values.at(1));
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

void Rpn::fBlnot() {
	if (mStack.size() < 1) throw "(!) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	mStack.emplace_back(!v1);
}

void Rpn::fNoteq() {
	if (mStack.size() < 2) throw "(!=) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back(values.at(0) != values.at(1));
}

void Rpn::fMod() {
	if (mStack.size() < 2) throw "(%) stack error";

	auto values = popNVectorElements(mStack, 2);
	mStack.emplace_back((long long)values.at(0) % (long long)values.at(1));
}

void Rpn::fInc() {
	if (mStack.size() < 1) throw "(++) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	mStack.emplace_back(++v1);
}

void Rpn::fDec() {
	if (mStack.size() < 1) throw "(--) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	mStack.emplace_back(--v1);
}

void Rpn::fRand() {
	// better randomization needed ?!
	mStack.emplace_back(rand());
}

void Rpn::fFact() {
	if (mStack.size() < 1) throw "(fact) stack error";

	auto v1 = mStack.back();
	if (v1 < 1) throw "factorial error: negative number";

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
	if (mStack.size() < 1) throw "(pick) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1) throw "(pick) stack error";

	auto v2 = *(mStack.begin() + v1-1);
	mStack.erase(mStack.begin() + v1-1);
	mStack.emplace_back(v2);
}

void Rpn::fsRepeat() {
	if (mStack.size() < 1) throw "(repeat) stack error";

	mRepeat = mStack.back();
	mStack.pop_back();
}

void Rpn::fsDepth() {
	mStack.emplace_back(mStack.size());
}

void Rpn::fsDrop() {
	if (mStack.size() < 1) throw "(drop) stack error";
	mStack.pop_back();
}

void Rpn::fsDropn() {
	if (mStack.size() < 1) throw "(dropn) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1) throw "(dropn) stack error";

	mStack.erase(mStack.end() - v1, mStack.end());
}

void Rpn::fsDup() {
	if (mStack.size() < 1) throw "(dup) stack error";

	mStack.emplace_back(mStack.back());
}

void Rpn::fsDupn() {
	if (mStack.size() < 1) throw "(dupn) stack error";

	auto v1 = mStack.back();
	mStack.pop_back();
	if (mStack.size() < v1) throw "(dupn) stack error";

	auto values = mStack;
	mStack.insert(mStack.end(), mStack.end() - v1, mStack.end());
}

void Rpn::fsRoll() {
	if (mStack.size() < 1) throw "(roll) stack error";

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
	if (mStack.size() < 1) throw "(roll) stack error";

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
	if (mStack.size() < 2) throw "(swap) stack error";

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

// if called with just CMD_MACRO, will just display defined macros
void Rpn::macroDefine(const std::vector<std::string>& elements) {
	std::stringstream macro;
	std::string macroName;
	auto itMacro = std::find(elements.begin(), elements.end(), CMD_MACRO);
	for (auto it = ++itMacro; it != elements.end(); ++it) {
		if (it->empty()) continue;

		if (macroName.empty()) {
			if (mFunctions.find(*it) != mFunctions.end()) throw (*it + ": reserved keyword").c_str();

			macroName = *it;
			continue;
		}

		macro << *it << " ";
	}

	if (!macroName.empty() && !macro.str().empty())
		mMacros[macroName] = macro.str();

	if (macroName.empty())
		for (auto& s : mMacros)
			std::cout << s.first << ": " << s.second << std::endl;
}

// ------------------------------------------------------------------
// public

Rpn::Rpn() {
	mFunctions.emplace("+",      &Rpn::fAdd);
	mFunctions.emplace("-",      &Rpn::fSub);
	mFunctions.emplace("*",      &Rpn::fMul);
	mFunctions.emplace("/",      &Rpn::fDiv);
	mFunctions.emplace("cla",    &Rpn::fCla);
	mFunctions.emplace("clr",    &Rpn::fClr);
	mFunctions.emplace("clv",    &Rpn::fClv);
	mFunctions.emplace("!",      &Rpn::fBlnot);
	mFunctions.emplace("!=",     &Rpn::fNoteq);
	mFunctions.emplace("%",      &Rpn::fMod);
	mFunctions.emplace("++",     &Rpn::fInc);
	mFunctions.emplace("--",     &Rpn::fDec);
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

}

Rpn::~Rpn() {
	
}

void Rpn::presentPrompt() {
	if (mVars.size() && !mVerticalStack) std::cout << varPrefix();
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
		// input is number
		if (isNumber(s)) {
			for (int i = 0; i < mRepeat; ++i)
				mStack.emplace_back(std::stod(s));
			mRepeat = 1;
		}
		else
		// input is variable assignment (e.g. x=)
		if (hasVariable(s)) {
			if (mStack.size() < 1) throw "variable stack error";
			if (mFunctions.find(s) != mFunctions.end()) throw "reserved keyword";

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
