#include "rpn.hpp"
#include "helpers.hpp"


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
	mStack.emplace_back((int)values.at(0) % (int)values.at(1));
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

}

void Rpn::fsRoll() {

}

void Rpn::fsRolld() {

}

void Rpn::fsStack() {

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

void Rpn::setMode(RpnMode aMode) {
	mMode = aMode;
}

void Rpn::presentPrompt() {
	if (mVars.size()) std::cout << "[ ";

	switch (mMode) {
		case rpnmHex:
			for (auto& n : mVars) std::cout << std::hex << n.first << "=" << (int)n.second << " ";
			if (mVars.size()) std::cout << "] ";
			for (auto& n : mStack) std::cout << std::hex << (int)n << " ";
			break;
		case rpnmDec:
			for (auto& n : mVars) std::cout << std::dec << n.first << "=" << n.second << " ";
			if (mVars.size()) std::cout << "] ";
			for (auto& n : mStack) std::cout << std::dec << n << " ";
			break;
		case rpnmBin:
			for (auto& n : mVars) std::cout << n.first << "=" << std::bitset<16>(n.second).to_string() << " ";
			if (mVars.size()) std::cout << "] ";
			for (auto& n : mStack) std::cout << std::bitset<16>(n).to_string() << " ";
			break;
		case rpnmOct:
			for (auto& n : mVars) std::cout << std::oct << n.first << "=" << (int)n.second << " ";
			if (mVars.size()) std::cout << "] ";		
			for (auto& n : mStack) std::cout << std::oct << (int)n << " ";
			break;
	}

	std::cout << std::dec;
	if (interactive) std::cout << "> ";
}

void Rpn::parse(const std::string& aStr) {
	std::vector<std::string> vElements = splitString(aStr, ' ');
	double value;
	for (auto& s : vElements) {
		if (s.empty()) continue;

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
		else {
			throw "unknown command";
		}
	}

	if (mRepeat != 1) {
		mRepeat = 1;
		throw "(repeat) stack error";
	}
}
