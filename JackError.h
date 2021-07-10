#pragma once

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class NotExpectedException : exception {
public:
	string msg;

	NotExpectedException(string expected, string faced, int row, int col);
	~NotExpectedException();
	const char* what() const noexcept;
};