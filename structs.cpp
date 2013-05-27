
#include "structs.h"

const char *tokenString(TokenType token){
	switch(token){
		case ENDFILE:return "ENDFILE";break;
		case ERROR:return "ERROR";break;
		case UNKNOWN:return "UNKNOWN";break;
		
		case VAR:return "VAR";break;
		case CALL:return "CALL";break;
		
		case IF:return "IF";break;
		case ELSE:return "ELSE";break;
		case INT:return "INT";break;
		case RETURN:return "RETURN";break;
		case VOID:return "VOID";break;
		case WHILE:return "WHILE";break;
		
		case EQUAL:return "EQUAL";break;
		case UNEQUAL:return "UNEQUAL";break;
		case ASSIGN:return "ASSIGN";break;
		case GREATER_OR_EQUAL:return "GREATER_OR_EQUAL";break;
		case GREATER:return "GREATER";break;
		case LESS_OR_EQUAL:return "LESS_OR_EQUAL";break;
		case LESS:return "LESS";break;
		
		case SEMICOLON:return "SEMICOLON";break;
		case COMMA:return "COMMA";break;
		case LEFT_PARENTHESE:return "LEFT_PARENTHESE";break;
		case RIGHT_PARENTHESE:return "RIGHT_PARENTHESE";break;
		case LEFT_BRACKET:return "LEFT_BRACKET";break;
		case RIGHT_BRACKET:return "RIGHT_BRACKET";break;
		case LEFT_BRACE:return "LEFT_BRACE";break;
		case RIGHT_BRACE:return "RIGHT_BRACE";break;
		case COMMENT:return "COMMENT";break;
	
		case PLUS:return "PLUS";break;
		case MINUS:return "MINUS";break;
		case MULTI:return "MULTI";break;
		case DIV:return "DIV";break;
		case NUM:return "NUM";break;
		case ID:return "ID";break;
	}
}

