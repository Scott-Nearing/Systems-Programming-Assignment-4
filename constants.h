//Scott Nearing
//sn379745

//file description
//This file defines constants that are used throughout the program

#define FILEEXTENTIONLENGTH  4//length of new file extentions

//The follwing are used for the opcode array and shifting opperations
#define AZERO 0
#define AONE 1
#define ATWO 2
#define ATHREE 3
#define AFOUR 4
#define AFIVE 5
#define ASIX 6
#define ASEVEN 7
#define AEIGHT 8
#define ANINE 9
#define ATEN 10
#define AELEVEN 11
#define ATWELVE 12
#define ATHIRTEEN 13
#define AFOURTEEN 14
#define AFIFTEEN 15
#define ASIXTEEN 16
#define ASEVENTEEN 17
#define AEIGHTEEN 18
#define ANINETEEN 19
#define ATWENTY 20
#define ATWENTYONE 21
#define ATWENTYTWO 22
#define ATWENTYTHREE 23
#define ATWENTYFOUR 24
#define ATWENTYFIVE 25
#define ATWENTYSIX 26
#define ATWENTYSEVEN 27
#define ATWENTYEIGHT 28
#define ATWENTYNINE 29
#define ATHIRTY 30
#define ATHIRTYONE 31
#define ATHIRTYTWO 32
#define ATHIRTYTHREE 33
#define ATHIRTYFOUR 34
#define ATHIRTYFIVE 35

//the following are masks used to assemble object code
#define WORDMASK 255
#define IMASKMINUS 65535
#define IMASKPLUS -32769

#define LINELENGTH 81 //lines can't be longer than 81

#define ARRAYSIZE 36 //opcode array only holds 36 elements

#define EXPECTEDARGS 2 //program should be run with only 2 arguments(file name and file to be assembled)
