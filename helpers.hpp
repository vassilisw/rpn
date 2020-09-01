#ifndef __HELPERS_HPP__
#define __HELPERS_HPP__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>


inline void initRandom() {
	srand(time(NULL));
}

inline bool stdinIsTerminal() {
	return isatty(fileno(stdin));
}

inline char separator() {
	#ifdef _WIN32
	return '\\';
	#else
	return '/';
	#endif
}

inline std::string userDir() {
	char homedir[PATH_MAX];
	#ifdef _WIN32
	snprintf(homedir, PATH_MAX, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
	#else
	snprintf(homedir, PATH_MAX, "%s", getenv("HOME"));
	#endif

	return std::string(homedir);
}

inline bool fileExists(const std::string& aFilePath) {
    std::ifstream inFile(aFilePath);
    return inFile.good();
}

inline bool readFileToVector(const std::string& aFilename, std::vector<std::string>& outStrings) {
	std::ifstream file(aFilename);
	if(!file) return false;

	std::string str;
	while (std::getline(file, str)) {
		// Line contains string of length > 0 then save it in vector
		if(str.size() > 0) outStrings.push_back(str);
	}

	file.close();
	return true;
}

inline std::vector<std::string> splitString(const std::string& aStr, char aDelimiter) {
	std::vector<std::string> tokens;
	tokens.reserve(20);
	std::string token;
	std::istringstream tokenStream(aStr);
	while (std::getline(tokenStream, token, aDelimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

inline std::string joinStrings(const std::vector<std::string>& elements, const std::string& separator) {
	switch (elements.size()) {
		case 0:
			return "";
		case 1:
			return elements[0];
		default:
			std::stringstream ss;
			for (const auto& s : elements)
				ss << s << separator;
			return ss.str();
	}
}

inline bool isNumber(const std::string& aStr) {
	return aStr.find_first_not_of("-.0123456789") == std::string::npos;
}

template <class T>
inline std::vector<T> popNVectorElements(std::vector<T>& vec, int aCount) {
	std::vector<T> res;
	res.reserve(aCount);
	res.insert(res.begin(), vec.end() - aCount, vec.end());
	vec.erase(vec.end() - aCount, vec.end());

	return res;

	// auto v1 = *std::prev(mStack.end(), 2);
	// auto v2 = *std::prev(mStack.end(), 1);
	// mStack.erase(mStack.end() - 2, mStack.end());
	// mStack.emplace_back(v1 + v2);

	// auto v1 = mStack.back();
	// mStack.pop_back();
	// auto v2 = mStack.back();
	// mStack.pop_back();
	// mStack.emplace_back(v1 + v2);
}

inline long long fromBin(long long n) {
	long long factor = 1;
	long long total = 0;

	while (n != 0) {
		total += (n % 10) * factor;
		n /= 10;
		factor *= 2;
	}

	return total;
}

inline std::string handleInputBase(const std::string& aStr) {
	if (aStr.size() < 3) return aStr;

	auto trim = [](const std::string& s) { return s.substr(2, s.size()-2); };

	std::stringstream ss;
	try {
		if (aStr.find("0x") == 0)
			ss << std::stoll(trim(aStr), nullptr, 16);
		else
		if (aStr.find("0o") == 0)
			ss << std::stoll(trim(aStr), nullptr, 8);
		else
		if (aStr.find("0b") == 0)
			ss << fromBin(std::stoll(trim(aStr)));
		else
			return aStr;
	}
	catch (...) {
		throw "convertion error";
	}

	return ss.str();
}

// ** Ask me about this thing **
inline std::string factorial(int num) {
	std::stringstream ss;
	std::vector<int> res;
	res.push_back(1);
	int carry = 0;
	for (int i = 2; i <= num; i++) {
		for(int j = 0; j < res.size(); j++) {
			int tmp = res[j] * i;
			res[j] = (tmp+carry) % 10 ;
			carry = (tmp+carry) / 10;
		}
		while(carry != 0){
			res.push_back(carry % 10);
			carry = carry / 10;
		}
	}

	for(int i = res.size()-1; i >= 0; i--) ss << res[i];
	return ss.str();
}


#endif