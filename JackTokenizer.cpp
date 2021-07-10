#include "JackTokenizer.h"

const string JackTokenizer::keywordList[21] = {
	string("class"),
	string("method"),
	string("function"),
	string("constructor"),
	string("int"),
	string("boolean"),
	string("char"),
	string("void"),
	string("var"),
	string("static"),
	string("field"),
	string("let"),
	string("do"),
	string("if"),
	string("else"),
	string("while"),
	string("return"),
	string("true"),
	string("false"),
	string("null"),
	string("this"),
};

JackTokenizer::JackTokenizer(ifstream& inputFile) : inputFile(inputFile), row(1), col(1), isLineBreak(false)
{
}

bool JackTokenizer::hasMoreTokens()
{
	bool blankCommandCheck = true;
	char currentCharacter;

	while (!inputFile.eof() && blankCommandCheck)
	{
		blankCommandCheck = false;
		currentCharacter = inputFile.peek();

		if (currentCharacter == '/')
		{
			ignore();
			if (inputFile.peek() == '/')
			{
				while (!inputFile.eof() && inputFile.peek() != '\n')
				{
					ignore();
				}
				ignore();
				blankCommandCheck = true;
			}
			else if (inputFile.peek() == '*')
			{
				char nextCharacter = get();
				while (!inputFile.eof() && !(nextCharacter == '*' && inputFile.peek() == '/'))
				{
					nextCharacter = get();
				}
				ignore();
				blankCommandCheck = true;
			}
			else
			{
				inputFile.putback('/');
			}
		}
		else if (isblank(currentCharacter) || currentCharacter == ' ' || currentCharacter == '\n')
		{
			while (!inputFile.eof() && (isblank(currentCharacter) || currentCharacter == ' ' || currentCharacter == '\n'))
			{
				ignore();
				currentCharacter = inputFile.peek();
			}
			blankCommandCheck = true;
		}
	}

	return inputFile.is_open() && (current.empty() || !inputFile.eof());
}

void JackTokenizer::advance()
{
	if (hasMoreTokens())
	{
		current.clear();
		current.push_back(get());

		if (current[0] == '\"')
		{
			char nextCharacter = inputFile.peek();
			while (nextCharacter != '\"')
			{
				current.push_back(get());
				nextCharacter = inputFile.peek();
			}
			current.push_back(get());

			//cout << "advance : string (" << current << ")" << endl;
		}
		else if (SYMBOL_STRING.find(current[0]) != string::npos)
		{
			//cout << "advance : symbol (" << current << ")" << endl;
		}
		else if (isdigit(current[0]))
		{
			char nextCharacter = inputFile.peek();
			while (isdigit(nextCharacter))
			{
				current.push_back(get());
				nextCharacter = inputFile.peek();
			}

			//cout << "advance : intVal (" << current << ")" << endl;
		}
		else if (current[0] == '_' || isalpha(current[0]))
		{
			char nextCharacter = inputFile.peek();
			while (current[0] == '_' || isalnum(nextCharacter))
			{
				current.push_back(get());
				nextCharacter = inputFile.peek();
			}

			//cout << "advance : keyword or identifier (" << current << ")" << endl;
		}
		else
		{
			cerr << "Error : Cannot read file - invaild characters included (" << current << ")" << endl;
		}
	}
}

TokenType JackTokenizer::tokenType()
{
	if (isdigit(current[0]))
	{
		//cout << "tokenType : int_const" << endl;
		return TokenType::INT_CONST;
	}
	else if (current[0] == '\"')
	{
		//cout << "tokenType : string_const" << endl;
		return TokenType::STRING_CONST;
	}
	else if (SYMBOL_STRING.find(current[0]) != string::npos)
	{
		//cout << "tokenType : symbol" << endl;
		return TokenType::SYMBOL;
	}
	else
	{
		for (int i = 0; i < sizeof(keywordList); i++)
		{
			if (current == keywordList[i])
			{
				//cout << "tokenType : keyword" << endl;
				return TokenType::KEYWORD;
			}
		}

		//cout << "tokenType : identifier" << endl;
		return TokenType::IDENTIFIER;
	}
}

TokenKeyword JackTokenizer::keyword()
{
	for (int i = 0; i < sizeof(keywordList); i++)
	{
		if (current == keywordList[i])
		{
			return TokenKeyword(i);
		}
	}

	throw NotExpectedException("Any keyword", current, row, col);
}

char JackTokenizer::symbol()
{
	if (SYMBOL_STRING.find(current[0]) == string::npos)
	{
		throw NotExpectedException("Any symbol", current, row, col);
	}
	return current[0];
}

char JackTokenizer::get()
{
	char picked = inputFile.get();
	if (isLineBreak)
	{
		row++;
		col = 1;
		isLineBreak = false;
	}
	col++;
	if (picked == '\n')
	{
		isLineBreak = true;
	}
	return picked;
}

void JackTokenizer::ignore()
{
	char picked = inputFile.get();
	if (isLineBreak)
	{
		row++;
		col = 1;
		isLineBreak = false;
	}
	col++;
	if (picked == '\n')
	{
		isLineBreak = true;
	}
}

string JackTokenizer::identifier()
{
	return current;
}

int JackTokenizer::intVal()
{
	return stoi(current);
}

string JackTokenizer::stringVal()
{
	string result(current);
	return result.substr(1, result.length() - 2);
}

string JackTokenizer::keywordToString()
{
	return keywordList[int(keyword())];
}

long JackTokenizer::getPosRow()
{
	return row;
}

long JackTokenizer::getPosCol()
{
	return col;
}