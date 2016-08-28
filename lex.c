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

		if (isalpha(c))
		{
			char name[MAXNAMELENGTH + 1];
			name[0] = c;

			int nPointer;
			for (nPointer = 1; isalpha(c = getc(stdin)) && nPointer < MAXNAMELENGTH; nPointer++)
				name[nPointer] = c;
			name[nPointer] = '\0';

			//consume remaining characters if the identifier is too long
			while (isalpha(c)) c = getc(stdin);

			//ask the symbol table for a token for this identifier
			stream[streamPointer++] = createIdentifier(name);
			tokensRemaining--;
            if (c == '\n') break;

           	ungetc(c, stdin);
			continue;

		}

		token* new = malloc(sizeof(token));

		switch (c)
		{
			case '&':
				new->type = AND;
				break;

			case '+':
				new->type = OR;
				break;

			case '~':
				new->type = NOT;
				break;

			case '(':
				new->type = LPAR;
				break;

			case ')':
				new->type = RPAR;
				break;

			default:
				printf("Syntax error: did not expect '%c' (ASCII %d)\n", c, c);
				return NULL;
		}

		stream[streamPointer++] = new;

		tokensRemaining--;

	}

	token* eos = malloc(sizeof(token));
	eos->type = EOS;
	stream[streamPointer] = eos;

	return stream;
}

void freeStream(token** stream)
{
	//the stream returned by lex will always have at least one token (EOS)
	int i = 0;
	tokenType lastToken;
	do
	{
		lastToken = stream[i]->type;
		free(stream[i]->name);
		free(stream[i++]);
	} while (lastToken != EOS);
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
