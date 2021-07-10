#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(ifstream& inputFile, ofstream& outputFile)
	: tokenizer(JackTokenizer(inputFile)), outputFile(outputFile), offset("")
{
	tokenizer.advance();
	compileClass();
}

void CompilationEngine::compileClass()
{
	outputFile << offset << "<class>\n";
	addTab();
	keyword();
	identifier();
	symbol(); // {
	while (tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::STATIC || tokenizer.keyword() == TokenKeyword::FIELD))
	{
		compileClassVarDec();
	}
	while (isSubroutine())
	{
		compileSubroutine();
	}
	symbol(); // }
	removeTab();
	outputFile << offset << "</class>\n";
}

void CompilationEngine::compileClassVarDec()
{
	outputFile << offset << "<classVarDec>\n";
	addTab();
	keyword();
	compileType();
	identifier();
	while (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ',')
	{
		symbol(); // ,
		identifier();
	}
	symbol(); // ;
	removeTab();
	outputFile << offset << "</classVarDec>\n";
}

void CompilationEngine::compileSubroutine()
{
	outputFile << offset << "<subroutineDec>\n";
	addTab();
	if (isSubroutine())
	{
		keyword(); // constructor or function or method
	}
	if (tokenizer.tokenType() == TokenType::KEYWORD
		&& tokenizer.keyword() == TokenKeyword::VOID)
	{
		keyword();
	}
	else if (isType())
	{
		compileType();
	}
	identifier();
	symbol(); // (
	compileParameterList();
	symbol(); // )
	outputFile << offset << "<subroutineBody>\n";
	addTab();
	symbol(); // {
	while (isVarDec())
	{
		compileVarDec();
	}
	if (isStatement())
	{
		compileStatements();
	}
	symbol(); // }
	removeTab();
	outputFile << offset << "</subroutineBody>\n";
	removeTab();
	outputFile << offset << "</subroutineDec>\n";
}

void CompilationEngine::compileParameterList()
{
	outputFile << offset << "<parameterList>\n";
	addTab();
	if (isType())
	{
		compileType();
		identifier();
		while (tokenizer.tokenType() == TokenType::SYMBOL
			&& tokenizer.symbol() == ',')
		{
			symbol();
			compileType();
			identifier();
		}
	}
	removeTab();
	outputFile << offset << "</parameterList>\n";
}

void CompilationEngine::compileVarDec()
{
	outputFile << offset << "<varDec>\n";
	addTab();
	keyword(); // var
	compileType();
	identifier();
	while (tokenizer.tokenType() == TokenType::SYMBOL && tokenizer.symbol() == ',')
	{
		symbol(); // ,
		identifier();
	}
	symbol(); // ;
	removeTab();
	outputFile << offset << "</varDec>\n";
}

void CompilationEngine::compileStatements()
{
	outputFile << offset << "<statements>\n";
	addTab();
	compileStatement();
	while (isStatement())
	{
		compileStatement();
	}
	removeTab();
	outputFile << offset << "</statements>\n";
}

void CompilationEngine::compileDo()
{
	outputFile << offset << "<doStatement>\n";
	addTab();
	keyword(); // do
	compileSubroutineCall();
	symbol(); // ;
	removeTab();
	outputFile << offset << "</doStatement>\n";
}

void CompilationEngine::compileLet()
{
	outputFile << offset << "<letStatement>\n";
	addTab();
	keyword(); // let
	identifier();
	if (tokenizer.tokenType() == TokenType::SYMBOL
		&& tokenizer.symbol() == '[')
	{
		symbol(); // [
		compileExpression();
		symbol(); // ]
	}
	symbol(); // =
	compileExpression();
	symbol(); // ;
	removeTab();
	outputFile << offset << "</letStatement>\n";
}

void CompilationEngine::compileWhile()
{
	outputFile << offset << "<whileStatement>\n";
	addTab();
	keyword(); // while
	symbol(); // (
	compileExpression();
	symbol(); // )
	symbol(); // {
	compileStatements();
	symbol(); // }
	removeTab();
	outputFile << offset << "</whileStatement>\n";
}

void CompilationEngine::compileReturn()
{
	outputFile << offset << "<returnStatement>\n";
	addTab();
	keyword(); // return
	if (!(tokenizer.tokenType() == TokenType::SYMBOL
		&& tokenizer.symbol() == ';'))
	{
		compileExpression();
	}
	symbol(); // ;
	removeTab();
	outputFile << offset << "</returnStatement>\n";
}

void CompilationEngine::compileIf()
{
	outputFile << offset << "<ifStatement>\n";
	addTab();
	keyword(); // if
	symbol(); // (
	compileExpression();
	symbol(); // )
	symbol(); // {
	compileStatements();
	symbol(); // }
	if (tokenizer.tokenType() == TokenType::KEYWORD
		&& tokenizer.keyword() == TokenKeyword::ELSE)
	{
		keyword(); // else
		symbol(); // {
		compileStatements();
		symbol(); // }
	}
	removeTab();
	outputFile << offset << "</ifStatement>\n";
}

void CompilationEngine::compileExpression()
{
	outputFile << offset << "<expression>\n";
	addTab();
	compileTerm();
	while (isOp())
	{
		symbol(); // op
		compileTerm();
	}
	removeTab();
	outputFile << offset << "</expression>\n";
}

void CompilationEngine::compileTerm()
{
	outputFile << offset << "<term>\n";
	addTab();
	if (tokenizer.tokenType() == TokenType::INT_CONST)
	{
		integerConstant();
	}
	else if (tokenizer.tokenType() == TokenType::STRING_CONST)
	{
		stringConstant();
	}
	else if (tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::TRUE
			|| tokenizer.keyword() == TokenKeyword::FALSE
			|| tokenizer.keyword() == TokenKeyword::NUL
			|| tokenizer.keyword() == TokenKeyword::THIS))
	{
		keyword();
	}
	else if (tokenizer.tokenType() == TokenType::SYMBOL
		&& (tokenizer.symbol() == '-'
			|| tokenizer.symbol() == '~'))
	{
		symbol(); // - or ~
		compileTerm();
	}
	else if (tokenizer.tokenType() == TokenType::SYMBOL
		&& (tokenizer.symbol() == '('))
	{
		symbol(); // (
		compileExpression();
		symbol(); // )
	}
	else if (tokenizer.tokenType() == TokenType::IDENTIFIER)
	{
		identifier();
		
		if (tokenizer.tokenType() == TokenType::SYMBOL)
		{
			if (tokenizer.symbol() == '[')
			{
				symbol(); // [
				compileExpression();
				symbol(); // ]
			}
			else if (tokenizer.symbol() == '.')
			{
				symbol(); // .
				identifier();
				symbol(); // (
				compileExpressionList();
				symbol(); // )
			}
			else if (tokenizer.symbol() == '(')
			{
				symbol(); // (
				compileExpressionList();
				symbol(); // )
			}
		}
	}
	removeTab();
	outputFile << offset << "</term>\n";
}

void CompilationEngine::compileExpressionList()
{
	outputFile << offset << "<expressionList>\n";
	addTab();
	if (isTerm())
	{
		compileExpression();
		while (tokenizer.tokenType() == TokenType::SYMBOL
			&& tokenizer.symbol() == ',')
		{
			symbol(); // ,
			compileExpression();
		}
	}
	removeTab();
	outputFile << offset << "</expressionList>\n";
}

void CompilationEngine::compileType()
{
	if (tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::INT
			|| tokenizer.keyword() == TokenKeyword::CHAR
			|| tokenizer.keyword() == TokenKeyword::BOOLEAN))
	{
		keyword();
	}
	else if (tokenizer.tokenType() == TokenType::IDENTIFIER)
	{
		identifier();
	}
}

void CompilationEngine::compileStatement()
{
	if (tokenizer.keyword() == TokenKeyword::LET)
	{
		compileLet();
	}
	else if (tokenizer.keyword() == TokenKeyword::IF)
	{
		compileIf();
	}
	else if (tokenizer.keyword() == TokenKeyword::WHILE)
	{
		compileWhile();
	}
	else if (tokenizer.keyword() == TokenKeyword::DO)
	{
		compileDo();
	}
	else if (tokenizer.keyword() == TokenKeyword::RETURN)
	{
		compileReturn();
	}
}

void CompilationEngine::compileSubroutineCall()
{
	identifier();
	if (tokenizer.tokenType() == TokenType::SYMBOL
		&& tokenizer.symbol() == '.')
	{
		symbol(); // .
		identifier();
	}
	symbol(); // (
	compileExpressionList();
	symbol(); // )
}

void CompilationEngine::addTab()
{
	offset.append("  ");
}

void CompilationEngine::removeTab()
{
	offset.pop_back();
	offset.pop_back();
}

void CompilationEngine::keyword(string expected)
{
	if (!expected.empty() && tokenizer.keywordToString() == expected)
	{
		throw NotExpectedException(expected, tokenizer.identifier(), tokenizer.getPosRow(), tokenizer.getPosCol());
	}
	outputFile << offset << "<keyword> " << tokenizer.keywordToString() << " </keyword>\n";
	tokenizer.advance();
}

void CompilationEngine::symbol(string expected)
{
	if (!expected.empty())
	{
		bool notFound = true;
		for (int i = 0; i < expected.length(); i++)
		{
			if (tokenizer.symbol() == expected[0])
			{
				notFound = false;
			}
		}
		if (notFound)
		{
			throw NotExpectedException(expected, tokenizer.identifier(), tokenizer.getPosRow(), tokenizer.getPosCol());
		}
	}
	
	string xmlConverter[4] = {
		string("&lt;"),
		string("&gt;"),
		string("&quot;"),
		string("&amp;"),
	};
	if (string("<>\"&").find(tokenizer.symbol()) != string::npos)
	{
		outputFile << offset << "<symbol> " << xmlConverter[string("<>\"&").find(tokenizer.symbol())] << " </symbol>\n";
	}
	else
	{
		outputFile << offset << "<symbol> " << tokenizer.symbol() << " </symbol>\n";
	}
	tokenizer.advance();
}

void CompilationEngine::integerConstant()
{
	outputFile << offset << "<integerConstant> " << tokenizer.intVal() << " </integerConstant>\n";
	tokenizer.advance();
}

void CompilationEngine::stringConstant()
{
	outputFile << offset << "<stringConstant> " << tokenizer.stringVal() << " </stringConstant>\n";
	tokenizer.advance();
}

void CompilationEngine::identifier()
{
	outputFile << offset << "<identifier> " << tokenizer.identifier() << " </identifier>\n";
	tokenizer.advance();
}

bool CompilationEngine::isType()
{
	return (tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::INT
			|| tokenizer.keyword() == TokenKeyword::CHAR
			|| tokenizer.keyword() == TokenKeyword::BOOLEAN))
		|| tokenizer.tokenType() == TokenType::IDENTIFIER;
}

bool CompilationEngine::isSubroutine()
{
	return tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::CONSTRUCTOR
			|| tokenizer.keyword() == TokenKeyword::FUNCTION
			|| tokenizer.keyword() == TokenKeyword::METHOD);
}

bool CompilationEngine::isVarDec()
{
	return tokenizer.tokenType() == TokenType::KEYWORD
		&& tokenizer.keyword() == TokenKeyword::VAR;
}

bool CompilationEngine::isStatement()
{
	return tokenizer.tokenType() == TokenType::KEYWORD
		&& (tokenizer.keyword() == TokenKeyword::LET
			|| tokenizer.keyword() == TokenKeyword::IF
			|| tokenizer.keyword() == TokenKeyword::WHILE
			|| tokenizer.keyword() == TokenKeyword::DO
			|| tokenizer.keyword() == TokenKeyword::RETURN);
}

bool CompilationEngine::isOp()
{
	return tokenizer.tokenType() == TokenType::SYMBOL
		&& OP_STRING.find(tokenizer.symbol()) != string::npos;
}

bool CompilationEngine::isTerm()
{
	return tokenizer.tokenType() == TokenType::INT_CONST
		|| tokenizer.tokenType() == TokenType::STRING_CONST
		|| tokenizer.tokenType() == TokenType::IDENTIFIER
		|| (tokenizer.tokenType() == TokenType::SYMBOL
			&& (tokenizer.symbol() == '-'
				|| tokenizer.symbol() == '~'
				|| tokenizer.symbol() == '('))
		|| (tokenizer.tokenType() == TokenType::KEYWORD
			&& (tokenizer.keyword() == TokenKeyword::TRUE
				|| tokenizer.keyword() == TokenKeyword::FALSE
				|| tokenizer.keyword() == TokenKeyword::NUL
				|| tokenizer.keyword() == TokenKeyword::THIS));
}
