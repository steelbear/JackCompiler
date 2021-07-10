#include "JackAnalyzer.h"

int main(int argc, char* argv[])
{
	string filePath;

#ifdef _DEBUG
	cin >> filePath;

	JackAnalyzer jackAnalyzer(filePath);
	jackAnalyzer.compile(true);
#else
	if (argc > 1)
	{
		filePath = string(argv[1]);

		JackAnalyzer jackAnalyzer(filePath);
		jackAnalyzer.compile(true);
	}
	else
	{
		cerr << "Error: Input file path or directory path" << endl;
	}
#endif
}