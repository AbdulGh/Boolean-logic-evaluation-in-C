#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lex.h"
#include "symbol.h"

#define CHUNKLENGTH 8

token** lex()
{
	int chunks = 1;
	int tokensRemaining = CHUNKLENGTH;
	int streamPointer = 0;
	token** stream = malloc(sizeof(token*) * CHUNKLENGTH);

	char c;

	while ((c = getc(stdin)) != '\n')
	{

		if (!tokensRemaining)
		{
			if ((stream = realloc(stream, (++chunks * CHUNKLENGTH) * sizeof(token*))) == NULL)
			{
				fprintf(stderr, "Failed to extend allocated memory for the token stream!\n");
				free(stream);
				exit(-1);
			}
			tokensRemaining = CHUNKLENGTH;
		}

		if (isspace(c)) continue;

		char name[MAXNAMELENGTH + 1];
		name[0] = c;

		int nPointer;
		for (nPointer = 1; isalpha(c = getc(stdin)) && nPointer < MAXNAMELENGTH; nPointer++)
			name[nPointer] = c;
		name[nPointer] = '\0';

		//consume remaining characters if the identifier is too long
		while (!isspace(c)) c = getc(stdin);
		ungetc(c, stdin);

		//ask the symbol table for a token for this lexeme
		stream[streamPointer++] = createIdentifier(name);
		tokensRemaining--;
	}

	if (!tokensRemaining)
	{
		//we need one more token* for the EOS
		if ((stream = realloc(stream, (chunks * CHUNKLENGTH) * sizeof(token*)+ 1)) == NULL)
		{
			fprintf(stderr, "Failed to extend allocated memory for the EOS token!\n");
			free(stream);
			freeAll();
			exit(-1);
		}
	}

	if (eos == NULL)
	{
		eos = malloc(sizeof(token));
		eos->type = EOS;
	}

	stream[streamPointer] = eos;

	return stream;
}

void printStream(token** stream)
{
	int i = 0;
	token* last;
	do
	{
		last = stream[i++];
		printf("Type: %s    Name: %s   Value: %d   Location: %p\n", 
			typeNames[last->type], last->type == VAR ? last->name : "Not a var", 
			last->value, last);
	} while (last->type != EOS);
}
