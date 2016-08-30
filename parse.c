#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "symbol.h"
#include "lex.h"

int streamPointer;
token** stream;
#define LOOKAHEAD stream[streamPointer]
#define LOOKAHEADTYPE LOOKAHEAD->type

void match(tokenType hope);
int expr();
int term();
int literal();
int atom();
void putValue(int pos, int value);
void putValue(int pos, int value);


int evaluate(token** instream)
{
	stream = instream;
	int value = expr();
	match(EOS);
	return value;
}

void match(tokenType hope)
{
	if (hope == LOOKAHEADTYPE) streamPointer++;

	else
	{
		printf("Couldn't parse input - expected %s at token %d, found %s\n", typeNames[hope], streamPointer, typeNames[LOOKAHEADTYPE]);
		freeAll();
		free(stream);
		exit(0);
	}
}

int expr()
{
	int val = term();

	while (LOOKAHEADTYPE == AND)
	{
		match(AND);
		if (!term()) val = 0;
	}

	return val;
}

int term()
{
	int val = literal();

	while (LOOKAHEADTYPE == OR)
	{
		match(OR);
		if (literal()) val = 1;
	}

	return val;
}

int literal()
{
	int negations = 0;
	while (LOOKAHEADTYPE == NOT) 
	{
		match(NOT);
		negations = 1 - negations;
	}

	int value = atom();

	if (negations) return 1 - value;
	else return value;
}

int atom()
{
	int value;

	if (LOOKAHEADTYPE == LPAR)
	{
		match(LPAR);
		value = expr();
		match(RPAR);
	}
	else 
	{
		value = LOOKAHEAD->value;
		match(VAR);
	}

	return value;
}