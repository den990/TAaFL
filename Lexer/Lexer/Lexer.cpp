//#include <iostream>
//#include "Lexer.h"
//
//Lexer::Lexer(std::istream& input, std::ostream& output)
//	:m_input(input)
//	,m_output(output)
//{
//	m_status = Status::START;
//	m_lineNumber = 0;
//	m_tempLineNumber = m_lineNumber;
//	m_positionInLIne = 0;
//	m_commentState = CommentState::NO_COMMENT;
//	m_comment = { "//", "/*", "*/" };
//	m_operation = { "if", "else", "for", "while", "do", "break", "continue", "return", "read", "write", "main", "void", "int", "float", "double", "char", "string", "bool" };
//	m_arithmeticOperator = { '+', '-', '*', '/', '=', '%' };
//	m_comparisonsOperator = { "==", "!=", ">", "<", ">=", "<=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=" };
//	m_logicalOperator = { "&&", "||", "!" };
//	m_bitwiseOperator = { "~", "&", "|", "^", "<<", ">>" };
//	m_separator = { ',', ';', ':' };
//	m_bracket = { '{', '}', '(', ')' };
//	m_binary = { '0', '1' };
//	m_octal = { '0', '1', '2', '3', '4', '5', '6', '7' };
//	m_decimalFractional = { 'E', '.' };
//	m_hexadecimal = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
//}
//
//void Lexer::Start()
//{
//	while (getline(m_input, m_line))
//	{
//		if (m_commentState == CommentState::SIMPLE)
//		{
//			m_commentState = CommentState::NO_COMMENT;
//		}
//		m_positionInLIne = 0;
//		m_lineNumber++;
//		std::istringstream iss(m_line);
//		std::string tempStr;
//
//		while (iss >> tempStr)
//		{
//			while (!tempStr.empty())
//			{
//				Token token = GetToken(tempStr);
//				//if (token.value != "//" && token.value != "/*" && token.value != "*/")
//				if (token.lexemeType != LexemeType::COMMENT)
//				{
//					m_tokens.push_back(token);
//				}
//			}
//
//		}
//	}
//	PrintTokens();
//}
//
//Token Lexer::GetToken(std::string& tempStr)
//{
//	Token token;
//	SetStatus(Status::START);
//
//	token = CheckComment(tempStr);
//
//	if (m_commentState == CommentState::NO_COMMENT && token.lexemeType != LexemeType::COMMENT)
//	{
//		size_t lenght = CheckExistOperation(tempStr);
//		bool isCheckOperation = lenght > 0;
//		size_t lenghtWithBracket = 0;
//		std::string substr;
//
//		if (!isCheckOperation)
//		{
//			size_t foundPosition = tempStr.find("(");
//
//			if (foundPosition != std::string::npos && foundPosition != 0)
//			{
//				substr = tempStr.substr(0, foundPosition);
//				lenghtWithBracket = CheckExistOperation(substr);
//			}
//			else if (foundPosition == std::string::npos)
//			{
//				substr = tempStr.substr(0, tempStr.find("["));
//				lenghtWithBracket = CheckExistOperation(substr);
//			}
//		}
//
//		if (isCheckOperation || lenghtWithBracket > 0)
//		{
//			token = SetToken(LexemeType::OPERATOR, isCheckOperation ? tempStr : substr);
//			tempStr.erase(0, isCheckOperation ? lenght : lenghtWithBracket);
//			SetStatus(Status::END);
//		}
//		else
//		{
//			char ch = tempStr[0];
//
//			if (m_status == Status::START && IsFindVectorString(m_comparisonsOperator, tempStr))
//			{
//				token = SetToken(LexemeType::COMPARISONS_OPERATOR, tempStr);
//				tempStr.erase(0, tempStr.length());
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && IsFindVectorString(m_logicalOperator, tempStr))
//			{
//				token = SetToken(LexemeType::LOGICAL_OPERATOR, tempStr);
//				tempStr.erase(0, tempStr.length());
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && IsFindVectorString(m_bitwiseOperator, tempStr))
//			{
//				token = SetToken(LexemeType::BITWISE_OPERATOR, tempStr);
//				tempStr.erase(0, tempStr.length());
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && ch == '\'')
//			{
//				token = CheckChar(tempStr);
//			}
//
//			if (m_status == Status::START && ConvertCharToString(ch) == "\"")
//			{
//				token = CheckString(tempStr);
//			}
//
//			if (m_status == Status::START && ch == '[')
//			{
//				token = CheckArray(tempStr);
//			}
//
//			if (m_status == Status::START && IsFindVectorChar(m_arithmeticOperator, ch) && !IsDigit(tempStr[1]))
//			{
//				token = SetToken(LexemeType::ARITHMETIC_OPERATOR, ConvertCharToString(ch));
//				tempStr.erase(0, 1);
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && IsFindVectorChar(m_bracket, ch))
//			{
//				token = SetToken(LexemeType::BRACKET, ConvertCharToString(ch));
//				tempStr.erase(0, 1);
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && IsSeparator(ch))
//			{
//				token = SetToken(LexemeType::SEPARATOR, ConvertCharToString(ch));
//				tempStr.erase(0, 1);
//				SetStatus(Status::END);
//			}
//
//			if (m_status == Status::START && IsDigit(ch))
//			{
//				token = CheckNumber(tempStr);
//			}
//
//			if (m_status == Status::START && IsLetter(ch))
//			{
//				token = CheckId(tempStr);
//			}
//		}
//	}
//
//	if ((token.lexemeType == LexemeType::ERROR && m_status == Status::START) || token.lexemeType == LexemeType::COMMENT)
//	{
//		token.value = tempStr;
//		tempStr.erase(tempStr.begin(), tempStr.end());
//	}
//
//	token.lineNumber = m_lineNumber;
//
//	return token;
//
//}
//
//
//size_t Lexer::CheckExistOperation(const std::string& str) const
//{
//	auto it = std::find(m_operation.begin(), m_operation.end(), str);
//	return it != m_operation.end() ? it->length() : 0;
//}
//
//void Lexer::SetStatus(const Status& status)
//{
//	m_status = status;
//}
//
//Token Lexer::CheckNumber(std::string& str)
//{
//	Token token;
//	std::string digits;
//
//	std::string numberSystem = str.substr(0, 2);
//	std::string substr = str.length() >= 2 ? str.substr(2) : str;
//
//	if (numberSystem == "0b")
//	{
//		digits = numberSystem;
//		for (const char& ch : substr)
//		{
//			if (!IsSeparator(ch))
//			{
//				if (!IsFindVectorChar(m_binary, ch))
//				{
//					SetStatus(Status::SKIP);
//				}
//				digits += ch;
//			}
//		}
//		token.lexemeType = m_status == Status::START ? LexemeType::BINARY : LexemeType::ERROR;
//	}
//	else if (numberSystem == "0o")
//	{
//		digits = numberSystem;
//		for (const char& ch : substr)
//		{
//			if (!IsSeparator(ch))
//			{
//				if (!IsFindVectorChar(m_octal, ch))
//				{
//					SetStatus(Status::SKIP);
//				}
//				digits += ch;
//			}
//		}
//		token.lexemeType = m_status == Status::START ? LexemeType::OCTAL : LexemeType::ERROR;
//	}
//	else if (numberSystem == "0x")
//	{
//		digits = numberSystem;
//		for (const char& ch : substr)
//		{
//			if (!IsSeparator(ch))
//			{
//				if (!IsFindVectorChar(m_hexadecimal, ch))
//				{
//					SetStatus(Status::SKIP);
//				}
//				digits += ch;
//			}
//		}
//		token.lexemeType = m_status == Status::START ? LexemeType::HEXADECIMAL : LexemeType::ERROR;
//	}
//	else
//	{
//		for (size_t i = 0; i < str.length(); ++i)
//		{
//			char ch = str[i];
//			char nextCh = str[i + 1];
//			bool isDecimalFractional = IsFindVectorChar(m_decimalFractional, ch);
//			bool isNextCh = IsDigit(nextCh) || nextCh == '-' || nextCh == '+';
//
//			if (!IsSeparator(ch) && (IsDigit(ch) || isDecimalFractional || ch == '+'))
//			{
//				if (ch == 'E' && isNextCh)
//				{
//					token.lexemeType = LexemeType::DOUBLE;
//				}
//
//				if (IsDigit(ch) && nextCh == '.')
//				{
//					token.lexemeType = isdigit(str[i + 2]) ? LexemeType::FLOAT : LexemeType::ERROR;
//
//					if (IsSeparator(str[i + 2]))
//					{
//						SetStatus(Status::SKIP);
//					}
//				}
//
//				if (token.lexemeType != LexemeType::FLOAT && token.lexemeType != LexemeType::DOUBLE)
//				{
//					token.lexemeType = isDecimalFractional && !isNextCh ? LexemeType::ERROR : LexemeType::INTEGER;
//				}
//				digits += ch;
//			}
//			else if (IsLetter(ch))
//			{
//				token.lexemeType = LexemeType::ERROR;
//				break;
//			}
//		}
//
//		if (token.lexemeType != LexemeType::ERROR)
//		{
//			SetStatus(Status::SKIP);
//		}
//	}
//
//	token = SetToken(token.lexemeType, digits);
//
//	if (token.lexemeType != LexemeType::ERROR || m_status == Status::SKIP)
//	{
//		SetStatus(Status::END);
//		str.erase(0, digits.length());
//	}
//
//	return token;
//}
//
//Token Lexer::CheckId(std::string& str)
//{
//	Token token;
//	std::string letters;
//	for (const char& ch : str)
//	{
//		if (IsLetter(ch) || isdigit(ch))
//		{
//			letters += ch;
//		}
//	}
//
//	SetStatus(Status::END);
//	token = SetToken(letters.length() <= 128 ? LexemeType::ID : LexemeType::ERROR, letters);
//	str.erase(0, letters.length());
//
//	return token;
//}
//
//Token Lexer::CheckComment(const std::string str)
//{
//	Token token;
//	std::string str1;
//
//	for (size_t i = 0; i < str.length(); ++i)
//	{
//		char ch = str[i];
//		char nextCh = str[i + 1];
//		if ((ch == '/' || ch == '*') && (nextCh == '/' || nextCh == '*'))
//		{
//			str1 = ConvertCharToString(ch) + ConvertCharToString(nextCh);
//			break;
//		}
//	}
//
//	if (IsFindVectorString(m_comment, str1) || m_commentState != CommentState::NO_COMMENT)
//	{
//		if (str1 == "//" && m_commentState != CommentState::COMPLEX)
//		{
//			SetCommentState(CommentState::SIMPLE); 
//			m_tempLineNumber = m_lineNumber;
//		}
//		else if (str1 == "/*")
//		{
//			SetCommentState(CommentState::COMPLEX);
//		}
//		else if (str1 == "*/")
//		{
//			SetCommentState(CommentState::NO_COMMENT);
//		}
//
//		if (m_commentState == CommentState::SIMPLE && m_tempLineNumber != m_lineNumber)
//		{
//			SetCommentState(CommentState::NO_COMMENT);
//		}
//
//		token.lexemeType = LexemeType::COMMENT;
//	}
//
//	return token;
//}
//
//Token Lexer::CheckArray(std::string& str)
//{
//	Token token;
//	std::string strArray;
//	for (const char& ch : str)
//	{
//		if (!IsSeparator(ch))
//		{
//			strArray += ch;
//		}
//	}
//
//	if (strArray[0] == '[' && strArray[strArray.length() - 1] == ']')
//	{
//		token.lexemeType = LexemeType::ARRAY;
//	}
//	token = SetToken(token.lexemeType, strArray);
//	str.erase(0, strArray.length());
//	SetStatus(Status::END);
//
//	return token;
//}
//
//Token Lexer::CheckChar(std::string& str)
//{
//	Token token;
//	std::string strChar;
//	for (const char& ch : str)
//	{
//		if (!IsSeparator(ch))
//		{
//			strChar += ch;
//		}
//	}
//
//	if (strChar[0] == '\'' && strChar[strChar.length() - 1] == '\'' && strChar.length() >= 2)
//	{
//		token.lexemeType = LexemeType::CHAR;
//	}
//	token = SetToken(token.lexemeType, strChar);
//	str.erase(0, strChar.length());
//	SetStatus(Status::END);
//
//	return token;
//}
//
//Token Lexer::CheckString(std::string& str)
//{
//	Token token;
//	std::string str1;
//	for (const char& ch : str)
//	{
//		if (!IsSeparator(ch))
//		{
//			str1 += ch;
//		}
//	}
//
//	if (ConvertCharToString(str1[0]) == "\"" && ConvertCharToString(str1[str1.length() - 1]) == "\"" && str1.length() >= 2)
//	{
//		token.lexemeType = LexemeType::STRING;
//	}
//	token = SetToken(token.lexemeType, str1);
//	str.erase(0, str1.length());
//	SetStatus(Status::END);
//
//	return token;
//}
//
//bool Lexer::IsSeparator(const char ch) const
//{
//	return IsFindVectorChar(m_separator, ch);
//}
//
//bool Lexer::IsDigit(const char ch) const
//{
//	return isdigit(ch) || ch == '-';
//}
//
//bool Lexer::IsLetter(const char ch) const
//{
//	return isalpha(ch) || ch == '_';
//}
//
//bool Lexer::IsFindVectorChar(const std::vector<char>& vec, const char ch) const
//{
//	return std::find(vec.begin(), vec.end(), ch) != vec.end();
//}
//
//bool Lexer::IsFindVectorString(const std::vector<std::string>& vec, const std::string str) const
//{
//	return std::find(vec.begin(), vec.end(), str) != vec.end();
//}
//
//void Lexer::SetCommentState(const CommentState& commentState)
//{
//	m_commentState = commentState;
//}
//
//Token Lexer::SetToken(const LexemeType& lexemeType, const std::string str)
//{
//	return Token{ lexemeType, str, m_lineNumber, m_line.find(str) };
//}
//
//std::string Lexer::ConvertCharToString(const char ch) const
//{
//	return std::string(1, ch);
//}
//
//
//std::string Lexer::GetTokenType(const LexemeType& lexemeType) const
//{
//	switch (lexemeType)
//	{
//	case LexemeType::ERROR:
//		return "ERROR";
//	case LexemeType::ID:
//		return "ID";
//	case LexemeType::INTEGER:
//		return "INTEGER";
//	case LexemeType::FLOAT:
//		return "FLOAT";
//	case LexemeType::DOUBLE:
//		return "DOUBLE";
//	case LexemeType::CHAR:
//		return "CHAR";
//	case LexemeType::STRING:
//		return "STRING";
//	case LexemeType::ARRAY:
//		return "ARRAY";
//	case LexemeType::BINARY:
//		return "BINARY";
//	case LexemeType::OCTAL:
//		return "OCTAL";
//	case LexemeType::HEXADECIMAL:
//		return "HEXADECIMAL";
//	case LexemeType::OPERATOR:
//		return "OPERATOR";
//	case LexemeType::ARITHMETIC_OPERATOR:
//		return "ARITHMETIC_OPERATOR";
//	case LexemeType::COMPARISONS_OPERATOR:
//		return "COMPARISONS_OPERATOR";
//	case LexemeType::LOGICAL_OPERATOR:
//		return "LOGICAL_OPERATOR";
//	case LexemeType::BITWISE_OPERATOR:
//		return "BITWISE_OPERATOR";
//	case LexemeType::SEPARATOR:
//		return "SEPARATOR";
//	case LexemeType::BRACKET:
//		return "BRACKET";
//	default:
//		return "Unknown state";
//	}
//}
//
//
//void Lexer::PrintTokens()
//{
//	for (auto it : m_tokens)
//	{
//		m_output << it.value << " " << GetTokenType(it.lexemeType)/*тут надо функцию переводающую в str*/ << " " << it.lineNumber << " " << it.positionInLine << std::endl;
//	}
//}
#include "Lexer.h"




Lexer::Lexer(std::istream& strm)
	:m_strm(strm)
{
	m_strm >> std::noskipws;
}

Lexeme Lexer::GetLexeme()
{
	std::string lexeme;
	do
	{
		try
		{
			lexeme = GetLexemeImpl();
		}
		catch (const EndOfFileException& ex)
		{
			return { ex.isControlled() ? LexemeType::EndOfFile : LexemeType::Error, "", m_currentLine, m_currentPos };
		}
	} while (lexeme.empty());
	return { ClassifyLexeme(lexeme), lexeme, m_currentLine, m_currentPos };
}

std::string Lexer::GetLexemeImpl()
{
	{
		SkipIgnored();
		if (m_strm.eof())
		{
			throw EndOfFileException();
		}

		char ch;
		std::string lexeme;
		auto wasIterated = false;
		while (!m_strm.eof() && GetNextChar(ch))
		{
			wasIterated = true;
			if (ch == '#')
			{
				ProcessHash();
				continue;
			}

			if (SEPARATORS.count(ch))
			{
				if (!lexeme.empty() || IGNORED_CHARS.count(ch))
				{
					break;
				}
				UpdateCurrentLine(ch);

				if (ch == '=')
				{
					return ProcessEqual();
				}
				if ((ch == '+') || (ch == '-'))
				{
					const auto nextCh = m_strm.peek();
					if (isdigit(nextCh) || (nextCh == '.'))
					{
						lexeme += ch;
						continue;
					}
				}
				if (ch == '"')
				{
					return ProcessString();
				}
				return std::string(1, ch);
			}
			lexeme += ch;
		}
		if (wasIterated)
		{
			PutCharBack();
		}
		return lexeme;
	}
}

void Lexer::SkipIgnored()
{
	char ch;
	while (!m_strm.eof() && GetNextChar(ch))
	{
		UpdateCurrentLine(ch);
		if (!IGNORED_CHARS.count(ch))
		{
			PutCharBack();
			break;
		}
	}
}

std::string Lexer::ProcessString()
{
	std::string lexeme;

	char ch;
	while (!m_strm.eof() && GetNextChar(ch) && (ch != '"'))
	{
		UpdateCurrentLine(ch);
		lexeme += ch;
	}
	if (m_strm.eof())
	{
		throw EndOfFileException(false);
	}
	return '"' + lexeme + '"';
}

std::string Lexer::ProcessEqual()
{
	if (!m_strm.eof())
	{
		char ch;
		if (GetNextChar(ch) && (ch == '='))
		{
			return "==";
		}
		else
		{
			PutCharBack();
			return "=";
		}
	}
	throw EndOfFileException();
}

void Lexer::ProcessHash()
{
	if (m_strm.eof())
	{
		throw EndOfFileException();
	}

	char ch;
	GetNextChar(ch);
	if (ch != '#')
	{
		while (ch != '\n')
		{
			GetNextChar(ch);
		}
		UpdateCurrentLine(ch);
		return;
	}
	else
	{
		char isLastHash = false;
		while (!m_strm.eof())
		{
			GetNextChar(ch);
			UpdateCurrentLine(ch);
			if (isLastHash && (ch == '#'))
			{
				return;
			}
			isLastHash = ch == '#';
		}
		throw EndOfFileException(false);
	}
}

void Lexer::UpdateCurrentLine(char ch)
{
	if (ch == '\n')
	{
		++m_currentLine;
		m_currentPos = 0;
	}
}

std::istream& Lexer::GetNextChar(char& ch)
{
	m_strm >> ch;
	++m_currentPos;
	return m_strm;
}

void Lexer::PutCharBack()
{
	m_strm.unget();
	if (m_currentPos != 0)
	{
		--m_currentPos;
	}
}
