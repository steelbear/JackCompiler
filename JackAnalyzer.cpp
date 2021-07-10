#include "JackAnalyzer.h"

JackAnalyzer::JackAnalyzer(string filepath)
{
	path file(filepath);

	if (file.empty())
	{
		cerr << "Error: Input file path" << endl;
	}
	else if (!file.has_extension())
	{
		directory_iterator dirIter(file);

		for (directory_entry dir : dirIter)
		{
			if (dir.path().extension() == ".jack")
			{
				files.push_back(dir.path().string());
			}
		}

		if (files.empty())
		{
			cerr << "Directory " << file.string() << "does not have any jack file" << endl;
		}
	}
	else
	{
		if (file.extension() == ".jack")
		{
			files.push_back(path(filepath));
		}
		else
		{
			cerr << "Error: " << file.string() << " is not jack file" << endl;
		}
	}
}

void JackAnalyzer::compile(bool xmlTest)
{
	for (path file : files)
	{
		string inputFilePathString(file.string());
		string outputFilePathString;
		if (xmlTest) {
			outputFilePathString = file.replace_extension("xml").string();
		}
		else
		{
			outputFilePathString = file.replace_extension("vm").string();
		}

		cout << "Compiling " << inputFilePathString << "... ";

		ifstream inputFile(inputFilePathString);
		ofstream outputFile;
		if (inputFile.fail())
		{
			cerr << "\nError: " << file.filename().string() << " does not exist" << endl;
		}
		else
		{
			outputFile.open(outputFilePathString);
			try
			{
				CompilationEngine compiler(inputFile, outputFile);

				cout << "Done!" << endl;
			}
			catch (NotExpectedException& e)
			{
				cerr << e.what() << endl;
			}
		}
	}
}