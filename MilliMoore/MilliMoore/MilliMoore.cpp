#include <iostream>
#include <locale>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <set>
#include <map>
#include <cmath>

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
    for (size_t i = (countVertex + 1)*2; i < arrayString.size(); i++)
    {
        if (i % (countVertex+1) == 0)
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

void MilliMoore(std::ofstream& fileOutput, std::ifstream& fileInput)
{
    std::vector<std::string> arrayString;
    ReadAndParsingData(fileInput, arrayString);
    size_t countVertex = CalculationVertex(arrayString);
    size_t countX = CalculationX(arrayString);
    std::set<std::string> uniqueVertex;
    FillUniqueVertex(uniqueVertex, countVertex + 1, arrayString);
    std::map<std::string, std::vector<std::string>> uniqueMap;
    FillMapMilli(uniqueMap, arrayString, countVertex + 1, countX);
    std::map<std::string, std::string> newVertex;
    FillNewVertex(uniqueVertex, newVertex);
    std::map<std::string, std::string> reverseNewVertex;
    ReverseNewVertex(newVertex, reverseNewVertex);
    std::vector<std::vector<std::string>> outMatrix(countX + 2, std::vector<std::string>(newVertex.size()));
    FillOutputMoore(outMatrix, newVertex, uniqueMap, countVertex, countX);
    PrintOutput(fileOutput, outMatrix, 1);
    DrawMoore(outMatrix, newVertex.size());
}

void MooreMilli(std::ofstream& fileOutput, std::ifstream& fileInput)
{
    std::vector<std::string> arrayString;
    ReadAndParsingData(fileInput, arrayString);
    size_t countVertex = CalculationVertex(arrayString);
    size_t countX = CalculationX(arrayString);
    std::vector < std::vector<std::string>> outMatrix(countX + 1, std::vector<std::string>(countVertex));
    std::map<std::string, std::string> uniqueMap;
    FillMapMoore(uniqueMap, arrayString, countVertex);
    FillOutputMilli(countX, countVertex, arrayString, outMatrix, uniqueMap);
    PrintOutput(fileOutput, outMatrix, 2);
    DrawMilli(outMatrix, countVertex);
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
    if (ValidationArguments(argc) == 1)
    {
        return 1;
    }
    std::ofstream fileOutput(argv[3]);
    std::ifstream fileInput(argv[2]);
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
    if (strcmp(argv[1], "mealy2moore") == 0)
    {
        MilliMoore(fileOutput, fileInput);
        return 0; 
    }
    if (strcmp(argv[1], "moore2mealy") == 0)
    {
        MooreMilli(fileOutput, fileInput);
        return 0;
    }
}

