#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lex.h"
#include "symbol.h"
#include "parse.h"

int main()
{
	printf("Please type out a boolean expression using variable names with fewer than %d letters. Press enter when done.\n",
		MAXNAMELENGTH);

	if (!initSTable()) return -1;

	token** stream = lex();

	//printStream(stream);

	assignVariables();

	printf("This statement is %s\n", evaluate(stream) ? "true" : "false");

	freeAll();
	free(stream);
	
	return 0;
}
