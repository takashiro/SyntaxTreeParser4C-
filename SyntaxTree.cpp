
#include "SyntaxTree.h"
#include "stdio.h"

SyntaxTree::SyntaxTree(const TokenType *tokens, StringList::iterator vars){
	_root = NULL;

	parse(tokens, vars);
}

SyntaxTree::~SyntaxTree(){
	if(_root != NULL){
		delete _root;
	}
}

bool SyntaxTree::parse(const TokenType *tokens, StringList::iterator vars){
	if(_root){
		delete _root;
	}
	
	_current_token = _token_head = tokens;
	_vars = vars;
	_root = program();
	
	return true;
}

void SyntaxTree::print(){
	print(_root, 0);
}

void SyntaxTree::print(Node *parent, int depth){
	if(parent == NULL){
		return;
	}
	
	for(int i = 0; i < depth; i++){
		printf("  ");
	}
	
	if(parent->type() == ID || parent->type() == NUM){
		printf("%s: %s\n", tokenString(parent->type()), parent->value.c_str());
	}else{
		puts(parent->type() == EMPTY ? "-+" : tokenString(parent->type()));
	}

	if(parent->child != NULL){
		print(parent->child, depth + 1);
	}
	
	if(parent->sibling != NULL){
		print(parent->sibling, depth);
	}
}

TokenType SyntaxTree::currentToken(){
	return *_current_token;
}

TokenType SyntaxTree::forwardToken(){
	TokenType token = *_current_token;
	nextToken();
	return token;
}

void SyntaxTree::nextToken(){
	if(*_current_token != ENDFILE){
		_current_token++;
	}
}

void SyntaxTree::prevToken(){
	_current_token--;
}

const TokenType *SyntaxTree::tokenPosition(){
	return _current_token;
}

void SyntaxTree::setTokenPosition(const TokenType *token){
	_current_token = token;
}

bool SyntaxTree::matchToken(TokenType token){
	TokenType cur = currentToken();
	if(cur == token){
		nextToken();
		return true;
	}else{
		int line_num = _current_token - _token_head + 1;
		printf("Line %d: Unexpected token %s. %s expected.\n", line_num, tokenString(cur), tokenString(token));
		return false;
	}
}

SyntaxTree::Node::Node(TokenType type){
	_type = type;
	
	child = NULL;
	sibling = NULL;
}

SyntaxTree::Node::Node(Node *node){
	_type = EMPTY;
	child = node;
	sibling = NULL;
}

SyntaxTree::Node::~Node(){
	if(child){
		delete child;
	}
	
	if(sibling){
		delete sibling;
	}
}

void SyntaxTree::Node::setType(TokenType type){
	_type = type;
}

TokenType SyntaxTree::Node::type() const{
	return _type;
}

SyntaxTree::Node *SyntaxTree::Node::setChild(TokenType type){
	if(child){
		delete child;
	}
	
	child = new Node(type);
	return child;
}

SyntaxTree::Node *SyntaxTree::Node::setSibling(TokenType type){
	if(sibling){
		delete sibling;
	}
	
	sibling = new Node(type);
	return sibling;
}

std::string SyntaxTree::getVar(){
	std::string cur = *_vars;
	_vars++;
	return cur;
}

