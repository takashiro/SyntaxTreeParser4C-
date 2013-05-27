
#include "utils.h"
#include "structs.h"

template <class Type>
void ListToArray(std::list<Type> list, Type *&arr, int &len){
	arr = new Type[list.size()];
	len = 0;
	for(std::list<TokenType>::iterator i = list.begin(); i != list.end(); i++){
		if(*i != COMMENT){
			arr[len++] = *i;
		}
	}
}

