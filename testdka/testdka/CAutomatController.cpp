#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_grammar = Grammar::UNKNOWN;
}

void CAutomatController::ProcessingCommand()
{
	size_t inputSize, stateCount;

	m_input >> inputSize >> stateCount;

	try
	{
		FillingData(inputSize, stateCount);

		Automat automat(m_output, inputSize, stateCount, m_stateMap, m_grammar);
		automat.Determination();
		automat.PrintInfo();
		automat.GraphView();
	}
	catch (const std::exception& error)
	{
		m_output << error.what();
	}
}

std::pair<std::string, std::string> CAutomatController::ParseState(const std::string str) const
{
	std::smatch match;
	std::pair<std::string, std::string> pairStr;

	if (m_grammar == Grammar::RIGHT)
	{
		if (std::regex_search(str, match, PARSE_NO_TERMINAL_REGEX))
		{
			pairStr = std::make_pair(match[0], match.suffix());
		}
		else if (std::regex_search(str, match, PARSE_END_STATE_REGEX))
		{
			pairStr = std::make_pair(match[0], "");
		}
	}
	else if (m_grammar == Grammar::LEFT)
	{
		if (std::regex_search(str, match, PARSE_TERMINAL_REGEX))
		{
			pairStr = std::make_pair(match[0], match.suffix());
		}
		else if (std::regex_search(str, match, PARSE_NO_TERMINAL_REGEX))
		{
			pairStr = std::make_pair("", match[0]);
		}
	}

	return pairStr;
}

void CAutomatController::DefinitionGrammar(const std::string str)
{
	m_grammar = str[0] >= 65 && str[0] <= 90 ? Grammar::LEFT : Grammar::RIGHT;
}

void CAutomatController::FillingData(const size_t inputSize, const size_t stateCount)
{
	std::string str, str1;

	for (size_t i = 0; i < stateCount; ++i)
	{
		bool isNext = true, isCheckGrammar = false;
		m_input >> str;

		std::vector<std::pair<std::string, std::string>> temporary;

		while (isNext)
		{
			m_input >> str1;
			if (i == 0 && !isCheckGrammar)
			{
				DefinitionGrammar(str1);
				isCheckGrammar = true;
			}

			temporary.push_back(ParseState(str1));

			if (m_input.get() == '\n' || m_input.eof())
			{
				isNext = false;
			}
		}

		m_stateMap.emplace(std::make_pair(i, str), temporary);
	}
}