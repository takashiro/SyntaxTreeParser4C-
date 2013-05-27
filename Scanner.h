
#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "structs.h"
#include <stdio.h>

class Scanner{
protected:
	static const int KEYWORD_MAXLENGTH;
	static const char *KeywordList[6];
	static const int VAR_MAXLENGTH;

public:
	enum TokenStatus{
		Start, InComment, InID, InNumber, End
	};
	
	static TokenType StringToKeyword(const char *);	
	static bool IsSpace(char);
	static bool IsLetter(char);
	static bool IsDigit(char);
	
	std::list<TokenType> tokens;
	StringList vars;
	void parse(FILE *in);
};

#endif

