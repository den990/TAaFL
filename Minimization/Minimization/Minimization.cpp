#include <iostream>
#include <locale>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

void ReadAndParsingData(std::istream& fileInput, std::vector<std::string>& arrayString)
{
	size_t position;
	std::string inputString;
	while (!fileInput.eof())
	{
		std::string inputString;
		getline(fileInput, inputString, ';');
		if ((position = inputString.find("\n")) != std::string::npos)
		{
			arrayString.push_back(inputString.substr(0, position));
			arrayString.push_back(inputString.substr(position + 1, inputString.length() - 1));
		}
		else
			arrayString.push_back(inputString);
	}
}

int CalculationVertex(std::vector<std::string>& arrayString)
{
	char nameVertex = arrayString[1][0];
	for (size_t i = 1; i < arrayString.size(); i++)
	{
		if (nameVertex != arrayString[i][0])
		{
			return i - 1;
		}
	}
	return 0;
}

int CalculationX(std::vector<std::string>& arrayString)
{
	int count = 0;
	for (size_t i = 0; i < arrayString.size(); i++)
	{
		if (arrayString[i].find('x') != std::string::npos)
		{
			count++;
		}
	}
	return count;
}

void FillUniqueVertex(std::set<std::string>& uniqueVertex, size_t count, std::vector<std::string>& arrayString)
{
	for (size_t i = count; i < arrayString.size(); i++)
	{
		if (i % count == 0)
		{
			continue;
		}
		if (arrayString[i] != "-")
			uniqueVertex.insert(arrayString[i]);
	}
}

void FillNewVertex(std::set<std::string>& uniqueVertex, std::map<std::string, std::string>& newVertex)
{
	int i = 0;
	for (auto it = uniqueVertex.begin(); it != uniqueVertex.end(); ++it)
	{
		newVertex["q" + std::to_string(i)] = *it;
		i++;
	}
}

void FillOutputMoore(std::vector<std::vector<std::string>>& outMatrix, std::map<std::string, std::string>& newVertex, std::map<std::string, std::vector<std::string>>& uniqueMap, int countVertex, int countX)
{
	for (size_t i = 0; i < newVertex.size(); i++)
	{
		outMatrix[1][i] = 'q' + std::to_string(i);
	}
	int i = 0;
	for (auto it = newVertex.begin(); it != newVertex.end(); ++it)
	{
		size_t position = it->second.find("/");
		outMatrix[0][i] = it->second.substr(position + 1, it->second.length());
		i++;
	}
	i = 0;
	int m = countX;
	for (auto it = newVertex.begin(); it != newVertex.end(); ++it)
	{
		size_t position = it->second.find("/");
		std::string substr = it->second.substr(0, position);
		for (size_t k = 0; k < uniqueMap[substr].size(); k++)
		{
			for (auto it2 = newVertex.begin(); it2 != newVertex.end(); ++it2)
			{
				if (uniqueMap[substr][k] == "")
				{
					m++;
					break;
				}
				if (it2->second.find(uniqueMap[substr][k]) != std::string::npos)
				{
					outMatrix[m][i] = it2->first;
					m++;

				}
			}
		}
		i++;
		m = countX;;
	}
}

void FillMapMilli(std::map<std::string, std::vector<std::string>>& uniqueMap, std::vector<std::string>& arrayString, int countVertex, int countX)
{
	for (size_t i = 0; i < countVertex - 1; i++)
	{
		for (size_t m = 1; m <= countX; m++)
		{
			if (arrayString[(i + 1) + (countVertex * m)] != "-")
				uniqueMap[arrayString[i + 1]].push_back(arrayString[(i + 1) + (countVertex * m)]);
			else
				uniqueMap[arrayString[i + 1]].push_back("");
		}
	}
}

void ReverseNewVertex(std::map<std::string, std::string>& newVertex, std::map<std::string, std::string>& reverseNewVertex)
{
	for (auto it : newVertex)
	{
		reverseNewVertex[it.second] = it.first;
	}
}

void PrintOutput(std::ostream& out, std::vector<std::vector<std::string>>& outMatrix, int type)
{
	for (size_t i = 0; i < outMatrix.size(); i++)
	{
		if (type == 1)
		{
			if (i < 2)
			{
				out << ";";
			}
			else
			{
				out << "x" << i - 1 << ";";
			}
		}
		if (type == 2)
		{
			if (i < 1)
			{
				out << ";";
			}
			else
			{
				out << "x" << i << ";";
			}
		}
		for (size_t m = 0; m < outMatrix[i].size(); m++)
		{
			out << outMatrix[i][m] << ";";
		}
		out << std::endl;
	}
}

void DrawMoore(std::vector<std::vector<std::string>>& outMatrix, int size)
{
	std::ofstream moore("moore.dot");
	moore << "digraph G {" << std::endl;
	for (size_t m = 2; m < outMatrix.size(); m++)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (outMatrix[m][i] != "")
				moore << outMatrix[1][i] << " -> " << outMatrix[m][i] << " [label=\"x" << std::to_string(m - 1) << "\"];" << std::endl;
		}
	}
	moore << "}" << std::endl;
	std::string command = "dot -Tpng moore.dot -o moore.png";
	system(command.c_str());
}

void FillOutputMilli(int countX, int countVertex, std::vector<std::string>& arrayString, std::vector < std::vector<std::string>>& outMatrix, std::map<std::string, std::string>& uniqueMap)
{
	for (size_t i = 0; i < countVertex; i++)
	{
		outMatrix[0][i] = "q" + std::to_string(i);
	}
	int count = 0;
	int count1 = 1;
	for (size_t i = (countVertex + 1) * 2; i < arrayString.size(); i++)
	{
		if (i % (countVertex + 1) == 0)
		{
			continue;
		}
		if (arrayString[i] != "-")
		{
			outMatrix[count1][count] = arrayString[i] + uniqueMap[arrayString[i]];
		}
		count++;
		if (count >= countVertex)
		{
			count1++;
			count = 0;
		}
	}
}
void FillMapMoore(std::map<std::string, std::string>& uniqueMap, std::vector<std::string>& arrayString, int countVertex)
{
	int max = countVertex * 2 + 2;
	for (size_t i = countVertex + 2; i < max; i++)
	{
		uniqueMap[arrayString[i]] = arrayString[i - countVertex - 1];
	}
}
void DrawMilli(std::vector < std::vector<std::string>>& outMatrix, int size)
{
	std::ofstream milli("milli.dot");
	milli << "digraph G {" << std::endl;
	for (size_t m = 1; m < outMatrix.size(); m++)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (outMatrix[m][i] != "")
			{
				size_t position = outMatrix[m][i].find("y");
				milli << outMatrix[0][i] << " -> " << outMatrix[m][i].substr(0, position) << " [label=\"x" << std::to_string(m) << "\"];" << std::endl;
			}
		}
	}
	milli << "}" << std::endl;
	std::string command = "dot -Tpng milli.dot -o milli.png";
	system(command.c_str());
}
//функции для 2 лабы

void FillUniqueTransitions(std::map<std::string, std::vector<std::string>>& uniqueMap, std::vector<std::vector<std::string>>& uniqueTransitions)
{
	for (auto it = uniqueMap.begin(); it != uniqueMap.end(); it++)
	{
		std::vector<std::string> transition;
		for (size_t m = 0; m < it->second.size(); m++)
		{
			size_t position = it->second.at(m).find("/");
			if (position != std::string::npos)
			{
				transition.push_back(it->second.at(m).substr(position + 1));

			}
		}
		uniqueTransitions.push_back(transition);
	}
}

std::vector<std::vector<std::string>> DeleteOdinakoviye(std::vector<std::vector<std::string>>& uniqueTransitions)
{
	std::vector<std::vector<std::string>> uniqueResult;

	for (const std::vector<std::string>& transition : uniqueTransitions) {
		// Проверяем, есть ли такой массив в uniqueResult
		if (std::find(uniqueResult.begin(), uniqueResult.end(), transition) == uniqueResult.end()) {
			// Если не нашли, то добавляем его в uniqueResult
			uniqueResult.push_back(transition);
		}
	}
	return uniqueResult;
}

void AllocationVertex(std::vector<std::vector<std::string>>& uniqueTransitions, std::map<std::string, std::vector<std::string>>& uniqueMap, std::map<std::string, std::vector<std::string>>& table)
{
	char ch = 'A';
	int index = 1;
	for (size_t i = 0; i < uniqueTransitions.size(); i++)
	{
		std::vector<std::string > pushVertex;
		for (auto it = uniqueMap.begin(); it != uniqueMap.end(); it++)
		{
			bool skip = false;
			for (size_t m = 0; m < it->second.size(); m++)
			{
				size_t position = it->second.at(m).find("/");
				if (uniqueTransitions[i][m] != it->second.at(m).substr(position + 1))
				{
					skip = true;
				}
			}
			if (!skip)
			{
				pushVertex.push_back(it->first);
			}
		}
		table["A" + std::to_string(index)] = pushVertex;
		index++;
	}
}

void FillNewTable(std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableNew, std::map<std::string, std::vector<std::string>>& table, std::map<std::string, std::vector<std::string>>& uniqueMap)
{
	for (auto it = table.begin(); it != table.end(); it++)
	{
		int index = 0;
		std::pair < std::string, std::vector<std::string>> pushPair;
		std::vector<std::pair < std::string, std::vector<std::string>>> pushVector;
		for (size_t i = 0; i < it->second.size(); i++)
		{
			std::vector<std::string> findVertex = uniqueMap[it->second.at(index)];
			std::vector<std::string> saveVertex;
			for (size_t m = 0; m < findVertex.size(); m++)
			{
				size_t position = findVertex[m].find("/");
				std::string vertex = findVertex[m].substr(0, position);
				for (auto it1 = table.begin(); it1 != table.end(); it1++)
				{
					for (size_t k = 0; k < it1->second.size(); k++)
					{

						if (it1->second.at(k).find(vertex) != std::string::npos)
						{
							if (it1->second.at(k) == vertex)
							{
								saveVertex.push_back(it1->first);
							}
						}
					}
				}
			}
			pushPair = { it->second.at(index), {saveVertex} };
			pushVector.push_back(pushPair);
			index++;
		}

		tableNew[it->first] = pushVector;
	}
}

bool custom_sort(const std::pair<std::string, std::vector<std::string>>& p1, const std::pair<std::string, std::vector<std::string>>& p2) {
	if (p1.second.at(0) == p2.second.at(0))
	{
		if (p1.second.size() > 1 && p2.second.size() > 1)
		{
			return p1.second.at(1) > p2.second.at(1);
		}
	}
	else
	{
		return p1.second.at(0) > p2.second.at(0);
	}
}

std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> CreateNewTable(std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& table, char& symbol)
{
	for (auto& entry : table) {
		// Сортируем вектор пар внутри значения мапы
		std::sort(entry.second.rbegin(), entry.second.rend(), custom_sort);
	}
	symbol += 1;
	int index = 1;
	std::string strChar(1, symbol);

	std::map < std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> newTable;
	for (auto it = table.begin(); it != table.end(); it++) {
		std::set<std::pair<std::string, std::vector<std::string>>> mySet;
		if (it->second.size() > 1) {
			for (size_t i = 0; i < it->second.size(); i++) {
				if (mySet.count({ it->first, it->second.at(i).second }) > 0) {
					if (newTable.find(strChar + std::to_string(index - 1)) == newTable.end()) {
						newTable[strChar + std::to_string(index - 1)] = { {it->second.at(i).first, {}} };
					}
					else {
						newTable[strChar + std::to_string(index - 1)].push_back({ it->second.at(i).first, {} });
					}
				}
				else {
					mySet.insert({ it->first, it->second.at(i).second });
					newTable[strChar + std::to_string(index)] = { {it->second.at(i).first, {}} };
					index++;
				}
			}
		}
		else {
			newTable[strChar + std::to_string(index)] = { {it->second[0].first, {}} };
			index++;
		}
	}
	return newTable;
}

void AdditionNewTable(std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableNew, std::map<std::string, std::vector<std::string>>& uniqueMap)
{
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		int index = 0;
		std::pair < std::string, std::vector<std::string>> pushPair;
		std::vector<std::pair < std::string, std::vector<std::string>>> pushVector;
		for (size_t i = 0; i < it->second.size(); i++)
		{
			std::vector<std::string> findVertex = uniqueMap[it->second.at(index).first];
			std::vector<std::string> saveVertex;
			for (size_t m = 0; m < findVertex.size(); m++)
			{
				size_t position = findVertex[m].find("/");
				std::string vertex = findVertex[m].substr(0, position);
				for (auto it1 = tableNew.begin(); it1 != tableNew.end(); it1++)
				{
					for (size_t k = 0; k < it1->second.size(); k++)
					{

						if (it1->second.at(k).first.find(vertex) != std::string::npos)
						{
							if (it1->second.at(k).first == vertex)
							{
								saveVertex.push_back(it1->first);
							}
						}
					}
				}
			}
			pushPair = { it->second.at(index).first, {saveVertex}};
			pushVector.push_back(pushPair);
			index++;
		}
		tableNew[it->first] = pushVector;
	}
}

void Minimization(std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableNew, std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableOld, std::map<std::string, std::vector<std::string>>& uniqueMap)
{
	bool go = true;
	char symbol = 'A';
	while (tableNew.size() < uniqueMap.size() && go)
	{
		tableOld = tableNew;
		tableNew = CreateNewTable(tableNew, symbol);
		AdditionNewTable(tableNew, uniqueMap);
		if (tableOld.size() == tableNew.size())
		{
			go = false;
			continue;
		}
	}
	if (tableNew.size() == uniqueMap.size())
	{
		std::cout << "Automata has been minimized already" << std::endl;
	}
}

void FillOutputMilli1(std::vector<std::vector<std::string>>& outMatrix, std::map<std::string, std::vector<std::string>>& uniqueMap, std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableNew)
{
	int index = 0;
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		outMatrix[0][index] = 'S' + it->first.substr(1);
		index++;
	}
	int index1 = 0;
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		
		std::pair<std::string, std::vector<std::string>> pair = it->second[0];
		std::string vertex = pair.first;
		std::vector<std::string> vectorVertexs = uniqueMap[vertex];
		for (size_t m = 0; m < vectorVertexs.size(); m++)
		{
			size_t position = vectorVertexs[m].find("/");
			for (auto it1 = tableNew.begin(); it1 != tableNew.end(); it1++)
			{
				for (size_t l = 0; l < it1->second.size(); l++)
				{
					if (it1->second[l].first == vectorVertexs[m].substr(0, position))
					{
						std::string outVertex = it1->first;
							outVertex[0] = 'S';
							outMatrix[m + 1][index1] = outVertex + vectorVertexs[m].substr(position + 1);
							break;
					}
				}
			}
		}
		index1++;
	}
}

void Milli(std::ofstream& fileOutput, std::ifstream& fileInput)
{
	std::vector<std::string> arrayString;
	ReadAndParsingData(fileInput, arrayString);
	size_t countVertex = CalculationVertex(arrayString);
	size_t countX = CalculationX(arrayString);
	std::set<std::string> uniqueVertex;
	FillUniqueVertex(uniqueVertex, countVertex + 1, arrayString);
	std::map<std::string, std::vector<std::string>> uniqueMap;
	FillMapMilli(uniqueMap, arrayString, countVertex + 1, countX);
	std::vector<std::vector<std::string>> uniqueTransitions;
	FillUniqueTransitions(uniqueMap, uniqueTransitions);
	uniqueTransitions = DeleteOdinakoviye(uniqueTransitions);//остановился на этом, получиили уникальные y
	std::map<std::string, std::vector<std::string>> table;
	AllocationVertex(uniqueTransitions, uniqueMap, table);
	std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> tableNew, tableOld;
	FillNewTable(tableNew, table, uniqueMap);
	Minimization(tableNew, tableOld, uniqueMap);
	std::vector<std::vector<std::string>> outMatrix(countX + 1, std::vector<std::string>(tableNew.size()));
	FillOutputMilli1(outMatrix, uniqueMap, tableNew);
	PrintOutput(fileOutput, outMatrix, 2);
	DrawMilli(outMatrix, tableNew.size());
}

void FillUniqueTransitionsMoore(std::map<std::string, std::string>& uniqueMap, std::vector<std::string>& uniqueTransitions)
{
	for (auto it = uniqueMap.begin(); it != uniqueMap.end(); it++)
	{
		if (std::find(uniqueTransitions.begin(), uniqueTransitions.end(), it->second) == uniqueTransitions.end())
		{
			uniqueTransitions.push_back(it->second);
		}
	}
}

void AllocationVertexMoore(std::map<std::string, std::string>& uniqueMap, std::vector<std::string>& uniqueTransitions, std::map<std::string, std::vector<std::string>>& table)
{
	int index = 1;
	for (size_t i = 0; i < uniqueTransitions.size(); i++)
	{
		std::vector<std::string> vertexes;
		for (auto it = uniqueMap.begin(); it != uniqueMap.end(); it++)
		{
			if (it->second == uniqueTransitions[i])
			{
				vertexes.push_back(it->first);
			}
		}
		table['A' + std::to_string(index)] = vertexes;
		index++;
	}
}

void FillOutputMoore1(std::vector<std::vector<std::string>>& outMatrix, std::map<std::string, std::vector<std::string>>& uniqueMapWithTransition, std::map<std::string, std::string>& uniqueMap, std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>>& tableNew)
{
	int index = 0;
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		outMatrix[1][index] = 'S' + it->first.substr(1);
		index++;
	}
	index = 0;
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		std::string vertex = it->second[0].first;
		outMatrix[0][index] = uniqueMap[vertex];
		index++;
	}
	index = 0;
	for (auto it = tableNew.begin(); it != tableNew.end(); it++)
	{
		std::vector<std::string> vertexes = it->second[0].second;
		for (size_t i = 0; i < vertexes.size(); i++)
		{
			outMatrix[i + 2][index] = "S" + vertexes[i].substr(1);
		}
		index++;
	}
}

void Moore(std::ofstream& fileOutput, std::ifstream& fileInput)
{
	std::vector<std::string> arrayString;
	ReadAndParsingData(fileInput, arrayString);
	size_t countVertex = CalculationVertex(arrayString);
	size_t countX = CalculationX(arrayString);
	std::map<std::string, std::string> uniqueMap;
	std::map<std::string, std::vector<std::string>> uniqueMapWithTransition;
	FillMapMoore(uniqueMap, arrayString, countVertex);
	arrayString.erase(arrayString.begin(), arrayString.begin() + countVertex + 1);
	FillMapMilli(uniqueMapWithTransition, arrayString, countVertex + 1, countX);
	std::vector<std::string> uniqueTransitions;
	FillUniqueTransitionsMoore(uniqueMap, uniqueTransitions);
	std::map<std::string, std::vector<std::string>> table;
	AllocationVertexMoore(uniqueMap, uniqueTransitions, table);
	std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> tableNew, tableOld;
	FillNewTable(tableNew, table, uniqueMapWithTransition);
	Minimization(tableNew, tableOld, uniqueMapWithTransition);
	std::vector<std::vector<std::string>> outMatrix(countX + 2, std::vector<std::string>(tableNew.size()));
	FillOutputMoore1(outMatrix, uniqueMapWithTransition, uniqueMap, tableNew);
	PrintOutput(fileOutput, outMatrix, 2);
	DrawMoore(outMatrix, tableNew.size());
}

int ValidationArguments(int argc)
{
	if (argc < 3)
	{
		std::cout << "Error, please enter \"task.exe mealy2moore input.csv output.csv\" or \"task.exe moore2mealy input.csv output.csv\" " << std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "RU");
	//if (ValidationArguments(argc) == 1)
	//{
	//    return 1;
	//}
	std::ofstream fileOutput("output.csv");
	std::ifstream fileInput("input.csv");
	if (!fileInput)
	{
		std::cout << "Файл не открыт" << std::endl;
		return 1;
	}
	if (!fileOutput)
	{
		std::cout << "Файл не открыт" << std::endl;
		return 1;
	}
	Milli(fileOutput, fileInput);
	//Moore(fileOutput, fileInput);
	//if (strcmp(argv[1], "moore2mealy") == 0)
	//{
	//	MooreMilli(fileOutput, fileInput);
	//	return 0;
	//}
}

