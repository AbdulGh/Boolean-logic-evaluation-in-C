#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

#define SYMBOLSCHUNKSIZE 4

static int symbolsRemaining = 0;
static int symbolPointerChunks = 0;
static int symbolPointerIndex = 0;
static token** symbols = NULL;

token* createIdentifier(char name[])
{
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
    for (int i = 0; i < symbolPointerIndex; i++)
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

void clearSTable()
{
	symbolsRemaining = 0;
	symbolPointerChunks = 0;
	symbolPointerIndex = 0;

	free(symbols);
}
