#include "Header.h"
#include "CAutomatController.h"
#include <iostream>

int main(int argc, char* argv[])
{


	std::ifstream fileInput("input.txt");
	std::ofstream fileOutput("output.txt");

	if (!fileInput.is_open())
	{
		std::cerr << ERROR_FILE_NOT_EXIST << std::endl;
		return 1;
	}

	CAutomatController automatController(fileInput, fileOutput);
	automatController.ProcessingCommand();
}