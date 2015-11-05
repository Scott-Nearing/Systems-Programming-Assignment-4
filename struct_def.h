//Scott Nearing
//sn379745

//File description
//This file defines two types; both are linked lists.
//The first holds a symbol table the records how many times a symbol was defined

//The 2nd holds undefined symbols

#ifndef STRUCT_DEF_H_INCLUDED
#define STRUCT_DEF_H_INCLUDED

typedef struct labelList{
int LC;//stores the LC value the label was defined at
int count;//shows how many times a label was added to the list
char label[81];//holds the string that represents the label
struct labelList *next;//pointer to next node of list.

}labels; //end labelList struct definition by defining type as labels

//the followig is a linked list of undefined symbols
typedef struct undefined{
char symbol[81];
struct undefined *next;

}undefineds;


#endif // STRUCT_DEF_H_INCLUDED
