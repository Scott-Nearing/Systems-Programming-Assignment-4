//Scott Nearing
//sn379745

//File description:
//This is the main file of a two pass tmips assembler.
//The main function calls on functions from another file, funct.c and relies on several header files
//The program takes a single command line argument; the filename of the assembly file



//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//headers
#include "constants.h"
#include "struct_def.h"
#include "globals.h"
#include "prototypes.h"


int main(int argc, char *argv[])
{
//    argc = 2;
//variables

  int i;//Holds length of inpit file name; counts down during while loop
  int j;//Holds length of input file name

  char inputName[sizeof(argv[1])];//holds input file name
  char *opCodes[ARRAYSIZE];//array of pointers to character arrays
  //char *directives[];//array of pointers to character arrays

  char *symbolName = malloc((int)strlen(inputName)+FILEEXTENTIONLENGTH);//holds filename of symbol table
  char *objectName = malloc((int)strlen(inputName)+FILEEXTENTIONLENGTH);//holds name of object file
  char *errorName = malloc((int)strlen(inputName)+FILEEXTENTIONLENGTH); //holds name of error file

  char inputLine[LINELENGTH];//holds line from input file
  //char *token;//holds token from inputLine //defined as global

  int labelLength;//holds length of label
  //int LCvalue;//holds LC value//defined as a global variable
  int num1;//holds number read in while parsing preprocessor directives
  int num2;//holds number read in while parsing preprocessor directives
  int num3;//holds opperand from .resw during 2nd pass
  int num4;//holds 1st opperand from .word during 2nd pass
  int num5;//holds 2nd opperand from .word during 2nd pass

  int o1;//holds first opperand
  int o2;//holds 2nd opperand
  int o3;//holds 3rd opperand


  int firstLabelFlag;// used to not increment LC value of first label
  int passTwoFlag;// LC value is incremented if this flag is 0

  int opCodeLoop;// used when comparing opcode to opcode array


  FILE *inputFile;//pointer to input file


//check for correct number of arguments
  if(argc != EXPECTEDARGS){return 1;}

//open input file; close program if file can't be opened
  if((inputFile=fopen(argv[1],"r"))==NULL){
    return 1;
  }//end if

//  printf("%s\n",argv[1]);
//assign op codes to opCodes[]

  opCodes[AZERO] = "hlt";
  opCodes[AONE] = "add";
  opCodes[ATWO] = "sub";
  opCodes[ATHREE] = "mul";
  opCodes[AFOUR] = "div";
  opCodes[AFIVE] = "mod";
  opCodes[ASIX] = "move";
  opCodes[ASEVEN] = "and";
  opCodes[AEIGHT] = "or";
  opCodes[ANINE] = "xor";
  opCodes[ATEN] ="com";
  opCodes[AELEVEN] ="sll";
  opCodes[ATWELVE] ="srl";
  opCodes[ATHIRTEEN] ="sra";
  opCodes[AFOURTEEN] ="jr";
  opCodes[AFIFTEEN] ="rdr";
  opCodes[ASIXTEEN] ="prr";
  opCodes[ASEVENTEEN] ="prh";
  opCodes[AEIGHTEEN] ="li";
  opCodes[ANINETEEN] ="addi";
  opCodes[ATWENTY] ="subi";
  opCodes[ATWENTYONE] ="muli";
  opCodes[ATWENTYTWO] ="divi";
  opCodes[ATWENTYTHREE] ="modi";
  opCodes[ATWENTYFOUR] ="lwb";
  opCodes[ATWENTYFIVE] ="swb";
  opCodes[ATWENTYSIX] ="lwa";
  opCodes[ATWENTYSEVEN] ="swa";
  opCodes[ATWENTYEIGHT] ="j";
  opCodes[ATWENTYNINE] ="jal";
  opCodes[ATHIRTY] ="jeq";
  opCodes[ATHIRTYONE] ="jne";
  opCodes[ATHIRTYTWO] ="jlt";
  opCodes[ATHIRTYTHREE] ="jle";
  opCodes[ATHIRTYFOUR] ="jgt";
  opCodes[ATHIRTYFIVE] ="jge";

  //printf("%s",opCodes[1]);
  //
  strcpy((char*)inputName,(char*)argv[1]/*,sizeof(argv[1])*/);//copies  file from command line input
  i = (int)strlen((char*)inputName);//assigns length to int i
  j=i;

  while (inputName[i]!= '.' && i>=0){//loop through inputFileName backwards until negative position
    i--;}//decrement i

  if(i==-1){//'.' wasn't found in input file name

/*
//new strings to hold file names
    char symbolName[sizeof(inputName)+4];//holds filename of symbol table
    char objectName[sizeof(inputName)+4];//holds name of object file
    char errorName[sizeof(inputName)+4]; //holds name of error file
    */

//copy modified inputName into new strings
    strncpy(symbolName,(char*)inputName,(int)strlen(inputName));
    strncpy(objectName,(char*)inputName,(int)strlen(inputName));
    strncpy(errorName,(char*)inputName,(int)strlen(inputName));

    strcat(symbolName, ".sym");//add file extensions
    strcat(objectName, ".obj");//''    ''
    strcat(errorName, ".err");//""   ""
//    printf("%s",symbolName);
  }//end of if

  else{//'.' was as found
    while(i<j){//traverse to the end of inputFileName
      inputName[i] = '\0';//erase file extention
      i++;}//end of while


/*
//new strings to hold file names
    char symbolName[sizeof(inputName)+4];//holds filename of symbol table
    char objectName[sizeof(inputName)+4];//holds name of object file
    char errorName[sizeof(inputName)+4]; //holds name of error file */


//copy modified inputName into new strings
    strcpy(symbolName,/*(char*)*/inputName/*,(int)strlen(inputName)*/);
    strcpy(objectName,/*(char*)*/inputName/*,(int)strlen(inputName)*/);
    strcpy(errorName,/*(char*)*/inputName/*,(int)strlen(inputName)*/);

//    printf("%s\n",symbolName);
//    printf("%s\n", objectName);
//    printf("%s\n",errorName);

//add file extensions to new file names
    strcat(symbolName, ".sym");

//     printf("%s\n",symbolName);

    strcat(objectName, ".obj");
    strcat(errorName, ".err");


//    printf("%s\n",symbolName);
//    printf("%s\n",objectName);
//    printf("%s\n",errorName);
  }//end of else

//    printf("%s\n","1");
  if((sym = fopen(symbolName,"w"))==NULL){return 1;}
  if((obj = fopen(objectName,"w"))==NULL){return 1;}
  if((err = fopen(errorName,"w"))==NULL){return 1;}

  //correct numbers of arguments, files opened and renamed, start reading in .asm file line by line
  firstLabelFlag = 1;
  listFlag = 1;//indicates that list is empty
  lineNumber = 0;//set line number to 0 initially
  errorFlag = 0;//starts off and is set to 1 if an error is detected


  fprintf(err,"%s\n", "Error(s) detected:");

  while(fgets(inputLine, LINELENGTH, inputFile)!= NULL){
//    printf("%s\n","4");
    lineNumber++;//increment line number for every line read in
    i = 0;//set i to zero initially
//printf("%d\n",lineNumber);

    while(i<81){//loop through array character by character
      if(inputLine[i]== '#'){
        //i =0;//set i back to 0
        //fgets(inputLine, 81, inputFile);//get next line of .asm
      //  printf("\n%s %d\n","pound sign found before non-whitespace character",i);
        break;
      }//end if '#' was found first

     else if(inputLine[i]!=' '&& inputLine[i]!='\t' && inputLine[i]!='\n'){
       // printf("\n%s\n","non-whitespace character was found before pound; processing line");
        break;//must have read in a non whitespace character that isn't a #; parse line.
      }//end if a character other than white space or sharp sign.
      else{i++;//read a white space character; read next char
     //  printf("%s %d ","character in line being examined:",i);
     }
    }//end loop through list

    if(inputLine[i]=='#'){
     // printf("%s\n","comment found first; reading in new line");
      continue;//ignore this line
    }//end if line is a comment

    if(inputLine[i]=='\0'){
    //    printf("%s\n","examined entire line and found only whitespace; reading in new line");
      continue;
    }//end if input line is NULL
    token = strtok(inputLine, "\t ");//braks inputLine by tabs and spaces
    labelLength = (int)strlen(token)-1;//assign length of string to  labelLength

    if(token[labelLength]==':'){//If last character in token is a : then it is a label
       // printf("%s\n","label found");
      if(firstLabelFlag==1){
         firstLabelFlag=0;
        // printf("%s\n","1st label; calling insert function");
        // printf("%s\n",token);
          token[labelLength] = '\0';//strip colon
         insert(LCvalue, token);
         //printf("%s","label: ");
         //printf("%s\n",token);

       //insert token into hash table as label without increasing LCvalue
      }//end if firstLabelFlag equals 1
      else{//LCvalue++;
    //    printf("%s\n","2nd or later label; calling insert function");
        token[labelLength] ='\0';//strip colon
        insert(LCvalue, token);
      //    printf("%s\n%s","insert was just called","label: ");
       // printf("%s\n",token);
      }//end else; firstLabelFlag equals 0
      token = strtok(NULL, "\t ");//assign return of strtok to token; delimit by whitespace
    }//end if

    if(token[0]== '.'){//if 1st character of token is a '.' then the token is a preprocessor directive

      if(token[1]=='r'){
        token=strtok(NULL, "\t \n");
        num1 = atoi(token); //assign integer value of read in resw paramter
        LCvalue += num1;//increase LCvalue by number of words reserved
        continue;
      }//end if resw

      else if(token[1]=='w'){
          //printf("\n\n\n\n%s\n\n\n\n",".word found");
        strtok(NULL,":");//throw away first character
        token = strtok(NULL, "\t ");//assign next token to token; delimters are space and tab
        num2 = atoi(token);
  //      printf("%s %d\n","number or words reserved by .word directive",num2);
        LCvalue += num2;
        continue;
      }//end if word

      if(token[1]== 't'){
        //text segment
        //printf("\n\n\n\n%s\n\n\n\n","text segment:");
        continue;
      }//end if 2nd character of token t

      if(token[1]=='d'){
        //data segment
        //printf("\n\n\n\n%s\n\n\n\n","data segment:");
        continue;
      }//end if 2nd character of token is a d

    }//end if preprocessor directive


  //current line was erronious or had a label
  //incremet LCvalue by 1 word
  LCvalue++;

  }//end while .asm has lines

//printf("\n\n%s\n\n","Printing list's contents");
printList();

//Symbol Table has been formed; ready for pass 2
rewind(inputFile);//rewinds position indicator for 2nd pass
lineNumber = 0;//set line number back to zero
LCvalue = 0;//set LC value back to 0

while(fgets(inputLine, LINELENGTH, inputFile)!= NULL){//read input line by line until there is nothing left(pass2)

  passTwoFlag = 0;
  lineNumber++;//increment line number for every line read in
  i = 0;//set i to zero initially
  //printf("%d %s\n",lineNumber,"pass two");

  while(i<LINELENGTH){//loop through array character by character
    if(inputLine[i]== '#'){
      //printf("\n%s %d\n","pound sign found before non-whitespace character",i);
      break;
    }//end if '#' was found first

    else if(inputLine[i]!=' '&& inputLine[i]!='\t' && inputLine[i]!='\n'){
      //printf("\n%s\n","non-whitespace character was found before pound; processing line");
      break;//must have read in a non whitespace character that isn't a #; parse line.
     }//end if a character other than white space or sharp sign.

    else{i++;//read a white space character; read next char
      //printf("%s %d ","character in line being examined:",i);
      }
    }//end loop through list

    if(inputLine[i]=='#'){
      //printf("%s\n","comment found first; reading in new line");
      continue;//ignore this line
    }//end if line is a comment

    if(inputLine[i]=='\0'){
      //printf("%s\n","examined entire line and found only whitespace; reading in new line");
      continue;
    }//end if input line is NULL

    token = strtok(inputLine, " \t \n");//parse line by whitespace characters
    labelLength = strlen(token)-1;//assign length of label; token is one longer than label

    if(token[labelLength]==':'){//if final position of token is a : then token must be a label
     //store token as label
     //passTwoFlag =1;
     //LCvalue++;
     token = strtok(NULL, " \t \n");// continue to parse line
    }//end if token is a label

    if(token[0]=='.'){//if 1st position of token is a . the the token is a directive
      passTwoFlag = 1;
      if(token[1]=='r'){//if 2nd position of token is an r then the dierective is .resw
          //printf("%s\n","2nd pass resw");
          token = strtok(NULL, " \t\n");//grab opperand
          num3 = atoi(token);//assign the integer value of the opperand to num3
          resw(LCvalue, num3);
          LCvalue = LCvalue +num3;

          //call function to write object code for .resw

      }// end if 2nd position is a r
      else if(token[1]=='w'){//if the 2nd position of token is a w, then the directive is .word
          //printf("%s\n","2nd pass word");
        num4 = atoi(strtok(NULL, ":"));//assign num4 the integer value of the opperand before the :
        num5 = atoi(strtok(NULL," \t\n"));//assign num5 the integer value of the opperand after the :
        word(LCvalue, num4, num5);
        LCvalue = LCvalue + num5;
      }//end if 2nd position is a w
      else if(token[1]=='t'){//text directive

      }//end if 2nd position is a t

      else if(token[1]=='d'){//data directive


      }//end if 2nd position is a d
      continue;
    }//end if token is a directive



  //At this point we know that the token isn't a preprocessor directive or a a label
  //so we will treat it as an opcode.

  opCodeLoop = 0;//assign opCodeLoop to zero before looping through array of opcodes
  while(opCodeLoop<ARRAYSIZE){//while elements remain in the opcode array
    if(strcmp(opCodes[opCodeLoop],token)==0){//if current token matches an opcode
        //printf("%s\n","opcode found");
      //value of opCodeLoop determines opperands to expect
      if(opCodeLoop == 0){
        //hlt takes no opperands
        rFormat(LCvalue, 0,0,0,0,0);
        break;
      }//end if hlt

      else if(opCodeLoop > 0 && opCodeLoop < ASIX){
        //these take three registers
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o3 = atoi(token);


        rFormat(LCvalue, opCodeLoop, o2, o3, o1,  0);
        break;
      }//end if opcode is an arithmatic operator(1)

      else if(opCodeLoop > ASIX && opCodeLoop <ATEN){
        //these take three registers
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o3 = atoi(token);


        rFormat(LCvalue, opCodeLoop, o2, o3, o1, 0);
        break;
      }//end if opcode is arithmatic

      else if(opCodeLoop ==ASIX || opCodeLoop == ATEN ){
        //these take two registers
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        rFormat(LCvalue, opCodeLoop, o2, 0, o1, 0);
        break;
      }//end if opcode is move or com

      else if(opCodeLoop > ATEN && opCodeLoop <AFOURTEEN){
        //these take two registers then an int
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        o3 = atoi(token);


        rFormat(LCvalue, opCodeLoop, o2, 0, o1, o3);
        break;
      }//end if opcode is a shift

      else if(opCodeLoop > ATHIRTEEN && opCodeLoop< AEIGHTEEN){
        //these take one register
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);
        rFormat(LCvalue, opCodeLoop, 0, 0, o1, 0);
        break;
      }//end if opcode is idk something

      else if(opCodeLoop == AEIGHTEEN){
        //this takes one register and one int
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        o2 = atoi(token);
         iFormat(LCvalue, opCodeLoop, 0, o1, o2);
         break;
      }//end if opcode is li

      else if(opCodeLoop > AEIGHTEEN && opCodeLoop< ATWENTYFOUR){
        //these take two registers and an int
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        o3 = atoi(token);
        iFormat(LCvalue, opCodeLoop, o2, o1, o3);
        break;
      }//end if opcode is an immediate

      else if(opCodeLoop == ATWENTYFOUR || opCodeLoop == ATWENTYFIVE){
        //these take a register followed by an immediate and another regiester in ()

        token = strtok(NULL, " ,\t\n()");//parse by whitespace, (, ), and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n()");//parse by whitespace, (, ),  and ,
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n()");//parse by whitespace, (, ),  and ,
        token[0] = ' ';//replace $ with ' '
        o3 = atoi(token);
        iFormat(LCvalue, opCodeLoop, o3, o1, o2);
        break;
      }//end if opcode is lwb or swb

      else if(opCodeLoop == ATWENTYSIX || opCodeLoop == ATWENTYSEVEN){
//        printf("%s\n","opcode 26 or 27");
        //these take a regiester and a label
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        //token[0] = ' ';

        jFormat(LCvalue, opCodeLoop, 0, o1, token);
        break;

      }//end if opcode is lwa or swa

      else if(opCodeLoop == ATWENTYEIGHT){
        //these take only a label
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        //token[0]= ' ';

        jFormat(LCvalue, opCodeLoop, 0, 0, token);
        break;

      }//end if opcode is an unconditional jump

      else if(opCodeLoop > ATWENTYEIGHT && opCodeLoop<= ATHIRTYFIVE){
        //these take two registers and a label
        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o1 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        token[0] = ' ';//replace $ with ' '
        o2 = atoi(token);

        token = strtok(NULL, " ,\t\n");//parse by whitespace and ,
        //token[0] = ' ';

        jFormat(LCvalue,opCodeLoop, o2, o1, token);
        break;

      }//end if opcode is a conditional jump


    }//end if token matches opCode

    opCodeLoop++;
    if(opCodeLoop == ARRAYSIZE){//opCode wasn't found in list
      errorFlag = 1;//error detected
      fprintf(err, "%d\t%s\n", lineNumber, "Illegal opcode found in this line.");
    }//end if opcode wasn't found in opcode array*/

  }//end loop through opcode array

     if(passTwoFlag == 0){
      LCvalue++;
    }//end if passTwo Flag is 0

}//end loop reading lines of input (pass2)

//.asm's lines to error file
rewind(inputFile);//rewinds position indicator for 2nd pass
lineNumber = 0;//set line number back to zero



fprintf(err,"\n\n\n%s\n", "Multiply defined symbols:");
checkMulti();
fprintf(err, "\n\n\n%s\n", "Undefined symbols:");
printUndefined();

fprintf(err,"\n\n\n%s\n","File listings:");
while(fgets(inputLine, LINELENGTH, inputFile)!= NULL){
  lineNumber++;
  fprintf(err,"%d\t%s\n", lineNumber, inputLine);

}//end while there are lines remaining in the .asm file

if(errorFlag == 1){//one or more errors detected
  remove(symbolName);//delete symbol table file
  remove(objectName);//delete object code file
}//end if errors were detected

else{//no errors detected
  remove(errorName);//delete error file
}//end else errors weren't detected

return 0;}//end of main
