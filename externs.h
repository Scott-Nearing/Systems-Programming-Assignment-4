//sn379745
//Scott Nearing

//file description
//this file contains extern variabels that are used by multiple files

#ifndef EXTERNS_H_INCLUDED
#define EXTERNS_H_INCLUDED



extern char *token;//string that holds part of current line of .asm file
extern int LCvalue;//holds current LCvalue

extern int lineNumber;//holds current line number

extern listFlag;//used to determine if the list of labels is empty
extern labels *root;//serves as headnode of list
extern undefineds *root2;//create headnode to list of undefined symbols

//File pointers; files are opened in main, but written to in funct
extern FILE *obj;
extern FILE *sym;
extern FILE *err;

extern int errorFlag;//used to determine if an error was detected



#endif // EXTERNS_H_INCLUDED
