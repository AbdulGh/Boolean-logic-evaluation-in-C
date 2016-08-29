#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "lex.h"

#define SYMBOLSCHUNKSIZE 8
#define OPERATORS 5

static int symbolsRemaining = 0;
static int symbolPointerChunks = 0;
static int symbolPointerIndex = 0;
static token** symbols = NULL;

static token* and;
static token* or;
static token* not;
static token* lpar;
static token* rpar;

int initSTable()
{
	int chunksNeeded = OPERATORS / SYMBOLSCHUNKSIZE + 1;
	symbolsRemaining = SYMBOLSCHUNKSIZE * chunksNeeded - OPERATORS;
	symbolPointerChunks = chunksNeeded;
	symbolPointerIndex = OPERATORS;
	if ((symbols = malloc(sizeof(token*) * symbolPointerChunks * SYMBOLSCHUNKSIZE)) == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for the symbol table\n");
		return 0;
	}

	and = malloc(sizeof(token*));
	and->name = malloc(sizeof(char) * 4);
	strcpy(and->name, "and");
	and->type = AND;
	symbols[0] = and;

	or = malloc(sizeof(token*));
	or->name = malloc(sizeof(char) * 3);
	strcpy(or->name, "or");
	or->type = OR;
	symbols[1] = or;

	not = malloc(sizeof(token*));
	not->name = malloc(sizeof(char) * 4);
	strcpy(not->name, "not");
	not->type = NOT;
	symbols[2] = not;

	lpar = malloc(sizeof(token*));
	lpar->name = malloc(sizeof(char) * 2);
	strcpy(lpar->name, "(");
	lpar->type = LPAR;
	symbols[3] = lpar;

	rpar = malloc(sizeof(token*));
	rpar->name = malloc(sizeof(char) * 2);
	strcpy(rpar->name, "");
	rpar->type = RPAR;
	symbols[4] = rpar;
	return 1;
}

token* createIdentifier(char name[])
{
	//check if it's an operator
	if (strlen(name) == 1)
	{
		switch (name[0])
		{
			case '+': case '|':
				return or;
				break;
			case '&': case '^':
				return and;
				break;
			case '!': case '~':
				return not;
				break;
		}
	}

	//check if it's already in the table
	int searchPointer;
	for (searchPointer = 0; searchPointer < symbolPointerIndex; searchPointer++)
		if (!strcmp(name, symbols[searchPointer]->name)) 
			return symbols[searchPointer];

	if (!symbolsRemaining)
	{
		if ((symbols = realloc(symbols, (++symbolPointerChunks * SYMBOLSCHUNKSIZE) * sizeof(token*))) == NULL)
		{
			fprintf(stderr, "Failed to extend allocated memory for the symbol pointer array!\n");
			free(symbols);
			exit(-1);
		}

		symbolsRemaining = SYMBOLSCHUNKSIZE - 1;
	}

	token* newID = malloc(sizeof(token));
	newID->type = VAR;
	newID->value = -1;
	newID->name = malloc(sizeof(char) * (strlen(name) + 1));

	strcpy(newID->name, name);
	newID->name[strlen(name)] = '\0';

	symbols[symbolPointerIndex++] = newID;

	return newID;
}

void assignVariables()
{
    for (int i = OPERATORS; i < symbolPointerIndex; i++)
    {
        printf("Is %s true or false? ", symbols[i]->name);

        char answer = getc(stdin);
        int c;
        while ((c = getc(stdin)) != '\n' && c != EOF); //flush the rest of the input

        switch (answer)
        {
            case 't': case 'T': case '1':
                symbols[i]->value = 1;
                break;
            default:
                symbols[i]->value = 0;
        }
    }
}

void freeAll()
{
	symbolsRemaining = 0;
	symbolPointerChunks = 0;

	if (symbols != NULL)
	{

		for (int i = 0; i < symbolPointerIndex; i++)
		{
			free(symbols[i]->name);
			free(symbols[i]);
		}

		free(eos);
		free(symbols);
	}

	symbolPointerIndex = 0;
}
