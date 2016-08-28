#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lex.h"
#include "symbol.h"
#include "parse.h"

int main()
{
	printf("Please type out a boolean expression using &, +, ~, and variable names with fewer than %d letters. Press enter when done.\n",
		MAXNAMELENGTH);

	token** stream = lex();

	assignVariables();

	printf("This statement is %s\n", evaluate(stream) ? "true" : "false");

	freeStream(stream);
	clearSTable();

	return 0;
}
