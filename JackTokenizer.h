#pragma once
#include "JackError.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

#define SYMBOL_STRING string("{}()[].,;+-*/&|<>=~")
#define OP_STRING string("+-*/&|<>=")

using namespace std;

enum class TokenType {
	KEYWORD,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST,
};

enum class TokenKeyword {
	CLASS,
	METHOD,
	FUNCTION,
	CONSTRUCTOR,
	INT,
	BOOLEAN,
	CHAR,
	VOID,
	VAR,
	STATIC,
	FIELD,
	LET,
	DO,
	IF,
	ELSE,
	WHILE,
	RETURN,
	TRUE,
	FALSE,
	NUL,
	THIS
};

class JackTokenizer
{
	static const string keywordList[21];
	ifstream& inputFile;
	string current;
	long row;
	long col;
	bool isLineBreak;

public:
	JackTokenizer(ifstream& inputFile);
	char get();
	void ignore();
	bool hasMoreTokens();
	void advance();
	TokenType tokenType();
	TokenKeyword keyword();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();

	string keywordToString();
	long getPosRow();
	long getPosCol();
};

