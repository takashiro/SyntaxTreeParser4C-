#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Scanner.h"
#include "SyntaxTree.h"

void printToken(TokenType type, std::list<std::string>::iterator &var){
	printf("%s", tokenString(type));	
	if(type == NUM || type == ID){
		printf(" %s\n", (*var).c_str());
		var++;
	}else{
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	FILE *in = NULL, *out = NULL;

	if(argc < 2){
		in = stdin;
		puts("Type your code here:");
		out = stdout;
	}else{
		in = fopen(argv[1], "r");
		out = fopen(strcat(argv[1], ".out"), "w");
	}
	
	Scanner scanner;
	scanner.parse(in);
	
	TokenType *tokens = new TokenType[scanner.tokens.size()];
	int len = 0;
	for(std::list<TokenType>::iterator i = scanner.tokens.begin(); i != scanner.tokens.end(); i++){
		if(*i != COMMENT){
			tokens[len++] = *i;
		}
	}

	puts("Token List:");
	StringList::iterator var = scanner.vars.begin();
	for(int k = 0; tokens[k] != ENDFILE; k++){
		printf("%3d: ", k + 1);
		printToken(tokens[k], var);
	}

	puts("\nSyntax Tree:");
	SyntaxTree syntax_tree(tokens, scanner.vars.begin());
	syntax_tree.print();

	delete[] tokens;
	
	if(argc >= 2){
		fclose(in);
		fclose(out);
	}

	return 0;
}

