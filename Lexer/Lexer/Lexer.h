//#pragma once
//#include <iostream>
//#include <string>
//#include <sstream>
//#include <vector>
//#include "LexemeType.h"
//
//enum class Status
//{
//	START,
//	END,
//	SKIP
//};
//
//enum class CommentState
//{
//	NO_COMMENT,
//	SIMPLE,
//	COMPLEX
//};
//
//struct Token
//{
//	LexemeType lexemeType;
//	std::string value;
//	size_t lineNumber;
//	size_t positionInLine;
//
//};
//
//
//class Lexer
//{
//public:
//	Lexer(std::istream& input, std::ostream& output);
//
//	void Start();
//private:
//	Token GetToken(std::string& tempStr);
//
//	size_t CheckExistOperation(const std::string& str) const;
//	Token CheckNumber(std::string& str);
//	Token CheckId(std::string& str);
//	Token CheckComment(const std::string str);
//	Token CheckArray(std::string& str);
//	Token CheckChar(std::string& str);
//	Token CheckString(std::string& str);
//
//	bool IsSeparator(const char ch) const;
//	bool IsDigit(const char ch) const;
//	bool IsLetter(const char ch) const;
//
//
//	void SetStatus(const Status& status);
//	std::string GetTokenType(const LexemeType& lexemeType) const;
//	Token SetToken(const LexemeType& lexemeType, const std::string str);
//	void SetCommentState(const CommentState& commentState);
//
//	std::string ConvertCharToString(const char ch) const;
//	bool IsFindVectorChar(const std::vector<char>& vec, const char ch) const;
//	bool IsFindVectorString(const std::vector<std::string>& vec, const std::string str) const;
//
//	void PrintTokens();
//	Status m_status;
//	size_t m_lineNumber, m_positionInLIne, m_tempLineNumber;
//	std::istream& m_input;
//	std::ostream& m_output;
//	std::string m_line;
//	std::vector<Token> m_tokens;
//	std::vector<std::string> m_comment, m_operation, m_comparisonsOperator, m_logicalOperator, m_bitwiseOperator;
//	std::vector<char> m_arithmeticOperator, m_bracket, m_separator, m_binary, m_octal, m_decimalFractional, m_hexadecimal;
//	CommentState m_commentState;
//};



#pragma once

#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <optional>
#include <istream>
#include <string>
#include <regex>
#include "LexemeType.h"

namespace
{

	const std::unordered_set<char> IGNORED_CHARS = { ' ', '\t', '\n' };
	const std::unordered_set<char> SEPARATORS = {
		' ', '\t', '\n', ';', ',', '{', '}', '(', ')', '[', ']', '=', '<', '>', '!', '/', '*', '+', '-', '"', '#'
	};

	bool IsNumber(const std::string& lexeme)
	{
		return std::regex_match(lexeme, std::regex("[-+]?\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?0b[01]+"))
			|| std::regex_match(lexeme, std::regex("[-+]?0x[0-9A-F]+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\d+\\.\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\.\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\d+\\."));
	}

	bool IsIdentifier(const std::string& lexeme)
	{
		return std::regex_match(lexeme, std::regex("[a-zA-Z_][a-zA-Z0-9_]*"));
	}

	LexemeType ClassifyLexeme(const std::string& lexeme)
	{
		if (lexeme == "void") return LexemeType::VoidType;
		if (lexeme == "string") return LexemeType::StringType;
		if (lexeme == "double") return LexemeType::DoubleType;
		if (lexeme == "int") return LexemeType::IntType;
		if (lexeme == "bool") return LexemeType::BoolType;

		if (lexeme == "main") return LexemeType::MainIdentifier;
		if (lexeme == "print") return LexemeType::PrintIdentifier;
		if (lexeme == "read") return LexemeType::ReadIdentifier;

		if (lexeme == "+") return LexemeType::PlusOperator;
		if (lexeme == "-") return LexemeType::MinusOperator;
		if (lexeme == "*") return LexemeType::MultOperator;
		if (lexeme == "/") return LexemeType::DivOperator;

		if (lexeme == "==") return LexemeType::EqualOperator;
		if (lexeme == "&&") return LexemeType::ConjunctionOperator;
		if (lexeme == "||") return LexemeType::DisjunctionOperator;
		if (lexeme == "<") return LexemeType::LessOperator;
		if (lexeme == ">") return LexemeType::GreaterOperator;
		if (lexeme == "!") return LexemeType::NotOperator;

		if (lexeme == "while") return LexemeType::WhileKeyWord;
		if (lexeme == "break") return LexemeType::BreakKeyWord;
		if (lexeme == "continue") return LexemeType::ContinueKeyWord;
		if (lexeme == "if") return LexemeType::IfKeyWord;
		if (lexeme == "else") return LexemeType::ElseKeyWord;
		if (lexeme == "return") return LexemeType::ReturnKeyWord;

		if (lexeme == "=") return LexemeType::AssignmentOperator;
		if (lexeme == "(") return LexemeType::OpenParenthesis;
		if (lexeme == ")") return LexemeType::CloseParenthesis;
		if (lexeme == "{") return LexemeType::LeftCurlyBracket;
		if (lexeme == "}") return LexemeType::RightCurlyBracket;
		if (lexeme == "[") return LexemeType::LeftSquareBracket;
		if (lexeme == "]") return LexemeType::RightSquareBracket;
		if (lexeme == ";") return LexemeType::Semicolon;
		if (lexeme == ",") return LexemeType::Comma;
		if ((lexeme.size() > 1) && (lexeme.front() == '"') && (lexeme.back() == '"')) return LexemeType::StringValue;
		if ((lexeme == "true") || (lexeme == "false")) return LexemeType::BooleanValue;
		if (IsNumber(lexeme)) return LexemeType::NumberValue;
		if (IsIdentifier(lexeme)) return LexemeType::Identifier;
		return LexemeType::Error;
	}

	std::string LexemeTypeToString(LexemeType type)
	{
		switch (type)
		{
		case LexemeType::VoidType:
			return "VoidType";
		case LexemeType::StringType:
			return "StringType";
		case LexemeType::DoubleType:
			return "DoubleType";
		case LexemeType::IntType:
			return "IntType";
		case LexemeType::BoolType:
			return "BoolType";

		case LexemeType::MainIdentifier:
			return "MainIdentifier";
		case LexemeType::PrintIdentifier:
			return "PrintIdentifier";
		case LexemeType::ReadIdentifier:
			return "ReadIdentifier";
		case LexemeType::Identifier:
			return "Identifier";

		case LexemeType::WhileKeyWord:
			return "WhileKeyWord";
		case LexemeType::BreakKeyWord:
			return "BreakKeyWord";
		case LexemeType::ContinueKeyWord:
			return "ContinueKeyWord";
		case LexemeType::IfKeyWord:
			return "IfKeyWord";
		case LexemeType::ElseKeyWord:
			return "ElseKeyWord";
		case LexemeType::ReturnKeyWord:
			return "ReturnKeyWord";

		case LexemeType::LeftSquareBracket:
			return "LeftSquareBracket";
		case LexemeType::RightSquareBracket:
			return "RightSquareBracket";
		case LexemeType::LeftCurlyBracket:
			return "LeftCurlyBracket";
		case LexemeType::RightCurlyBracket:
			return "RightCurlyBracket";
		case LexemeType::OpenParenthesis:
			return "OpenParenthesis";
		case LexemeType::CloseParenthesis:
			return "CloseParenthesis";

		case LexemeType::PlusOperator:
			return "PlusOperator";
		case LexemeType::MinusOperator:
			return "MinusOperator";
		case LexemeType::MultOperator:
			return "MultOperator";
		case LexemeType::DivOperator:
			return "DivOperator";

		case LexemeType::EqualOperator:
			return "EqualOperator";
		case LexemeType::ConjunctionOperator:
			return "ConjunctionOperator";
		case LexemeType::DisjunctionOperator:
			return "DisjunctionOperator";
		case LexemeType::LessOperator:
			return "LessOperator";
		case LexemeType::GreaterOperator:
			return "GreaterOperator";
		case LexemeType::NotOperator:
			return "NotOperator";

		case LexemeType::StringValue:
			return "StringValue";
		case LexemeType::NumberValue:
			return "NumberValue";
		case LexemeType::BooleanValue:
			return "BooleanValue";

		case LexemeType::AssignmentOperator:
			return "AssignmentOperator";
		case LexemeType::Semicolon:
			return "Semicolon";
		case LexemeType::Comma:
			return "Comma";
		case LexemeType::EndOfFile:
			return "EndOfFile";
		case LexemeType::Error:
		default:
			return "Error";
		}
	}

	class EndOfFileException
		:public std::exception
	{
	public:
		explicit EndOfFileException(bool isControlled = true)
			:std::exception()
			, m_isControlled(isControlled)
		{}

		bool isControlled() const
		{
			return m_isControlled;
		}

	private:
		bool m_isControlled = false;
	};

};

struct Lexeme
{
	LexemeType type;
	std::string lexeme;
	size_t lineNum;
	size_t linePos;
};


class Lexer
{
public:
	Lexer(std::istream& strm);

	Lexeme GetLexeme();

private:
	std::string GetLexemeImpl();

	void SkipIgnored();

	std::string ProcessString();

	std::string ProcessEqual();

	void ProcessHash();

	void UpdateCurrentLine(char ch);

	std::istream& GetNextChar(char& ch);

	void PutCharBack();

	std::istream& m_strm;
	size_t m_currentLine = 1;
	size_t m_currentPos = 0;
};