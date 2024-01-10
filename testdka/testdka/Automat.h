#pragma once
#include "Header.h"

enum class Grammar
{
	UNKNOWN,
	LEFT,
	RIGHT
};

class Automat
{
public:
	Automat(std::ostream& output, const size_t inputSize, const size_t stateCount, const std::map<std::pair<size_t, std::string>, std::vector<std::pair<std::string, std::string>>>& stateMap, const Grammar grammar);
	~Automat() = default;

	void GraphView();
	void Determination();
	void PrintInfo() const;

private:
	size_t m_inputSize, m_stateCount;

	Grammar m_grammar;
	std::map<std::pair<size_t, std::string>, std::vector<std::pair<std::string, std::string>>> m_stateMap;

	std::vector<std::vector<std::string>> m_determinationState;
	std::vector<std::vector<std::pair<std::string, std::string>>> m_inputState;
	std::vector<std::vector<std::vector<std::string>>> m_outputState;

	std::map<std::string, size_t> m_noTermimalKeyString, m_termimalKeyString;
	std::map<size_t, std::string> m_noTermimalKeySize_t, m_termimalKeySize_t;

	std::ostream& m_output;

	bool isUniqueVector(std::vector<std::vector<std::string>>& columnVector, std::vector<std::vector<std::string>>& vectorPush);

	void AddDeterminationRight();
	void AddDeterminationLeft();

	void DeterminationRightGrammar();
	void DeterminationLeftGrammar();

	std::vector < std::string> ConvertVertexString();
	std::vector<std::pair<std::string, std::string>> ConvertStateString();
};