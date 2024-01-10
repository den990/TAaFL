#pragma once
#include "Struct_type.h"

class RegexToDFA {
public:
	unordered_map<int, char> IntToString;
	unordered_map<char, int> StringToInt;
	NfaState NfaStates[MAX];	
	int nfaStateNum = 0;		
	DfaState DfaStates[MAX];		
	int dfaStateNum = 0;			
	IntSet s[MAX];					
	DfaState minDfaStates[MAX];		
	int minDfaStateNum = 0;			
	FinalDFA dfa;

	void add(NfaState* n1, NfaState* n2, char ch);
	void add(NfaState* n1, NfaState* n2);
	NFA creatNFA(int sum);
	void preprocess(string& s);
	int priority(char ch);
	string infixToSuffix(string s);
	NFA strToNfa(string s);
	IntSet epcloure(IntSet s);
	IntSet moveEpCloure(IntSet s, char ch);
	bool IsEnd(NFA n, IntSet s);
	DFA nfaToDfa(NFA n, string str);
	int findSetNum(int count, int n);
	DFA minDFA(DFA d);
	FinalDFA labelTrans(DFA d);
	FinalDFA GetDFA(string str);
	void update(string str);
	RegexToDFA();
};

void RegexToDFA::add(NfaState* n1, NfaState* n2, char ch)
{
	n1->input = ch;
	n1->chTrans = n2->index;
}


void RegexToDFA::add(NfaState* n1, NfaState* n2)
{

	n1->epTrans.insert(n2->index);
}


NFA RegexToDFA::creatNFA(int sum)
{

	NFA n;

	n.head = &NfaStates[sum];
	n.tail = &NfaStates[sum + 1];

	return n;
}



void RegexToDFA::preprocess(string& s)
{
	string str = "";
	int i = 0, length = s.size();
	char c = 'a';
	while (i < length)
	{
		if (!isdigit(s[i])) {
			str += s[i];
			i++;
		}
		else {
			int num = 0;
			while (isdigit(s[i])) {
				num = num * 10 + (s[i] - '0');
				i++;
			}
			if (!IntToString.count(num)) {
				IntToString[num] = c;
				StringToInt[c] = num;
				str += c;
				c++;
			}
			else {
				str += IntToString[num];
			}

		}
	}
	s = str;
}


int RegexToDFA::priority(char ch)
{
	if (ch == '*')
	{
		return 3;
	}

	if (ch == '+')
	{
		return 3;
	}

	if (ch == '?')
	{
		return 3;
	}

	if (ch == '&')
	{
		return 2;
	}

	if (ch == '|')
	{
		return 1;
	}

	if (ch == '(')
	{
		return 0;
	}
}

string RegexToDFA::infixToSuffix(string s)
{

	preprocess(s);			

	string str;				
	stack<char> oper;		

	for (int i = 0; i < s.size(); i++)
	{

		if (s[i] >= 'a' && s[i] <= 'z')	
		{
			str += s[i];
		}
		else							
		{

			if (s[i] == '(')			
			{
				oper.push(s[i]);
			}

			else if (s[i] == ')')	
			{

				char ch = oper.top();
				while (ch != '(')		
				{

					str += ch;

					oper.pop();
					ch = oper.top();
				}

				oper.pop();				
			}
			else					
			{

				if (!oper.empty())			
				{

					char ch = oper.top();
					while (priority(ch) >= priority(s[i]))	
					{

						str += ch;
						oper.pop();

						if (oper.empty())	
						{
							break;
						}
						else ch = oper.top();
					}

					oper.push(s[i]);		
				}

				else				
				{
					oper.push(s[i]);
				}
			}
		}
	}

	
	while (!oper.empty())
	{

		char ch = oper.top();
		oper.pop();

		str += ch;
	}



	return str;
}


NFA RegexToDFA::strToNfa(string s)
{

	stack<NFA> NfaStack;		

	for (int i = 0; i < s.size(); i++)		
	{

		if (s[i] >= 'a' && s[i] <= 'z')		
		{

			NFA n = creatNFA(nfaStateNum);		
			nfaStateNum += 2;					

			add(n.head, n.tail, s[i]);			

			NfaStack.push(n);					
		}

		else if (s[i] == '*')		
		{
			NFA n1 = creatNFA(nfaStateNum);		
			nfaStateNum += 2;					

			NFA n2 = NfaStack.top();			
			NfaStack.pop();

			add(n2.tail, n1.head);				
			add(n2.tail, n1.tail);				
			add(n1.head, n2.head);				
			add(n1.head, n1.tail);				

			NfaStack.push(n1);					
		}

		else if (s[i] == '+') {
			NFA n1 = creatNFA(nfaStateNum);		
			nfaStateNum += 2;					

			NFA n2 = NfaStack.top();			
			NfaStack.pop();

			add(n2.tail, n1.head);				
			add(n2.tail, n1.tail);				
			add(n1.head, n2.head);				

			NfaStack.push(n1);					
		}

		else if (s[i] == '?') {
			NFA n1 = creatNFA(nfaStateNum);		
			nfaStateNum += 2;					
			NFA n2 = NfaStack.top();			
			NfaStack.pop();

			add(n2.tail, n1.tail);				
			add(n1.head, n2.head);				
			add(n1.head, n1.tail);				

			NfaStack.push(n1);					
		}

		else if (s[i] == '|')		
		{

			NFA n1, n2;							
			n2 = NfaStack.top();
			NfaStack.pop();

			n1 = NfaStack.top();
			NfaStack.pop();

			NFA n = creatNFA(nfaStateNum);		
			nfaStateNum += 2;					

			add(n.head, n1.head);				
			add(n.head, n2.head);				
			add(n1.tail, n.tail);				
			add(n2.tail, n.tail);				

			NfaStack.push(n);					
		}

		else if (s[i] == '&')		
		{

			NFA n1, n2, n;				

			n2 = NfaStack.top();				
			NfaStack.pop();

			n1 = NfaStack.top();
			NfaStack.pop();

			add(n1.tail, n2.head);				

			n.head = n1.head;					
			n.tail = n2.tail;					

			NfaStack.push(n);					
		}


	}

	return NfaStack.top();		
}


IntSet RegexToDFA::epcloure(IntSet s)
{
	stack<int> epStack;

	IntSet::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		epStack.push(*it);
	}

	while (!epStack.empty())
	{
		int temp = epStack.top();
		epStack.pop();

		IntSet::iterator iter;
		for (iter = NfaStates[temp].epTrans.begin(); iter != NfaStates[temp].epTrans.end(); iter++)
		{
			if (!s.count(*iter))
			{
				s.insert(*iter);
				epStack.push(*iter);
			}
		}
	}

	return s;
}

IntSet RegexToDFA::moveEpCloure(IntSet s, char ch)
{
	IntSet temp;

	IntSet::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		if (NfaStates[*it].input == ch)
		{
			temp.insert(NfaStates[*it].chTrans);
		}
	}

	temp = epcloure(temp);
	return temp;
}

bool RegexToDFA::IsEnd(NFA n, IntSet s)
{
	IntSet::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		if (*it == n.tail->index)
		{
			return true;
		}
	}

	return false;
}

DFA RegexToDFA::nfaToDfa(NFA n, string str)
{
	int i;
	DFA d;
	set<IntSet> states;

	memset(d.trans, -1, sizeof(d.trans));

	for (i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			d.terminator.insert(str[i]);
		}
	}

	d.startState = 0;

	IntSet tempSet;
	tempSet.insert(n.head->index);

	DfaStates[0].closure = epcloure(tempSet);
	DfaStates[0].isEnd = IsEnd(n, DfaStates[0].closure);

	dfaStateNum++;

	queue<int> q;
	q.push(d.startState);

	while (!q.empty())
	{
		int num = q.front();
		q.pop();

		CharSet::iterator it;
		for (it = d.terminator.begin(); it != d.terminator.end(); it++)
		{
			IntSet temp = moveEpCloure(DfaStates[num].closure, *it);

			if (!states.count(temp) && !temp.empty())
			{
				states.insert(temp);

				DfaStates[dfaStateNum].closure = temp;
				DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;
				DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = dfaStateNum;
				DfaStates[num].edgeNum++;

				d.trans[num][*it - 'a'] = dfaStateNum;

				DfaStates[dfaStateNum].isEnd = IsEnd(n, DfaStates[dfaStateNum].closure);

				q.push(dfaStateNum);
				dfaStateNum++;
			}
			else
			{
				for (i = 0; i < dfaStateNum; i++)
				{
					if (temp == DfaStates[i].closure)
					{
						DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;
						DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = i;
						DfaStates[num].edgeNum++;

						d.trans[num][*it - 'a'] = i;

						break;
					}
				}
			}
		}
	}

	for (i = 0; i < dfaStateNum; i++)
	{
		if (DfaStates[i].isEnd == true)
		{
			d.endStates.insert(i);
		}
	}

	return d;
}

int RegexToDFA::findSetNum(int count, int n)
{
	for (int i = 0; i < count; i++)
	{
		if (s[i].count(n))
		{
			return i;
		}
	}
}

DFA RegexToDFA::minDFA(DFA d)
{
	int i, j;
	DFA minDfa;
	minDfa.terminator = d.terminator;

	memset(minDfa.trans, -1, sizeof(minDfa.trans));

	bool endFlag = true;
	for (i = 0; i < dfaStateNum; i++)
	{
		if (DfaStates[i].isEnd == false)
		{
			endFlag = false;
			minDfaStateNum = 2;
			s[1].insert(DfaStates[i].index);
		}
		else
		{
			s[0].insert(DfaStates[i].index);
		}
	}

	if (endFlag)
	{
		minDfaStateNum = 1;
	}

	bool cutFlag = true;
	while (cutFlag)
	{
		int cutCount = 0;
		for (i = 0; i < minDfaStateNum; i++)
		{
			CharSet::iterator it;
			for (it = d.terminator.begin(); it != d.terminator.end(); it++)
			{
				int setNum = 0;
				stateSet temp[20];

				IntSet::iterator iter;
				for (iter = s[i].begin(); iter != s[i].end(); iter++)
				{
					bool epFlag = true;
					for (j = 0; j < DfaStates[*iter].edgeNum; j++)
					{
						if (DfaStates[*iter].Edges[j].input == *it)
						{
							epFlag = false;

							int transNum = findSetNum(minDfaStateNum, DfaStates[*iter].Edges[j].Trans);

							int curSetNum = 0;
							while ((temp[curSetNum].index != transNum) && (curSetNum < setNum))
							{
								curSetNum++;
							}

							if (curSetNum == setNum)
							{
								temp[setNum].index = transNum;
								temp[setNum].s.insert(*iter);
								setNum++;
							}
							else
							{
								temp[curSetNum].s.insert(*iter);
							}
						}
					}

					if (epFlag)
					{
						int curSetNum = 0;
						while ((temp[curSetNum].index != -1) && (curSetNum < setNum))
						{
							curSetNum++;
						}

						if (curSetNum == setNum)
						{
							temp[setNum].index = -1;
							temp[setNum].s.insert(*iter);
							setNum++;
						}
						else
						{
							temp[curSetNum].s.insert(*iter);
						}
					}
				}

				if (setNum > 1)
				{
					cutCount++;
					for (j = 1; j < setNum; j++)
					{
						IntSet::iterator t;
						for (t = temp[j].s.begin(); t != temp[j].s.end(); t++)
						{
							s[i].erase(*t);
							s[minDfaStateNum].insert(*t);
						}
						minDfaStateNum++;
					}
				}
			}
		}

		if (cutCount == 0)
		{
			cutFlag = false;
		}
	}

	for (i = 0; i < minDfaStateNum; i++)
	{
		IntSet::iterator y;
		for (y = s[i].begin(); y != s[i].end(); y++)
		{
			if (*y == d.startState)
			{
				minDfa.startState = i;
			}

			if (d.endStates.count(*y))
			{
				minDfaStates[i].isEnd = true;
				minDfa.endStates.insert(i);
			}

			for (j = 0; j < DfaStates[*y].edgeNum; j++)
			{
				for (int t = 0; t < minDfaStateNum; t++)
				{
					if (s[t].count(DfaStates[*y].Edges[j].Trans))
					{
						bool haveEdge = false;
						for (int l = 0; l < minDfaStates[i].edgeNum; l++)
						{
							if ((minDfaStates[i].Edges[l].input == DfaStates[*y].Edges[j].input) && (minDfaStates[i].Edges[l].Trans == t))
							{
								haveEdge = true;
							}
						}

						if (!haveEdge)
						{
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].input = DfaStates[*y].Edges[j].input;
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].Trans = t;
							minDfaStates[i].edgeNum++;

							minDfa.trans[i][DfaStates[*y].Edges[j].input - 'a'] = t;

							break;
						}
					}
				}
			}
		}
	}

	return minDfa;
}

FinalDFA RegexToDFA::labelTrans(DFA d)
{
	FinalDFA f;
	f.stateNum = minDfaStateNum;
	f.startState = d.startState;
	f.endStates = d.endStates;
	set<char>::iterator it;
	for (it = d.terminator.begin(); it != d.terminator.end(); it++)
	{
		int temp = 0;
		temp = StringToInt[*it];
		f.labelSet.insert(temp);
	}
	for (int i = 0; i < minDfaStateNum; i++)
	{
		unordered_map<int, int> temp;
		f.labelToState.push_back(temp);
		for (int j = 0; j < f.labelSet.size(); j++)
		{
			if (d.terminator.count(j + 'a'))
			{
				if (d.trans[i][j] != -1)
				{
					int temp = StringToInt[j + 'a'];
					f.labelToState[i][temp] = d.trans[i][j];
					f.LabelArriveState[temp].insert(d.trans[i][j]);
				}
			}
		}
	}
	int label_num = d.terminator.size();
	f.Back_labelToState.resize(minDfaStateNum);
	for (int i = 0; i < minDfaStateNum; i++)
	{
		for (auto it = f.labelToState[i].begin(); it != f.labelToState[i].end(); it++)
		{
			int label = it->first;
			int state = it->second;
			f.Back_labelToState[state][label].insert(i);
		}
	}
	return f;
}

FinalDFA RegexToDFA::GetDFA(string str)
{
	str = infixToSuffix(str);
	NFA n = strToNfa(str);
	DFA d = nfaToDfa(n, str);
	DFA minDfa = minDFA(d);
	FinalDFA dfa = labelTrans(minDfa);
	return dfa;
}

void RegexToDFA::update(string str)
{
	dfa = GetDFA(str);
}

RegexToDFA::RegexToDFA()
{
	IntToString.clear();
	StringToInt.clear();
	nfaStateNum = 0;
	dfaStateNum = 0;
	minDfaStateNum = 0;
	int i, j;
	for (i = 0; i < MAX; i++)
	{
		NfaStates[i].index = i;
		NfaStates[i].input = '#';
		NfaStates[i].chTrans = -1;
		NfaStates[i].epTrans.clear();
		s[i].clear();
	}
	for (i = 0; i < MAX; i++)
	{
		DfaStates[i].index = i;
		DfaStates[i].isEnd = false;
		for (j = 0; j < 10; j++)
		{
			DfaStates[i].Edges[j].input = '#';
			DfaStates[i].Edges[j].Trans = -1;
		}
		DfaStates[i].edgeNum = 0;
		DfaStates[i].closure.clear();
	}
	for (i = 0; i < MAX; i++)
	{
		minDfaStates[i].index = i;
		minDfaStates[i].isEnd = false;
		for (int j = 0; j < 10; j++)
		{
			minDfaStates[i].Edges[j].input = '#';
			minDfaStates[i].Edges[j].Trans = -1;
		}
		minDfaStates[i].edgeNum = 0;
		minDfaStates[i].closure.clear();
	}
	dfa.endStates.clear();
	dfa.Back_labelToState.clear();
	dfa.LabelArriveState.clear();
	dfa.labelSet.clear();
	dfa.labelToState.clear();
	dfa.startState = -1;
}
