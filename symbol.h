#ifndef SYMBOL_H
#define SYMBOL_H

#include "token.h"

token* and;
token* or;
token* not;
token* lpar;
token* rpar;

int initSTable();
token* createIdentifier(char name[]);
void assignVariables();
void freeAll();

#endif