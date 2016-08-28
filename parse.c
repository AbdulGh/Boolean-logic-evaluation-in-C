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
void putValue(int pos, int value);
void putValue(int pos, int value);
int atom();
int var();

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
		clearSTable();
		freeStream(stream);
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
	int val = atom();

	while (LOOKAHEADTYPE == OR)
	{
		match(OR);
		if (atom()) val = 1;
	}

	return val;
}

int atom()
{
	int negations = 0;
	int value;

	while (LOOKAHEADTYPE == NOT) 
	{
		match(NOT);
		negations = 1 - negations;
	}

	if (LOOKAHEADTYPE == LPAR)
	{
		match(LPAR);
		value = expr();
		match(RPAR);
	}
	else value = var();

	if (negations) return 1 - value;

	return value;
}

int var()
{
	int value = LOOKAHEAD->value;
	match(VAR);
	return value;
}