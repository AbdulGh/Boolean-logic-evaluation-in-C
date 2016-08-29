#ifndef LEX_H
#define LEX_H

#include "token.h"

#define MAXNAMELENGTH 8

token** lex();
token* eos;
void printStream(token** stream);


#endif