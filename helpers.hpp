#ifndef __HELPERS_HPP__
#define __HELPERS_HPP__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>


inline void initRandom() {
	srand(time(NULL));
}

inline std::vector<std::string> splitString(const std::string& aStr, char aDelimiter) {
	std::vector<std::string> tokens;
	tokens.reserve(10);
	std::string token;
	std::istringstream tokenStream(aStr);
	while (std::getline(tokenStream, token, aDelimiter)) {
		tokens.push_back(token);
	}

	return tokens;
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

#endif