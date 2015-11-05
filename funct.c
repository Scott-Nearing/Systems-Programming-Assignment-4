//Scott Nearing
//sn379745

//File Description:
//This program declares several functions;
//some add nodes to linked lists others pull contents of nodes out of linked lists

//other functions assemble tmips instructions and print them to the object file

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Headers
#include "constants.h"
#include "struct_def.h"
#include "externs.h"
#include "prototypes.h"

 //root = malloc(sizeof(labels));

//This function inserts symbols into a linked lists
void insert(int LC2, char *symbol){//pass integer LC and string containing symbol

  labels *tempNode;//create labels pointer to loop through list;
  if(listFlag == 1){//if list is empty
      listFlag =0;
    root = malloc(sizeof(labels));//allocate space for root node
    root->LC = LC2;//copy LC value into node
    strcpy(root->label, symbol);//copy label into node
    root->count = 1;//assign 1 to count value of root node
//    printf("%s\n",root->label);
//    printf("%s\n","insert just printed it's first label");
  }//end if list was empty

  else{//list wasn't empty
//      printf("%s\n","list wasn't empty.");
    tempNode = root;
    if(strcmp(symbol, tempNode->label)==0){//new label matches previously defined label
//      printf("%s\n","multiply defined symbol; increasing node's count value");
      tempNode->count++;
      return;}//end if current label matches previously defined label
    while(tempNode->next !=NULL){
    if(strcmp(symbol, tempNode->label)==0){//new label matches previously defined label
//      printf("%s\n","multiply defined symbol; increasing node's count value");
      tempNode->count++;
      return;}//end if current label matches previously defined label
      tempNode=tempNode->next;
    }//end while list has more elements
//    printf("%s\n","current symbol isn't in list already; allocating space for new node and populating with new symbol and it's LC");
     tempNode->next=malloc(sizeof(labels));
     tempNode=tempNode->next;
     tempNode->LC=LC2;
     strcpy(tempNode->label, symbol);
     tempNode->count=1;

//     printf("%s\n",tempNode->label);
//     printf("%s\n","insert printed another node");
  }


}//end of insert function

/*
//this function searches through the list of labels and returns the labels' LC value
//-1 is returned if label isn't defined
int search(char *labal){
    labels *tempNode2;//create temporary node ptr for loop through list
    if (root->next == NULL){
      return -1;
    }//end if list is empty

  tempNode2 = root;//make tempPtr point to head ndoe of list

  while(tempNode2!=NULL){
    if(strcmp(tempNode2->label,labal)==0){//if current node's label matches symbol
      return tempNode2->LC;

    }//end if current node's label matches symbol
    tempNode2=tempNode2->next;//move to next node

  }//end loop through list of labels
  return -1;//searched entire list and didn't match symbol to label

}//end of search function */

//this function prints the contents of the symbol table to stdout
//testing purposes only
void printList(){
  labels *tempNode;
  if(root == NULL){
  return;}
  tempNode = root;
  while(tempNode->next!=NULL){
    fprintf(sym, "%s\t%d\n",tempNode->label, tempNode->LC);
    tempNode=tempNode->next;
  }//end while list isn't empty
    fprintf(sym, "%s\t%d\n",tempNode->label, tempNode->LC);
  return;
}//end of printList() function

//this function takes no parameters and returns nothing, but prints undefined symbols to the error file
void printUndefined(){
  undefineds *tempPtr;//ptr that traverses the list
  if(root2==NULL){
    return;}
  tempPtr = root2;
  while(tempPtr->next != NULL){
    fprintf(err, "%s\n", tempPtr->symbol);
    tempPtr = tempPtr->next;//move to next node
  }//end of loop through list of undefined labels
    fprintf(err, "%s\n", tempPtr->symbol);

}//end printUndefined()*/

//this function takes no parameters and returns nothing
void checkMulti(){
  labels *tempNode;
  if(root == NULL){//there can't be any multiply defined symbols if there are no symbols
  return;}
  tempNode = root;
  while(tempNode->next!=NULL){
    if(tempNode-> count != 1){//if current node's count isn't one then it was defined more than once
      errorFlag = 1;//error detected
      fprintf(err, "%s\n", tempNode->label);
    }//end if node's count wasnt 1
    tempNode = tempNode->next;
  }//end while list still has ndoes to check
  if(tempNode-> count != 1){//if current node's count isn't one then it was defined more than once
    errorFlag = 1;
    fprintf(err, "%s\n", tempNode->label);}//end if tempNode's count !=1
}//end of checkMulti()


//This function takes string and returns an int
//The string is a label and the returned int is it's LC value
//-1 is returned if the symbol isn't found
unsigned int fetchLC(char *symbol){
  labels *tempNode;//ptr to node of list; used to traverse the list
  if(root == NULL){//check if list is empty
    return -1;//symbol can't be in an empty list
  }//end if the list is empty
  tempNode = root;//assign root to temp
  while(tempNode->next!=NULL){//keep looping through list until no nodes remain
    if(strcmp(tempNode->label,symbol)==0){//if symbol matches label stored in node
      return tempNode->LC;
    }//end if symbol matches node
    tempNode=tempNode->next;//traverse to next node
  }//end of while nodes remain
    if(strcmp(tempNode->label,symbol)==0){//if symbol matches label stored in final node
      return tempNode->LC;
    }//end if symbol matches final node

    else{return -1;}
}//end fetchLC

//the following function takes the components of an r instruction, forces them into the correct format, and prints them in hex
void rFormat(int LC, int op, int rs1, int rs2, int rt,int shift){
  int final;//this integer is printed to the object file as the assembled instruction
  final = 0;//final starts with all of it's bit's off

  //the following lines shift the opperands into their correct position
  op = op << ATWENTYSIX; //shift left 26
  rs1 = rs1 << ATWENTYONE;//shit source register left 21
  rs2 = rs2 << ASIXTEEN;//shift source register left 16
  rt = rt << AELEVEN;//shift target register left 11
  shift = shift <<ASIX;//shift shift ammount left 6

  //the following lines use bitwise or to combine the parametrs into the assembled instruction
  final = final | op;
  final = final | rs1;
  final = final | rs2;
  final = final | rt;
  final = final | shift;
  //unused is already set to 0

  //the following lines print the LC value and it's corresponding instruction in base 16
  fprintf(obj, "%X\t", LC);
  fprintf(obj, "%X\n", final);
}//end of rFormat function

//This function takes an opcode, opperands and an LC value, then prints the LC value and it's instruction in hex
//two masks are used to copy im's sign bit and move it to the 16th bit
void iFormat(int LC, int op, int rs, int rt, int im){
  int final;//this int holds the instruction
  int alpha;//mask
  int beta;//mask

  final = 0;//it must start with all bits off

  alpha = 1;// alpah is first set to one
  alpha = alpha <<ATHIRTYONE;// the single 1 bit is moved to the other end of the int

  beta = alpha & im;// beta will be 0 if sign bit was off

  if(beta>0){//if sign bit was on

    //change alpha to flip 16th bit on
    alpha = 1;
    alpha = alpha <<AFIFTEEN;

    im = im | alpha;//force 16th bit of im to be on without changing other bits
    im =  im & IMASKMINUS;//sets bits 17-32 off
  }//end if sign bit was on

  else{//sign bit was off
    im = im & IMASKPLUS;//sets 16th bit to 0 while preserving all others
  }//end else sign bit was off

  //the following lines shit the opperands into position; im is already in position
  op = op << ATWENTYSIX;
  rs = rs << ATWENTYONE;
  rt = rt << ASIXTEEN;

  //the following lines set the bits in final
  final = final | op;
  final = final | rs;
  final = final | rt;
  final = final | im;

  fprintf(obj,"%X\t%X\n", LC, final);


}//end of iformat function*/

//the following function takes the components of a j instruction and forces them into the correct format;
//they are then printed as a hex number with the LC value.
void jFormat(int LC, int op, int rs, int rt, char *symbol){
  int final;//This integer holds the instruction after it is assembled
  unsigned int labelLC;//this int holds the LC value of the symbol's declatation
  undefineds *tempPtr;//used to loop through lst of undefined symbols

  if(fetchLC(symbol)==-1){
      errorFlag =1;//error detected

    if(root2 == NULL){//if this is the 1st undefined symbol
      root2 = malloc(sizeof(undefineds));//allocate space for root node
      strcpy(root2->symbol, symbol);//copy undefined symbol to list
    }//end if this was the 1st undefined symbol
    else{tempPtr = root2;//assign root2 to tempPtr
      while(tempPtr->next !=NULL){
        tempPtr = tempPtr->next;
      }//end loop through list of undefined symbols
      tempPtr->next = malloc(sizeof(undefineds));
      tempPtr = tempPtr->next;//move to new node
      strcpy(tempPtr->symbol,symbol);//copy undefined symbol to new node
    }//end if this wasn't the 1st undefined symbol
    return;
  }//end if fetchLC indicated an undefined symbol*/

  final = 0;//final starts with all of it's bits set to 0

  labelLC = fetchLC(symbol);//calls function that returns LC value of a label

  //the following lines shift the opperands into the correct position; labelLC doesn't require shifting
  op = op << ATWENTYSIX;
  rs = rs << ATWENTYONE;
  rt = rt << ASIXTEEN;

  //the following lines flip the bits in final to match it's components
  final = final | op;
  final = final | rs;
  final = final | rt;
  final = final | labelLC;

  fprintf(obj, "%X\t%X\n", LC , final);//prints LC value and it's corresponding instruction


}//end of jFormat function*/

//this function takes an LCvalue and an integer opperand; nothing is returned, but object file is altered
void resw(int LC, int many){
  int counter;//used in loop to control how many zeros are wirtten to the object file
  counter = 0;//start the counter at zero

  while(counter<many){

  fprintf(obj, "%X\t%X\n", LC, 0);
  counter++;
  LC++;
  }//end while counter is still less than many

}//end function resw()

//this function takes the LC value as an integer, and takes both opperands as integers; nothing is returned, but
//the buffer is written to the object file
void word(int LC, int what, int many){
  int counter;//used in loop to control how many zeros are wirtten to the object file
  unsigned int alpha;//1st mask
  unsigned int beta;//2nd mask
  int final;// holds little ending version of number to be written
  final = 0;

  counter = 0;//start the counter at zero
  alpha = WORDMASK;//set first 8 bits to 1
  alpha = alpha << ATWENTYFOUR;//shift mask1 into position

  beta = what & alpha;// get 4th byte and only forth byte
  beta = beta >> ATWENTYFOUR;// move mask2 over to write bytes on other side of int

  final = final | beta;// write bits to final

  alpha = alpha >> AEIGHT;//move mask1 over 1 byte

  beta = alpha & what;//mask 2 copies 3rd byte and only 4th byte
  beta = beta >> AEIGHT;//move 2nd mask into position

  final =  final | beta;//writes bits to final

  alpha =  alpha >> AEIGHT;//shift 1st mask right one byte

  beta =  what & alpha;//mask 2 copies 2nd byte
  beta =  beta << AEIGHT;//shift 2nd mask into position

  final = final | beta;//write bits to final

  alpha =  alpha >> AEIGHT;//move mask1 into position to copy first byte

  beta = alpha & what;//copies 1st byte into mask2
  beta = beta<< ATWENTYFOUR;//shift 2nd mask into position

  final = final | beta;//write bits to final

  while(counter < many){//while there are still words to write
    fprintf(obj,"%X\t%X\n", LC, final);//print number to object file
    //fprintf(obj, "%X\t%X\n", LC, what);//This would revert the endianness; my answer doesn't match the key, but it seems like the way I have it is correct.
    counter++;
    LC++;
    }//end while words remain

}//end function word()
