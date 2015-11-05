//Scott Nearing
//sn379745

//file description:
//This file contains global variables that are used in multiple files

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

char *token;//string that holds portions of .asm file

int LCvalue;//integer that holds LC

int lineNumber;

int listFlag;//integer used to indicate list of symbols is empty

labels *root;//create a headnode to list of symbols
undefineds *root2;//create headnode to list of undefined symbols

//File pointers; files are opened in main, but written to in funct
FILE *obj;
FILE *sym;
FILE *err;

int errorFlag;//used to determine if an error was detected




#endif // GLOBALS_H_INCLUDED
