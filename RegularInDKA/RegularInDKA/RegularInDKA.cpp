//Tema 1 Tehnici de compilare  Problema 2
//Costea Calin
//Nitu Mandel-Andrei


//Utilizare program: se introduce un fisier cu numele regex.txt in acelasi fisier cu executabilul si apoi se da drumul la aplicatie

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <queue>

using namespace std;

enum TypeOfCharacter { star, bar, dot, chara, paranthesisLeft, paranthesisRight };
std::vector<char> letterOnPosition;
std::set<char> letters;

std::vector<int>appendUnique(std::vector<int> left, std::vector<int> right) {
	left.insert(left.end(), right.begin(), right.end());
	std::set<int> aux(left.begin(), left.end());
	std::vector<int> result;
	result.assign(aux.begin(), aux.end());
	return result;
}

class State {
public:
	virtual bool isNullable() = 0;
	virtual std::vector<int> firstPos() = 0;
	virtual std::vector<int> lastPos() = 0;
	virtual void calculateFollowers(std::vector<std::vector<int>>& followers) {};

};

class DotState :public State {
private:
	State* stateLeft;
	State* stateRight;
public:
	DotState(State* stateLeft, State* stateRight) {
		this->stateLeft = stateLeft;
		this->stateRight = stateRight;
	}
	bool isNullable() {
		return stateLeft->isNullable() && stateRight->isNullable();
	}
	std::vector<int> firstPos() {
		return appendUnique(stateLeft->firstPos(), !stateLeft->isNullable() ? std::vector<int>() : stateRight->firstPos());
	}

	std::vector<int> lastPos() {
		auto result = appendUnique(stateRight->lastPos(), stateLeft->lastPos());
		result.push_back(letterOnPosition.size() - 1); // Добавление '#' к lastPos()
		return result;
	}
	void calculateFollowers(std::vector<std::vector<int>>& followersMatrix) {
		stateLeft->calculateFollowers(followersMatrix);
		stateRight->calculateFollowers(followersMatrix);
		for (int i : stateLeft->lastPos()) {
			for (int j : stateRight->firstPos()) {
				followersMatrix[i].push_back(j);
			}
		}

	}
	~DotState() {
		delete stateLeft;
		delete stateRight;
	}

};

class NodeState :public State {
	int position;
	char character;
public:
	NodeState(int position, char character) {
		this->position = position;
		this->character = character;
	}

	bool isNullable() {
		return false;
	}
	std::vector<int> firstPos() {
		return std::vector<int>(1, position);
	}
	std::vector<int> lastPos() {
		return std::vector<int>(1, position);
	}


};

class OrState :public State {

	State* stateLeft;
	State* stateRight;
public:
	OrState(State* stateLeft, State* stateRight) {
		this->stateLeft = stateLeft;
		this->stateRight = stateRight;
	}
	bool isNullable() {
		return stateLeft->isNullable() || stateRight->isNullable();
	}
	std::vector<int> firstPos() {
		return appendUnique(stateLeft->firstPos(), stateRight->firstPos());
	}

	std::vector<int> lastPos() {
		return appendUnique(stateRight->lastPos(), stateLeft->lastPos());
	}
	void calculateFollowers(std::vector<std::vector<int>>& followersMatrix) {
		stateLeft->calculateFollowers(followersMatrix);
		stateRight->calculateFollowers(followersMatrix);

	}
	~OrState() {
		delete stateLeft;
		delete stateRight;
	}

};
class StarState :public State {
	State* state;
public:
	StarState(State* state) {
		this->state = state;
	}
	bool isNullable() {
		return true;
	}
	std::vector<int> firstPos() {
		return state->firstPos();
	}
	std::vector<int> lastPos() {
		return state->lastPos();
	}
	void calculateFollowers(std::vector<std::vector<int>>& followersMatrix) {
		state->calculateFollowers(followersMatrix);
		for (int i : lastPos()) {
			for (int j : firstPos()) {
				followersMatrix[i].push_back(j);
			}
		}
	}

	~StarState() {
		delete state;
	}
};



TypeOfCharacter returnCharState(char x) {
	switch (x) {
	case '*':
		return star;
	case '.':
		return dot;
	case '|':
		return bar;
	case '(':
		return paranthesisLeft;
	case ')':
		return paranthesisRight;
	default:
		return chara;

	}

}


State* convertRegexToTree(string regex) {
	std::vector<State*> states;
	for (int i = 0; i < regex.size(); i++) {
		char character = regex[i];
		TypeOfCharacter type = returnCharState(character);
		switch (type) {
		case chara: {
			NodeState* state = new NodeState(letterOnPosition.size(), character);
			letters.insert(character);
			letterOnPosition.push_back(character);
			states.push_back(state);
			break;
		}
		case star: {
			State* lastState = states[states.size() - 1];
			states.pop_back();
			StarState* newState = new StarState(lastState);
			states.push_back(newState);
			break;
		}
		case dot: {
			State* leftState = states[states.size() - 2];
			State* rightState = states[states.size() - 1];
			states.pop_back();
			states.pop_back();
			DotState* newState = new DotState(leftState, rightState);
			states.push_back(newState);
			break;
		}
		case bar: {
			State* leftState = states[states.size() - 2];
			State* rightState = states[states.size() - 1];
			states.pop_back();
			states.pop_back();
			OrState* newState = new OrState(leftState, rightState);
			states.push_back(newState);
			break;
		}
		}

	}
	return states[0];

}

int findRightParanthesis(string characters, int offset) {
	int leftParanthesisFound = 0;
	for (int i = offset; i < characters.size(); i++) {
		if (characters[i] == '(') {
			leftParanthesisFound++;
		}
		else if (characters[i] == ')') {
			if (leftParanthesisFound == 0) {
				return i - 1;
			}
			else leftParanthesisFound--;

		}
	}
	return characters.size() - 1;
}

string convertRegexToPolishForm(string regex) {
	string polish = "";
	for (int i = 0; i < regex.size(); i++) {
		char character = regex[i];
		TypeOfCharacter type = returnCharState(character);
		switch (type) {
		case star: {
			polish.append("*");
			break; }
		case chara: {
			string str = "";
			str += character;
			if (polish != "") {
				str += ".";
			}
			polish.append(str);
			break; }
		case bar: {
			string rightSide;
			rightSide.assign(regex.begin() + i + 1, regex.begin() + findRightParanthesis(regex, i) + 1);
			polish.append(convertRegexToPolishForm(rightSide));
			polish.append("|");
			i += rightSide.size();
			break; }
		case paranthesisLeft: {
			string interior;
			interior.assign(regex.begin() + i + 1, regex.begin() + findRightParanthesis(regex, i + 1) + 1);
			string str;
			str.append(convertRegexToPolishForm(interior));
			i += interior.size() + 1;
			if (i < regex.size() - 1 && regex[i] == '*') {
				str.append("*");
				i++;
			}
			if (polish != "") {
				str.append(".");
			}
			polish.append(str);
			break; }
		case paranthesisRight: {
			break; }

		}

	}
	return polish;
}

int main()
{
	string regex= "(a*b)*|(b*a)*";


	regex += "#";
	string polish = convertRegexToPolishForm(regex);

	State* state = convertRegexToTree(polish);
	letters.erase('#');

	vector<vector<int>> vec(letterOnPosition.size(), vector<int>());

	state->calculateFollowers(vec);


	map<int, map<char, int>> automata;
	map<set<int>, int> statesToState;
	queue<set<int>> currentStates;


	vector<int> firstStatesVector = state->firstPos();
	set<int> firstState = set<int>(firstStatesVector.begin(), firstStatesVector.end());

	int currentState = 0;
	statesToState.insert(pair<set<int>, int>(firstState, currentState));
	currentStates.push(firstState);
	currentState++;


	while (!currentStates.empty()) {
		set<int> positions = currentStates.front();
		currentStates.pop();
		int position = statesToState[positions];


		for (char c : letters) {
			set<int> nextState;
			for (int position : positions) {
				if (letterOnPosition[position] == c) {
					vector<int>followPos = vec[position];
					for (int follow : followPos) {
						nextState.insert(follow);
					}

				}

			}
			if (statesToState.find(nextState) == statesToState.end()) {
				statesToState.insert(pair<set<int>, int>(nextState, currentState));
				automata[position].insert(pair<char, int>(c, currentState));
				currentState++;
				currentStates.push(nextState);
			}
			else {
				automata[position][c] = statesToState[nextState];
			}
		}


	}

	cout << "Literele alfabetului sunt: ";
	for (char c : letters) {
		cout << c << " ";
	}
	cout << '\n';
	cout << "Starile automatului sunt: ";
	for (pair<set<int>, int> c : statesToState) {
		cout << "q" << c.second << "  ";
	}
	cout << "cu statea initiala q0 si starile finale: ";
	for (pair<set<int>, int> c : statesToState) {
		if (c.first.find(letterOnPosition.size() - 1) != c.first.end()) {
			cout << "q" << c.second << "  ";
		}
	}
	cout << '\n' << "Tranzitiile sunt urmatoarele: \n";

	for (pair<int, map<char, int>> translatie : automata) {
		for (pair<char, int> rezultant : translatie.second) {
			cout << "q" << translatie.first << " -" << rezultant.first << "> q" << rezultant.second << '\n';
		}

	}
	delete state;
	int x;
	cin >> x;
	return 0;

}