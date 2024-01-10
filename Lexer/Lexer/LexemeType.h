#pragma once
enum class LexemeType
{
	VoidType,
	StringType,
	DoubleType,
	IntType,
	BoolType,

	MainIdentifier,
	PrintIdentifier,
	ReadIdentifier,
	Identifier,

	WhileKeyWord,
	BreakKeyWord,
	ContinueKeyWord,
	IfKeyWord,
	ElseKeyWord,
	ReturnKeyWord,

	LeftSquareBracket,
	RightSquareBracket,
	LeftCurlyBracket,
	RightCurlyBracket,
	OpenParenthesis,
	CloseParenthesis,

	PlusOperator,
	MinusOperator,
	MultOperator,
	DivOperator,

	EqualOperator,
	ConjunctionOperator,
	DisjunctionOperator,
	LessOperator,
	GreaterOperator,
	NotOperator,

	StringValue,
	NumberValue,
	BooleanValue,

	AssignmentOperator,
	Semicolon,
	Comma,
	EndOfFile,
	Error,
};