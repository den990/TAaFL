#pragma once
#include "Automat.h"
#include <sstream>

const std::regex PARSE_TERMINAL_REGEX("[A-Z]+");
const std::regex PARSE_NO_TERMINAL_REGEX("(\\d+)|([a-z]+)");
const std::regex PARSE_END_STATE_REGEX("[#]+");

class CAutomatController
{
public:
	CAutomatController(std::istream& input, std::ostream& output);

	void ProcessingCommand();

private:
	std::istream& m_input;
	std::ostream& m_output;

	std::map<std::pair<size_t, std::string>, std::vector<std::pair<std::string, std::string>>> m_stateMap;
	Grammar m_grammar;

	void FillingData(const size_t inputSize, const size_t stateCount);
	std::pair<std::string, std::string> ParseState(const std::string str) const;
	void DefinitionGrammar(const std::string str);
};