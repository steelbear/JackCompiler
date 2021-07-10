#pragma once
#include "JackTokenizer.h"
#include "JackError.h"
#include <iostream>
#include <fstream>

using namespace std;

class CompilationEngine
{
	JackTokenizer tokenizer;
	ofstream& outputFile;
	string offset;

public:
	CompilationEngine(ifstream& inputFile, ofstream& outputFile);
	void compileClass();
	void compileClassVarDec();
	void compileSubroutine();
	void compileParameterList();
	void compileVarDec();
	void compileStatements();
	void compileDo();
	void compileLet();
	void compileWhile();
	void compileReturn();
	void compileIf();
	void compileExpression();
	void compileTerm();
	void compileExpressionList();

private:
	void keyword(string expected = "");
	void symbol(string expected = "");
	void integerConstant();
	void stringConstant();
	void identifier();
	bool isType();
	bool isSubroutine();
	bool isVarDec();
	bool isStatement();
	bool isOp();
	bool isTerm();
	void compileType();
	void compileStatement();
	void compileSubroutineCall();
	void addTab();
	void removeTab();
};

