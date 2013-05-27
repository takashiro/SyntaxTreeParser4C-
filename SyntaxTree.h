
#ifndef __SYNTAXTREE_H__
#define __SYNTAXTREE_H__

#include "structs.h"

class SyntaxTree{
public:
	SyntaxTree(const TokenType *tokens, StringList::iterator vars);
	~SyntaxTree();
	
	bool parse(const TokenType *tokens, StringList::iterator vars);
	void print();

	class Node{
	public:		
		Node(TokenType type = EMPTY);
		Node(Node *);
		~Node();

		void setType(TokenType);
		TokenType type() const;

		Node *child;
		Node *setChild(TokenType);
		
		Node *sibling;
		Node *setSibling(TokenType);
		
		std::string value;

	protected:
		TokenType _type;
	};

private:
	void print(Node *current, int depth);
	
	TokenType currentToken();
	TokenType forwardToken();
	void nextToken();
	void prevToken();
	const TokenType *tokenPosition();
	void setTokenPosition(const TokenType *);
	bool matchToken(TokenType);
	
	std::string getVar();

	//Rules
	Node *program();
	Node *declaration_list();
	Node *declaration();
	Node *var_declaration();
	Node *type_specifier();
	Node *params();
	Node *param_list();
	Node *param();
	Node *compound_stmt();
	Node *local_declarations();
	Node *statement_list();
	Node *statement();
	Node *expression_stmt();
	Node *selection_stmt();
	Node *iteration_stmt();
	Node *return_stmt();
	Node *expression();
	Node *var();
	Node *simple_expression();
	Node *relop();
	Node *additive_expression();
	Node *addop();
	Node *term();
	Node *mulop();
	Node *factor();
	Node *args();
	Node *arg_list();
	
	Node *_root;
	const TokenType *_current_token;
	const TokenType *_token_head;
	StringList::iterator _vars;
};

#endif

