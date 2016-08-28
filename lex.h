#ifndef LEX_H
#define LEX_H

#include "token.h"

#define MAXNAMELENGTH 8

token** lex();

void freeStream(token** stream);
void printStream(token** stream);


#endif