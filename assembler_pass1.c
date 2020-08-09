#include<stdio.h>

#include "symtable.h"

#define A_COMMAND 0
#define C_COMMAND 1
#define L_COMMAND 2
#define L_AND_A_COMMAND 3 //This is addition to Pg 113 Label + A in same line
#define L_AND_C_COMMAND 4 //This is addition to Pg 113 Label + C in same line
#define COMMANDERROR 5

#define MAXCHARPERLINE 100

int commandType(char* parsedCommand){
    int i;
    if (parsedCommand[0] == '@') return A_COMMAND;
    if (parsedCommand[0] != '(') return C_COMMAND;
    if (parsedCommand[0] == '('){
        i=0;
        while ((parsedCommand[i] != '\n') && parsedCommand[i] != ')') 
            i++;
        if (parsedCommand[i] == ')')
        {
            if (parsedCommand[i+1] == '\n') return L_COMMAND;
            if (parsedCommand[i+1] == '@') 
                return L_AND_A_COMMAND;
            else
                return L_AND_C_COMMAND;
        }
        
    }
    return COMMANDERROR;
} // fun. commandType ends
/***********************End of Stage 2 **********************/


/***********Beginning of Stage 3 - PASS 2********************/

//Useful for A-instructions
void convertToBinary(int inpNum, char* outNum){
    for (int i = 0; i < 15; i++)
    {
        if(inpNum%2 == 0) outNum[i] = '0'; else outNum[i] ='1';
        inpNum = inpNum /2;
    }
    

}


FILE* initializeParser(char* filename){
    int i =0;
    char tempFile[MAXCHARPERLINE];
    strcpy(tempFile,filename);
    
    return (fopen(tempFile,"rt"));
}

// The Interfile is written in PASS 1
FILE* initializeInter(char* filename){
    int i =0;
    char tempFile[MAXCHARPERLINE];
    strcpy(tempFile,filename);
    while (tempFile[i] !='.') i++;
    tempFile[i+1] = 'i';
    tempFile[i+2] = 'n';
    tempFile[i+3] = 't';
    tempFile[i+4] = '\0';

    return (fopen(tempFile,"wt"));
}

//The InterFile is opened in PASS 2
FILE* initializeInterRead(char* filename){
    int i =0;
    char tempFile[MAXCHARPERLINE];
    strcpy(tempFile,filename);
    while (tempFile[i] !='.') i++;
    tempFile[i+1] = 'i';
    tempFile[i+2] = 'n';
    tempFile[i+3] = 't';
    tempFile[i+4] = '\0';

    return (fopen(tempFile,"rt"));
}
FILE* initializeOutput(char* filename){
    int i =0;
    char tempFile[MAXCHARPERLINE];
    strcpy(tempFile,filename);
    
    return (fopen(tempFile,"wt"));
}
//This converts Jump mnemonics to binary
void getJmpCode(char* inpJmp, char* jmp){
    if (strcmp(inpJmp,"JGT") == 0) strcpy(jmp,"001");
    if (strcmp(inpJmp,"JEQ") == 0) strcpy(jmp,"010");
    if (strcmp(inpJmp,"JGE") == 0) strcpy(jmp,"011");
    if (strcmp(inpJmp,"JLT") == 0) strcpy(jmp,"100");
    if (strcmp(inpJmp,"JNE") == 0) strcpy(jmp,"101");
    if (strcmp(inpJmp,"JLE") == 0) strcpy(jmp,"110");
    if (strcmp(inpJmp,"JMP") == 0) strcpy(jmp,"111");
}

//This converts Destination part of mnemonics to binary
void getDestcode(char* inpDest,char* dest){
    if (strcmp(inpDest,"M")== 0) strcpy(dest,"001");

    if (strcmp(inpDest,"D")== 0) strcpy(dest,"010");
    
    if (strcmp(inpDest,"MD")== 0) strcpy(dest,"011");
    if (strcmp(inpDest,"DM")== 0) strcpy(dest,"011");

    if (strcmp(inpDest,"A")== 0) strcpy(dest,"100");
    
    if (strcmp(inpDest,"AM")== 0) strcpy(dest,"101");
    if (strcmp(inpDest,"MA")== 0) strcpy(dest,"101");

    if (strcmp(inpDest,"AD")== 0) strcpy(dest,"110");
    if (strcmp(inpDest,"DA")== 0) strcpy(dest,"110");

    if (strcmp(inpDest,"AMD")== 0) strcpy(dest,"111");
    if (strcmp(inpDest,"ADM")== 0) strcpy(dest,"111");
    if (strcmp(inpDest,"DAM")== 0) strcpy(dest,"111");
    if (strcmp(inpDest,"DMA")== 0) strcpy(dest,"111");
    if (strcmp(inpDest,"MAD")== 0) strcpy(dest,"111");
    if (strcmp(inpDest,"MDA")== 0) strcpy(dest,"111");
}

//This converts Comute part of mnemonics to binary

void getCompCode(char* inpComp, char* comp){
    //'a' is '0'
    if ((strcmp(inpComp),"0") == 0) strcpy(comp,"0101010");
    if ((strcmp(inpComp),"1") == 0) strcpy(comp,"0111111");
    if ((strcmp(inpComp),"-1") == 0) strcpy(comp,"0111010");
    if ((strcmp(inpComp),"D") == 0) strcpy(comp,"0001100");
    if ((strcmp(inpComp),"A") == 0) strcpy(comp,"0110000");
    if ((strcmp(inpComp),"!D") == 0) strcpy(comp,"0001101");
    if ((strcmp(inpComp),"!A") == 0) strcpy(comp,"0110001");
    if ((strcmp(inpComp),"-D") == 0) strcpy(comp,"0001111");
    if ((strcmp(inpComp),"-A") == 0) strcpy(comp,"0110011");
    if ((strcmp(inpComp),"D+1") == 0) strcpy(comp,"0011111");
    if ((strcmp(inpComp),"A+1") == 0) strcpy(comp,"0110111");
    if ((strcmp(inpComp),"D-1") == 0) strcpy(comp,"0001110");
    if ((strcmp(inpComp),"A-1") == 0) strcpy(comp,"0110010");
    if ((strcmp(inpComp),"D+A") == 0) strcpy(comp,"0000010");
    if ((strcmp(inpComp),"D-A") == 0) strcpy(comp,"0010011");
    if ((strcmp(inpComp),"A-D") == 0) strcpy(comp,"0000111");
    if ((strcmp(inpComp),"D&A") == 0) strcpy(comp,"0000000");
    if ((strcmp(inpComp),"D|A") == 0) strcpy(comp,"0010101");

    //'a' is '1'
    if ((strcmp(inpComp),"M") == 0) strcpy(comp,"1110000");
    
    if ((strcmp(inpComp),"!M") == 0) strcpy(comp,"1110001");
    
    if ((strcmp(inpComp),"-M") == 0) strcpy(comp,"1110011");
    
    if ((strcmp(inpComp),"M+1") == 0) strcpy(comp,"1110111");
    
    if ((strcmp(inpComp),"M-1") == 0) strcpy(comp,"1110010");
    if ((strcmp(inpComp),"D+M") == 0) strcpy(comp,"1000010");
    if ((strcmp(inpComp),"D-M") == 0) strcpy(comp,"1010011");
    if ((strcmp(inpComp),"M-D") == 0) strcpy(comp,"1000111");
    if ((strcmp(inpComp),"D&M") == 0) strcpy(comp,"1000000");
    if ((strcmp(inpComp),"D|M") == 0) strcpy(comp,"1010101");
}

void main(int argc, char** argv){
    struct SymbolTableEntry item;

    //PASS 1 
    FILE* inputFile;

    //PASS2-Start
    FILE* interFile;
    //PASS2-End
    FILE* outputFile;
    int i,j,k; //Temporary variables
    int instructionNumber = 0; //This keeps the count of instructions
    int lineNumber = 0; //This keeps the count of number  of lines read

    //The nextCommand string will store the command to be processed
    // This is called a buffer
    char nextCommand[MAXCHARPERLINE];

    char parsedCommand[MAXCHARPERLINE];

    //Temporary strings for holding symbols
    char symbol1[MAXCHARPERLINE];
    char symbol2[MAXCHARPERLINE];

    InitializeSymbolTable(); //Initializing the Symbol Table

    //Open the input and output files
    inputFile = initializeParser(argv[1]);
    interFile = initializeInter(argv[1]);

    //Reading the input file line after line till End of file
    //fgets() is used for reading. This implements the steps
    // hasMoreCommands and advance steps in Pg. 113 of the book
    while (fgets(nextCommand,MAXCHARPERLINE,inputFile) != NULL)
    {
        //Inside this loop we parse the command got through nextCommand
        // This is PASS 1 of the assembler and used for only Symbol Resolution.
        i=0;
        j=0;

        parsedCommand[0] = '\0'; //Initializing parsed command

        while (nextCommand[i] != '\n')
        {
            //Remove all whitespaces
            if((nextCommand[i] != ' ') && (nextCommand[i] != '\t'))
                parsedCommand[j++] = nextCommand[i];
            i++;
        }
        parsedCommand[j] = '\n';    //Add the end of line
        parsedCommand[j+1] = '\0';  //Add the end of string

        //Remove comments if any inside a line
        i=0;
        while (parsedCommand[i] != '\n')
        {
            if ((parsedCommand[i] == '/') && (parsedCommand[i+1] == '/'))
            {
                parsedCommand[i] = '\n';
                parsedCommand[i+1] = '\0';
                break;
            }
            i++;
        }
        
        //Find the commandType and extract symols
        if(parsedCommand[0] !='\n') //This is an instruction
        {
            i = commandType(parsedCommand);
            if ((i == L_COMMAND) || (i == L_AND_A_COMMAND) || (i == L_AND_C_COMMAND))
            {
                j = 0;
                if((parsedCommand[1] < '0') || (parsedCommand[i]>'9'))
                {
                    while ((parsedCommand[j+1] != ')') && (parsedCommand[j+1]) != '\n')
                    {
                        //Extracts the first symbol in calse of L_, L_AND_A and L_AND_C
                        symbol1[j] = parsedCommand[j+1];
                        j++;
                    }
                    
                }
                symbol1 [j] = '\0';

                //Check for duplicate or illegal symbols like () -- No symbol or starting with num. or no ')'
                if ((containGetAddress(symbol1) != -2) || (j == 0) || (parsedCommand[j+1] != ')'))
                {
                    printf("ERROR: Duplicate or Illegal Label Definition for %s in Line %d with command %s\n",\
                    symbol1,lineNumber +1, parsedCommand);
                    exit(-1);
                }
                
                //Add the symbol1 to symbol table
                strcpy(item.symbol,symbol1);
                item.address = instructionNumber;
                addEntry(item);

                //Extract symbol2 in case of L_AND_A instruction
                if( i == L_AND_A_COMMAND)
                {
                    // Increment j by 1 to reach the ")" character in parsedCommand
                    // Increment j by 1 more to reach the "@" character in parsedCommand
                    // Increment j by 1 more to reach the first characer of A_Instruction in parsedCommand
                    j = j+3;
                    if((parsedCommand[j]<'0') || (parsedCommand[j] >'9'))
                    {
                        k = 0;
                        while (parsedCommand[j] != '\n')
                        {
                            // Extracts the second symbol in case of L_AND_A
                            symbol2[k] = parsedCommand[j];
                            j++;
                            k++;
                        }
                        symbol2[k] = '\0';
                        //Add the symbol2 to symbol table
                        strcpy(item.symbol,symbol2);
                        item.address = -1;
                        addEntry(item);
                    } 
                }
            }

            //Handle Symbols in A_COMMAND
            if (i == A_COMMAND)
            {
                j=0;
                if((parsedCommand[1]<'0') || (parsedCommand[1]>'9'))
                {
                    while ((parsedCommand[j+1] != ')') && (parsedCommand[j+1] !='\n'))
                    {
                        //Extracts the first symbol in calse of L_,L_AND_A and L_AND_C
                        symbol1[j] = parsedCommand[j+1];
                        j++;
                    }
                    symbol1[j] = '\0';
                    //Add the symbol1 to symbol table
                    strcpy(item.symbol,symbol1);
                    item.address = -1;
                    addEntry(item);
                }
            }
            
            if( i == COMMANDERROR )
            {
                //Diagnostics 
                printf("Error in Line number %d\n",lineNumber);
                exit(-1);
            }

            if (i != L_COMMAND)
            {
                if (i != L_AND_A_COMMAND)
                {
                    fprintf(interFile,"%s",parsedCommand);
                }
                else
                {
                    k=0;
                    while (parsedCommand[k] != ')') k++;
                    //k++;
                    while (parsedCommand[k] != '\n')
                    {
                        fprintf(interFile,"%c",parsedCommand[k]);
                        k++;
                    }
                    fprintf(interFile,"\n");
                    
                }
                instructionNumber++;
                
            }
            
        }
        lineNumber ++;
    }
    
    //It is not advisable to use
    // fgets() in uncontrolled enviroment as it does not check
    // for the buffer bounds. This can cause a vulnerability called
    // Buffer overflow

    //Assign Address to Data varaiables starting from 16. These are symbols with address -1

    j=16;
    for ( i = 0; i < MAXENTRIES; i++)
    {
        if((strcmp(SymbolTable[i].symbol,"\0") !=0) && (SymbolTable[i].address == -1))
            SymbolTable[i].address = j++;
    }
    
    //Closing the input and output files
    fclose(inputFile);
    fclose(interFile);
    
    //Beginning of PASS 2
    interFile = initializeInterRead(argv[1]);
    outputFile = initializeOutput(argv[1]);

    //... ...

    fclose(interFile);
    fclose(outputFile);
    
}