#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <unordered_map>

void ReadFromFile(std::istream& fileInput, std::unordered_map<std::string, std::vector<std::string>>& matrix)
{
	std::string str;
	while (getline(fileInput, str))
	{
		std::istringstream sstrm(str);
		std::string vertex;
		sstrm >> vertex;
		std::vector<std::string> array;
		while (sstrm >> str)
		{
			if (str == "->" || str == "|")
			{
				continue;
			}
			else
			{
				array.push_back(str);
			}
		}
		matrix[vertex] = array;
	}
}

void FindTransition(std::unordered_map<std::string, std::vector<std::string>>& matrix, std::vector<char>& transition, std::string& grammatic)
{
	for (const auto& entry : matrix)
	{
		for (const std::string& str : entry.second)
		{
			if (grammatic == "right")
			{
				if (std::find(transition.begin(), transition.end(), str[0]) == transition.end())
				{
					transition.push_back(str[0]);
				}
			}
			if (grammatic == "left")
			{
				if (std::find(transition.begin(), transition.end(), str[1]) == transition.end())
				{
					transition.push_back(str[1]);
				}
			}
		}
	}
}

void FillIntermediateMatrix(std::unordered_map<std::string, std::vector<std::unordered_map<char, std::vector<std::string>>>>& intermediateMatrix, std::unordered_map<std::string, std::vector<std::string>>& matrix,
	std::vector<char>& transition, std::string& grammatic)
{

	for (auto it = matrix.begin(); it != matrix.end(); it++)
	{
		std::vector < std::unordered_map<char, std::vector<std::string>>> vectorsTemp;
		for (size_t i = 0; i < transition.size(); i++)
		{
			std::unordered_map<char, std::vector<std::string>> mapTemp;
			std::vector<std::string> vectorTemp;
			for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
			{
				if (it1->at(0) == transition[i])
				{
					vectorTemp.push_back(std::string(1, it1->at(1)));
				}
			}
			mapTemp[transition[i]] = vectorTemp;
			vectorsTemp.push_back(mapTemp);
		}
		intermediateMatrix[it->first] = vectorsTemp;
	}

}

void Copy()
{

}

void FillFinishMatrix(std::unordered_map<std::string, std::vector<std::unordered_map<char, std::string>>>& finishMatrix,
	std::unordered_map<std::string, std::vector<std::unordered_map<char, std::vector<std::string>>>>& intermediateMatrix, std::vector<std::string>& deleteVertex)
{
	for (auto it = intermediateMatrix.begin(); it != intermediateMatrix.end(); it++)
	{
		std::vector<std::unordered_map<char, std::string>> tempString;
		for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
		{

			if (it1->begin()->second.size() > 1)
			{
				std::unordered_map<char, std::string> tempMap;
				std::string combinedString;
				for (size_t i = 0; i < it1->begin()->second.size(); i++)
				{
					combinedString += it1->begin()->second[i];
					deleteVertex.push_back(it1->begin()->second[i]);
				}
				tempMap[it1->begin()->first] = combinedString;
				tempString.push_back(tempMap);
			}
			else
			{
				if (it1->begin()->second.size() == 0)
				{
					std::unordered_map<char, std::string> tempMap;
					tempMap[it1->begin()->first] = "";
					tempString.push_back(tempMap);
				}
				else
				{
					std::unordered_map<char, std::string> tempMap;
					tempMap[it1->begin()->first] = it1->begin()->second[0];
					tempString.push_back(tempMap);
				}
			}
		}
		finishMatrix[it->first] = tempString;
		//TODO: Мы получили совместную вершину, теперь ее надо записать в finishMatrix, и совместить их. Которые совместили пропускать.
	}
}

void Copy(std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& newfinishMatrix, std::unordered_map<std::string, std::vector<std::unordered_map<char, std::string>>>& finishMatrix)
{
	for (auto& entry : newfinishMatrix) {
		const std::string& key = entry.first;
		const std::unordered_map<std::string, std::string>& innerMap = entry.second;

		// Проверяем, есть ли такой ключ в finishMatrix
		if (finishMatrix.find(key) != finishMatrix.end()) {
			for (const auto& innerEntry : innerMap) {
				const std::string& innerKey = innerEntry.first;
				const std::string& innerValue = innerEntry.second;

				// Создаем новую unordered_map в формате finishMatrix
				std::unordered_map<char, std::string> innerMapConverted;
				for (size_t i = 0; i < innerValue.length(); i++) {
					innerMapConverted[innerValue[i]] = innerKey;
				}

				// Добавляем данные в соответствующий вектор в finishMatrix
				finishMatrix[key].push_back(innerMapConverted);
			}
		}
		// Если ключа в finishMatrix нет, создаем новую запись
		else {
			std::vector<std::unordered_map<char, std::string>> newVector;
			for (const auto& innerEntry : innerMap) {
				const std::string& innerKey = innerEntry.first;
				const std::string& innerValue = innerEntry.second;

				std::unordered_map<char, std::string> innerMapConverted;
				for (size_t i = 0; i < innerValue.length(); i++) {
					innerMapConverted[innerValue[i]] = innerKey;
				}

				newVector.push_back(innerMapConverted);
			}

			finishMatrix[key] = newVector;
		}
	}
}

void RefactorFinishMatrix(std::unordered_map<std::string, std::vector<std::unordered_map<char, std::string>>>& finishMatrix,
	std::unordered_map<std::string, std::vector<std::string>>& matrix, std::vector<std::string>& deleteVertex, std::vector<char>& transition)
{
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> newfinishMatrix;
	std::unordered_map<std::string, std::string> mapTemp;
	auto saveIteration = finishMatrix.begin();
	int index = 0;
	while (1)
	{
		if (index > 0)
		{
			Copy(newfinishMatrix, finishMatrix);
			saveIteration = newfinishMatrix.begin();
		}
		index++;
		for (auto it = finishMatrix.begin(); it != finishMatrix.end(); it++)
		{
			newfinishMatrix[it->first] = std::unordered_map<std::string, std::string>();
			for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
			{
				std::unordered_map<std::string, std::string > tempMap1;
				if (!it1->begin()->second.empty() && it1->begin()->second.length() > 1)
				{
					if (!it1->begin()->second.empty())
					{
						std::string newVertex = it1->begin()->second;
						for (size_t i = 0; i < it1->begin()->second.length(); i++)
						{
							if (i == 0)
							{
								for (auto it2 = matrix.begin(); it2 != matrix.end(); it2++)
								{
									if (it2->first == std::string(1, it1->begin()->second.at(i)))
									{
										for (auto itTransition : transition)
										{
											bool add = false;
											//мб проверка на грамматику
											for (size_t l = 0; l < it2->second.size(); l++)
											{
												if (it2->second.at(l).at(0) == itTransition)
												{
													if (tempMap1.find(std::string(1, itTransition)) == tempMap1.end()) {
														// Если ключ itTransition не существует, добавляем его в tempMap1
														tempMap1[std::string(1, itTransition)] = it2->second.at(l).at(1);
													}
													else {
														// Если ключ itTransition уже существует, добавляем значение или выполняем другие действия
														// Например, можно объединить значения или выполнить другую логику
														tempMap1[std::string(1, itTransition)] += it2->second.at(l).at(1); // Просто конкатенируем значения
													}
													add = true;
												}
											}
											if (!add)
											{
												tempMap1[std::string(1, itTransition)] = "";
											}
										}
									}
								}

							}
							else
							{
								for (auto it2 = matrix.begin(); it2 != matrix.end(); it2++)
								{
									if (it2->first == std::string(1, it1->begin()->second.at(i)))
									{
										for (auto itTransition : transition)
										{
											//мб проверка на грамматику
											for (size_t l = 0; l < it2->second.size(); l++)
											{
												if (it2->second.at(l).at(0) == itTransition)
												{
													tempMap1[std::string(1, itTransition)] += it2->second.at(l).at(1);

												}
											}
										}
									}
								}
							}
						}
						newfinishMatrix[it1->begin()->second] = tempMap1;

						char keyChar = it1->begin()->first;
						std::string keyString(1, keyChar);
						mapTemp[keyString] = it1->begin()->second;
					}
				}
				else
				{
					char keyChar = it1->begin()->first;
					std::string keyString(1, keyChar);
					mapTemp[keyString] = it1->begin()->second;
				}

			}
			newfinishMatrix[it->first] = mapTemp;
		}
	}
}

int main()
{
	std::ifstream fileInput("input.txt");
	if (!fileInput)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::unordered_map<std::string, std::vector<std::string>> matrix;
	ReadFromFile(fileInput, matrix);
	std::string grammatic;
	std::cin >> grammatic;
	std::vector<char> transition;
	FindTransition(matrix, transition, grammatic);
	std::unordered_map<std::string, std::vector<std::unordered_map<char, std::vector<std::string>>>> intermediateMatrix;
	FillIntermediateMatrix(intermediateMatrix, matrix, transition, grammatic);
	std::unordered_map<std::string, std::vector<std::unordered_map<char, std::string>>> finishMatrix;
	std::vector<std::string> deleteVertex;
	FillFinishMatrix(finishMatrix, intermediateMatrix, deleteVertex);
	RefactorFinishMatrix(finishMatrix, matrix, deleteVertex, transition);
}

