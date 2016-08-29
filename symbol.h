#ifndef SYMBOL_H
#define SYMBOL_H

int initSTable();
token* createIdentifier(char* name);
void assignVariables();
void freeAll();

#endif