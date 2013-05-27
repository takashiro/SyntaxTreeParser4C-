
#include "Scanner.h"
#include <list>
#include <string.h>

const int Scanner::KEYWORD_MAXLENGTH = 7;
const char *Scanner::KeywordList[6] = {
	"else",
	"if",
	"int",
	"return",
	"void",
	"while"
};
const int Scanner::VAR_MAXLENGTH = 32;

TokenType Scanner::StringToKeyword(const char *word){
	for(int i = 0; i < 6; i++){
		if(strcmp(word, KeywordList[i]) == 0){
			switch(i){
				case 0:return ELSE;
				case 1:return IF;
				case 2:return INT;
				case 3:return RETURN;
				case 4:return VOID;
				case 5:return WHILE;
			}
		}
	}
	
	return ID;
}

bool Scanner::IsSpace(char ch){
	return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool Scanner::IsLetter(char ch){
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Scanner::IsDigit(char ch){
	return '0' <= ch && ch <= '9';
}

void Scanner::parse(FILE *in){
	tokens.clear();
	vars.clear();

	TokenStatus status = Start;
	char word[VAR_MAXLENGTH];
	int wi = 0;

	char ch;
	while((ch = fgetc(in)) != EOF){
		switch(status){
				case Start:
				while(IsSpace(ch)){
					ch = fgetc(in);
				}
				
				if(IsLetter(ch)){
					status = InID;
					word[wi++] = ch;
				
				}else if(IsDigit(ch)){
					status = InNumber;
					word[wi++] = ch;
				
				}else if(ch == '+'){
					tokens.push_back(PLUS);
				
				}else if(ch == '-'){
					tokens.push_back(MINUS);
				
				}else if(ch == '*'){
					tokens.push_back(MULTI);
				
				}else if(ch == '/'){
					ch = fgetc(in);
					if(ch == '*'){
						status = InComment;
					}else{
						tokens.push_back(DIV);
						ungetc(ch, in);
					}
					
				}else if(ch == '='){
					ch = fgetc(in);
					if(ch == '='){
						tokens.push_back(EQUAL);
					}else{
						tokens.push_back(ASSIGN);
						ungetc(ch, in);
					}
				
				}else if(ch ==  '<'){
					ch = fgetc(in);
					if(ch == '='){
						tokens.push_back(LESS_OR_EQUAL);
					}else{
						tokens.push_back(LESS);
						ungetc(ch, in);
					}
				
				}else if(ch == '>'){
					ch = fgetc(in);
					if(ch == '='){
						tokens.push_back(GREATER_OR_EQUAL);
					}else{
						tokens.push_back(GREATER);
					}
					ungetc(ch, in);
				
				}else if(ch == '!'){
					ch = fgetc(in);
					if(ch == '='){
						tokens.push_back(UNEQUAL);
					}
					
					ungetc(ch, in);
				
				}else if(ch == ';'){
					tokens.push_back(SEMICOLON);
				
				}else if(ch == ','){
					tokens.push_back(COMMA);
				
				}else if(ch == '('){
					tokens.push_back(LEFT_PARENTHESE);
				
				}else if(ch == ')'){
					tokens.push_back(RIGHT_PARENTHESE);
				
				}else if(ch == '['){
					tokens.push_back(LEFT_BRACKET);
				
				}else if(ch == ']'){
					tokens.push_back(RIGHT_BRACKET);
				
				}else if(ch == '{'){
					tokens.push_back(LEFT_BRACE);
				
				}else if(ch == '}'){
					tokens.push_back(RIGHT_BRACE);
				}
				
			break;
			
			case InID:	
				if(IsLetter(ch)){
					if(wi < VAR_MAXLENGTH - 1){
						word[wi++] = ch;
					}
				}else{
					word[wi] = 0;
					wi = 0;
					
					TokenType keyword = StringToKeyword(word);
					tokens.push_back(keyword);
					if(keyword == ID){
						vars.push_back(std::string(word));
					}
					
					status = Start;
					ungetc(ch, in);
				}

			break;
			
			case InNumber:
				if(IsDigit(ch)){
					if(wi < VAR_MAXLENGTH - 1){
						word[wi++] = ch;
					}
				}else{
					word[wi] = 0;
					wi = 0;
					vars.push_back(std::string(word));
					
					tokens.push_back(NUM);
					
					status = Start;
					ungetc(ch, in);
				}
			break;
			
			case InComment:
				if(ch == '*'){
					ch = fgetc(in);
					if(ch == '/'){
						tokens.push_back(COMMENT);
						status = Start;
					}
				}
				
			break;
			
			default:;
		}
	}
	
	status = End;
	tokens.push_back(ENDFILE); 
}

