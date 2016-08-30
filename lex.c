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
	while (1)
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

		c = getc(stdin);
		
		//check if it's an operator or newline
		switch (c)
		{
			case '+': case '|':
				stream[streamPointer++] = or;
				tokensRemaining--;
				continue;
			case '&': case '^':
				stream[streamPointer++] = and;
				tokensRemaining--;
				continue;
			case '!': case '~':
				stream[streamPointer++] = not;
				tokensRemaining--;
				continue;
			case '(':
				stream[streamPointer++] = lpar;
				tokensRemaining--;
				continue;
			case ')':
				stream[streamPointer++] = rpar;
				tokensRemaining--;
				continue;
			case '\n':
				eos = malloc(sizeof(token));
				eos->type = EOS;
				stream[streamPointer] = eos;
				return stream;
		}

		if (isspace(c)) continue;

		char name[MAXNAMELENGTH + 1];
		name[0] = c;

		int npointer;
		for (npointer = 1; isalnum(c = getc(stdin)) && npointer < MAXNAMELENGTH; npointer++)
			name[npointer] = c;
		name[npointer] = '\0';

		//consume the rest of the name, if there's any left
		while (isalnum(c)) c = getc(stdin);
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
