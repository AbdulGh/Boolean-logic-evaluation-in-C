#ifndef TOKEN_H
#define TOKEN_H

typedef enum {VAR, LPAR, RPAR, AND, OR, NOT, EOS} tokenType;
static char* typeNames[] = {"VAR", "LPAR", "RPAR", "AND", "OR", "NOT", "EOS"}; //debugging purposes

typedef struct 
{
	tokenType type;
	char* name;
	int value;
} token;

#endif