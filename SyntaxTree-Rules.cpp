
#include "SyntaxTree.h"
#include "stdio.h"

SyntaxTree::Node *SyntaxTree::program(){
	return declaration_list();
}

SyntaxTree::Node *SyntaxTree::declaration_list(){
	Node *list = new Node;

	list->child = declaration();
	if(list->child){
		Node *dec = list->child;
		while((dec->sibling = declaration()) != NULL){
			dec = dec->sibling;
		}	
	}

	return list;
}

SyntaxTree::Node *SyntaxTree::declaration(){
	Node *parent = type_specifier();
	if(parent){
		matchToken(ID);		
		Node *dec = parent->setChild(ID);
		dec->value = getVar();
		
		TokenType token = forwardToken();
		//Declare an array
		if(token == LEFT_BRACKET){
			matchToken(NUM);
			dec = dec->setSibling(NUM);
			dec->value = getVar();
			matchToken(RIGHT_BRACKET);
			matchToken(SEMICOLON);

		//Declare a function
		}else if(token == LEFT_PARENTHESE){
			dec->sibling = params();
			dec = dec->sibling;
			matchToken(RIGHT_PARENTHESE);
			dec->sibling = compound_stmt();
		
		//Declare a variable
		}else if(token == SEMICOLON){
			
		//Error, should never happen
		}else{
			delete parent;
			return NULL;
		}
	}

	return parent;
}

SyntaxTree::Node *SyntaxTree::type_specifier(){
	TokenType token = forwardToken();
	
	if(token == INT || token == VOID){
		return new Node(token);
	}
	
	prevToken();
	return NULL;
}

SyntaxTree::Node *SyntaxTree::params(){
	Node *p = NULL;

	switch(currentToken()){
		case VOID:
			nextToken();
		case RIGHT_PARENTHESE:
			p = new Node(VOID);
			break;
		default:
			p = param_list();
	}
	
	return p;
}

SyntaxTree::Node *SyntaxTree::param_list(){
	Node *p = param();
	Node *head = p;
	
	if(p){
		while(currentToken() == COMMA){
			nextToken();
			p->sibling = param();
			p = p->sibling;
		}
		
		p = new Node;
		p->child = head;
		return p;		
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::param(){
	Node *parent = type_specifier();
	if(parent){
		if(forwardToken() == ID){
			Node *child = parent->setChild(ID);
			child->value = getVar();
			return parent;
		}

		prevToken();
		delete parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::compound_stmt(){
	if(matchToken(LEFT_BRACE)){
		Node *local_dec = local_declarations();
		Node *stmt_list = statement_list();
		
		Node *parent = new Node;	
		if(local_dec){
			parent->child = local_dec;
			if(stmt_list){
				parent->child->sibling = stmt_list;
			}
		}else if(stmt_list){
			parent->child = stmt_list;
		}
		
		if(matchToken(RIGHT_BRACE)){
			return parent;	
		}

		delete parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::var_declaration(){
	Node *parent = type_specifier();
	if(parent){
		if(forwardToken() == ID){
			Node *child = parent->setChild(ID);
			child->value = getVar();
			if(matchToken(SEMICOLON)){
				return parent;
			}
		}
		delete parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::local_declarations(){
	Node *head = var_declaration();
	
	if(head){
		Node *vard = head;
		while(vard){
			vard->sibling = var_declaration();
			vard = vard->sibling;
		}
		
		Node *parent = new Node;
		parent->child = head;
		return parent;
	}
	
	return NULL;
}

SyntaxTree::Node *SyntaxTree::statement_list(){
	Node *head = statement();
	if(head){
		Node *stmt = head;
		while(stmt){
			stmt->sibling = statement();
			stmt = stmt->sibling;
		}
		
		Node *list = new Node;
		list->child = head;
		return list;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::statement(){
	Node *stmt = NULL;
	switch(currentToken()){
		case LEFT_PARENTHESE:case ID:case NUM:case SEMICOLON:
			stmt = expression_stmt();
			break;
		case LEFT_BRACE:
			stmt = compound_stmt();
			break;
		case IF:
			stmt = selection_stmt();
			break;
		case WHILE:
			stmt = iteration_stmt();
			break;
		case RETURN:
			stmt = return_stmt();
			break;
	}

	return stmt;
}

SyntaxTree::Node *SyntaxTree::expression_stmt(){
	const TokenType *opos = tokenPosition();
	
	if(currentToken() == SEMICOLON){
		nextToken();
		Node *exp = new Node(SEMICOLON);
		return exp;
	}else{
		Node *exp = expression();
		if(exp){
			if(matchToken(SEMICOLON)){
				return exp;
			}
			delete exp;
		}
	}
	
	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::selection_stmt(){
	if(!matchToken(IF)){
		return NULL;
	}

	const TokenType *opos = tokenPosition();

	Node *stmt = new Node(IF);
	if(matchToken(LEFT_PARENTHESE)){
		stmt->child = expression();
		Node *part = stmt->child;
		if(part){
			if(matchToken(RIGHT_PARENTHESE)){
				part->sibling = statement();
				part = part->sibling;
				
				if(currentToken() == ELSE){
					nextToken();
					part->sibling = statement();
					part = part->sibling;
					
					if(part == NULL){
						delete stmt;
						return NULL;
					}
				}

				return stmt;
			}
		}
	}
	
	setTokenPosition(opos);
	delete stmt;
	return NULL;
}

SyntaxTree::Node *SyntaxTree::iteration_stmt(){
	if(!matchToken(WHILE)){
		return NULL;
	}
	
	Node *stmt = new Node(WHILE);
	if(matchToken(LEFT_PARENTHESE)){
		stmt->sibling = expression();
		Node *part = stmt->sibling;
		if(part && matchToken(RIGHT_PARENTHESE)){
			part->sibling = statement();
			
			if(part->sibling){
				return stmt;
			}
		}
	}

	delete stmt;
	return NULL;
}

SyntaxTree::Node *SyntaxTree::return_stmt(){
	if(matchToken(RETURN)){
		Node *stmt = new Node(RETURN);
		Node *exp = expression();
		if(exp){
			stmt->child = exp;			
			if(matchToken(SEMICOLON)){
				return stmt;
			}
		}
		
		delete stmt;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::expression(){
	Node *exp = simple_expression();
	if(exp == NULL){
		delete exp;
	}else{
		if(currentToken() == ASSIGN){
			Node *var = exp;
			exp = new Node(ASSIGN);
			exp->child = var;
	
			if(exp->child){
				Node *part = exp->child;
				if(matchToken(ASSIGN)){
					part->sibling = expression();
					
					if(part->sibling){
						return exp;	
					}
				}
			}
		}

		return exp;
	}
	
	return NULL;
}

SyntaxTree::Node *SyntaxTree::var(){
	const TokenType *opos = tokenPosition();
	
	if(matchToken(ID)){
		Node *variable = new Node(VAR);
		Node *id_node = variable->setChild(ID);
		id_node->value = getVar();
	
		if(currentToken() == LEFT_BRACKET){
			nextToken();
			id_node->sibling = expression();
			if(id_node->sibling && matchToken(RIGHT_BRACKET)){
				return variable;
			}
		}
		
		return id_node;
	}

	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::simple_expression(){
	Node *exp = additive_expression();

	if(exp){
		Node *cmpop = relop();
		while(cmpop){
			exp->sibling = additive_expression();			
			cmpop->child = exp;
			exp = cmpop;
			
			cmpop = relop();
		}

		return exp;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::relop(){
	TokenType token = currentToken();
	if(token == LESS_OR_EQUAL || token == LESS || token == GREATER || token == GREATER_OR_EQUAL || token == EQUAL || token == UNEQUAL){
		nextToken();
		return new Node(token);
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::additive_expression(){
	const TokenType *opos = tokenPosition();

	Node *left = term();
	if(left){
		Node *op = addop();
		while(op){
			left->sibling = term();
			op->child = left;
			
			left = op;
			op = addop();
		}

		return left;
	}

	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::addop(){
	TokenType token = currentToken();
	
	if(token == PLUS || token == MINUS){
		nextToken();
		return new Node(token);
	}
	
	return NULL;
}

SyntaxTree::Node *SyntaxTree::term(){
	const TokenType *opos = tokenPosition();
	
	Node *left = factor();
	if(left){
		Node *op = mulop();
		while(op){
			left->sibling = factor();
			op->child = left;
			
			left = op;
			op = mulop();
		}

		return left;
	}

	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::mulop(){
	TokenType token = currentToken();
	if(token == MULTI || token == DIV){
		nextToken();
		return new Node(token);
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::factor(){
	const TokenType *opos = tokenPosition();

	TokenType token = forwardToken();	
	Node *head = NULL, *part = NULL;
	TokenType next = EMPTY;
	switch(token){
		case LEFT_PARENTHESE:
			head = expression();
			if(head && matchToken(RIGHT_PARENTHESE)){
				return head;
			}
			break;
		case ID:
			next = currentToken();
			if(next == LEFT_PARENTHESE){
				nextToken();
				head = new Node(CALL);
				part = head->setChild(ID);
				part->value = getVar();
				part->sibling = args();
				if(matchToken(RIGHT_PARENTHESE)){
					return head;
				}
			}else{
				prevToken();
				return var();
			}
			break;
		case NUM:
			Node *node = new Node(NUM);
			node->value = getVar();
			return node;
			break;
	}
	
	if(head){
		delete head;
	}
	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::args(){
	Node *list = arg_list();
	if(list){
		list = new Node(list);
	}
	return list;
}

SyntaxTree::Node *SyntaxTree::arg_list(){
	const TokenType *opos = tokenPosition();
	
	Node *head = expression();
	if(head){
		Node *part = head;
		while(currentToken() == COMMA){
			nextToken();
			part->sibling = expression();
		
			if(part->sibling){
				part = part->sibling;
			}else{
				break;
			}
		}

		return head;
	}
	
	setTokenPosition(opos);
	return NULL;
}

