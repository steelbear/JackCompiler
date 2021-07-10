#pragma once

#include "JackTokenizer.h"
#include "CompilationEngine.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

class JackAnalyzer
{
	vector<path> files;

public:
	JackAnalyzer(string filepath);
	void compile(bool xmlTest);
};

