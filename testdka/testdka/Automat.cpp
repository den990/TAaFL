#include "Automat.h"

Automat::Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const std::map<std::pair<size_t, std::string>, std::vector<std::pair<std::string, std::string>>>& stateMap, const Grammar grammar)
	: m_output(output)
	, m_inputSize(inputSize)
	, m_stateCount(stateCount)
	, m_stateMap(stateMap)
	, m_grammar(grammar)
{
}

bool Automat::isUniqueVector(std::vector<std::vector<std::string>>& columnVector, std::vector<std::vector<std::string>>& vectorPush)
{
	bool isPush = false;
	for (auto& column : columnVector)
	{
		if (!column.empty())
		{
			isPush = true;
			std::sort(column.begin(), column.end());
			column.erase(std::unique(column.begin(), column.end()), column.end());

			if (std::find(vectorPush.begin(), vectorPush.end(), column) == vectorPush.end())
			{
				if (m_grammar == Grammar::RIGHT && column[0] == SYMBOL_F)
				{
					continue;
				}
				vectorPush.push_back(column);
			}
		}
	}
	return isPush;
}

void Automat::Determination()
{
	if (m_grammar == Grammar::RIGHT)
	{
		AddDeterminationRight();
		DeterminationRightGrammar();
	}
	else if (m_grammar == Grammar::LEFT)
	{
		AddDeterminationLeft();
		DeterminationLeftGrammar();
	}
}

void Automat::AddDeterminationRight()
{
	for (auto it = m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		std::vector<std::pair<std::string, std::string>> temporary;
		m_noTermimalKeyString.emplace((*it).first.second, m_noTermimalKeyString.size());
		m_noTermimalKeySize_t.emplace(m_noTermimalKeySize_t.size(), (*it).first.second);

		for (const auto& column : (*it).second)
		{
			std::string str = column.second[0] ? column.second : SYMBOL_F;
			temporary.push_back(std::make_pair(column.first, str));
			m_termimalKeyString.emplace(column.first, m_termimalKeyString.size());
		}

		m_inputState.push_back(temporary);
	}

	m_inputState.push_back({});

	m_noTermimalKeyString.emplace(SYMBOL_F, m_noTermimalKeyString.size());

	for (auto it = m_termimalKeyString.begin(); it != m_termimalKeyString.end(); ++it)
	{
		m_termimalKeySize_t.emplace((*it).second, (*it).first);
	}
}

void Automat::DeterminationRightGrammar()
{
	m_determinationState.push_back({ m_noTermimalKeySize_t[0] });

	for (size_t i = 0; i < m_determinationState.size(); ++i)
	{
		std::vector<std::vector<std::string>> temporary(m_inputSize);

		for (const auto& column : m_determinationState[i])
		{
			for (const auto& cell : m_inputState[m_noTermimalKeyString[column]])
			{
				auto index = m_termimalKeyString[cell.first];
				std::string str = cell.second[0] ? cell.second : SYMBOL_F;
				temporary[index].push_back(str);
			}
		}

		if (!m_determinationState[i].empty() && isUniqueVector(temporary, m_determinationState))
		{
			m_outputState.push_back(temporary);
		}
	}
}

void Automat::AddDeterminationLeft()
{
	m_noTermimalKeyString.emplace(SYMBOL_I, m_noTermimalKeyString.size());
	for (auto it = m_stateMap.rbegin(); it != m_stateMap.rend(); ++it)
	{
		std::vector<std::pair<std::string, std::string>> temporary;
		m_noTermimalKeyString.emplace((*it).first.second, m_noTermimalKeyString.size());
		m_noTermimalKeySize_t.emplace(m_noTermimalKeySize_t.size(), (*it).first.second);

		for (const auto& column : (*it).second)
		{
			std::string str = column.first[0] ? column.first : SYMBOL_I;
			temporary.push_back(std::make_pair(str, column.second));
			m_termimalKeyString.emplace(column.second, m_termimalKeyString.size());
		}

		m_inputState.push_back(temporary);
	}

	for (auto it = m_termimalKeyString.begin(); it != m_termimalKeyString.end(); ++it)
	{
		m_termimalKeySize_t.emplace((*it).second, (*it).first);
	}
}

void Automat::DeterminationLeftGrammar()
{
	std::vector<std::vector<std::vector<std::string>>> outputStateTemporary(m_stateMap.size() + 1);

	for (auto& column : outputStateTemporary)
	{
		column.resize(m_inputSize);
	}

	for (size_t i = 0; i < m_inputState.size(); ++i)
	{
		for (const auto& column : m_inputState[i])
		{
			auto index = m_noTermimalKeyString[column.first];
			auto index2 = m_termimalKeyString[column.second];

			outputStateTemporary[index][index2].push_back(m_noTermimalKeySize_t[i]);
		}
	}

	m_determinationState.push_back({ SYMBOL_I });

	for (size_t i = 0; i < m_determinationState.size(); ++i)
	{
		std::vector<std::vector<std::string>> temporary(m_inputSize);

		for (const auto& column : m_determinationState[i])
		{
			auto element = outputStateTemporary[m_noTermimalKeyString[column]];

			for (size_t j = 0; j < element.size(); ++j)
			{
				temporary[j].insert(temporary[j].end(), element[j].begin(), element[j].end());
			}
		}

		if (!m_determinationState[i].empty() && isUniqueVector(temporary, m_determinationState))
		{
			m_outputState.push_back(temporary);
		}
	}
}

std::vector<std::string> Automat::ConvertVertexString()
{
	std::vector<std::string> vertexString;
	for (const auto& column : m_determinationState)
	{
		std::string str;
		for (const auto& cell : column)
		{
			str += cell;
		}
		vertexString.push_back(str);
	}
	return vertexString;
}

std::vector<std::pair<std::string, std::string>> Automat::ConvertStateString()
{
	std::vector<std::pair<std::string, std::string>> stateString;
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputState)
		{
			std::string str1, str2;
			for (const auto& cell : column[i])
			{
				str1 += cell;
				str2 = m_termimalKeySize_t[i] == END ? "END" : m_termimalKeySize_t[i];
			}
			stateString.push_back(std::make_pair(str1, str2));
		}
	}
	return stateString;
}

void Automat::GraphView()
{
	std::vector<std::pair<std::string, std::string>> stateString = ConvertStateString();

	std::vector<std::string> vertexString = ConvertVertexString();

	std::ofstream outputGraph("outputGraph.dot");

	outputGraph << "digraph G {" << std::endl;

	for (const auto& vertex : vertexString)
	{
		outputGraph << vertex << ";" << std::endl;
	}

	for (size_t i = 0; i < m_outputState.size(); ++i)
	{
		size_t index = i;
		for (size_t j = 0; j < m_inputSize; ++j)
		{
			if (!stateString[index].second.empty())
			{
				outputGraph << vertexString[i] << "->" << stateString[index].first << " [label=" << stateString[index].second << "];" << std::endl;
			}

			index += m_outputState.size();
		}
	}

	outputGraph << "}" << std::endl;
	std::string command = "dot -Tpng outputGraph.dot -o outputGraph.png";
	system(command.c_str());

}

void Automat::PrintInfo() const
{
	for (size_t i = 0; i < m_inputSize; ++i)
	{
		for (const auto& column : m_outputState)
		{
			for (const auto& cell : column[i])
			{
				m_output << cell;
			}

			if (column[i].empty())
			{
				m_output << "-";
			}

			m_output << " ";
		}

		m_output << std::endl;
	}
}