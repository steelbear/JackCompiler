#include "JackError.h"

NotExpectedException::NotExpectedException(string expected, string faced, int row, int col)
{
	msg += "Exception(" + to_string(row) + "," + to_string(col) + ")\n";
	msg += "\t: Expected \'" + expected + "\', but \'" + faced + "\'\n";
}

NotExpectedException::~NotExpectedException()
{}

const char* NotExpectedException::what() const noexcept
{
	return msg.c_str();
}

