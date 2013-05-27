
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <list>
#include <string>

enum TokenType{
	ENDFILE,
	
	ERROR,
	UNKNOWN,
	EMPTY,

	CALL,
	VAR,

	IF,
	ELSE,
	INT,
	RETURN,
	VOID,
	WHILE,
	
	EQUAL,
	UNEQUAL,
	ASSIGN,
	GREATER_OR_EQUAL,
	GREATER,
	LESS_OR_EQUAL,
	LESS,
	
	SEMICOLON,
	COMMA,
	LEFT_PARENTHESE,
	RIGHT_PARENTHESE,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMENT,

	PLUS,
	MINUS,
	MULTI,
	DIV,

	NUM,
	ID	
};

#ifndef NULL
#define NULL 0
#endif

const char *tokenString(TokenType token);

typedef std::list<std::string> StringList;

#endif

