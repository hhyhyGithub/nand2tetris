#pragma warning(disable:4996)

//This is the VM Translator that shall covert
// Stack based VM program to HACK Mnemonics
// This will translate the Arithmetic / logical and 
// Memory access VM Instrucitons

//This is done as project 07 define in 
// The Elements of Computing Book by Noam Nisan and Shimon Schocken

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHARPERLINE	80

//This function opens the input VM file given in the command line
FILE* initializeParser(char* filename) {
	return (fopen(filename, "rt"));
}

//This funciton opens the file into which the mnemonic output is written into
// It takes the input file Xxxxx.vm and creates Xxxxx.asm as the output filename
FILE* initializeOutput(char* filename) {
	int i = 0;

	while (filename[i] != '.') i++;

	filename[i + 1] = 'a';
	filename[i + 2] = 's';
	filename[i + 3] = 'm';
	filename[i + 4] = '\0';

	return (fopen(filename, "wt"));
}

void fileNameforLabel(char* filename, char* outfileName) {
	int x = strlen(filename);    
	char ch = '\\';    
	char* q = strrchr(filename, ch) + 1;	//提取路径中的文件名
	int i = 0;
	
	while (q[i] != '.') 
	{
		//if ((q[i] != '/') && (q[i] == '\\'))
			outfileName[i] = q[i];
		i++;
	}
	

	outfileName[i] = '.';
	outfileName[i + 1] = '\0';

}

char* getSegment(char* symbol2, char* symbol3, char* filename) {
	if (strcmp(symbol2, "argument") == 0) return "ARG";
	if (strcmp(symbol2, "local") == 0) return "LCL";
	if (strcmp(symbol2, "this") == 0) return "THIS";
	if (strcmp(symbol2, "that") == 0) return "THAT";
	if (strcmp(symbol2, "pointer") == 0) return "3";
	if (strcmp(symbol2, "temp") == 0) return "5";
	if (strcmp(symbol2, "constant") == 0) return symbol3;
	if (strcmp(symbol2, "static") == 0) return strcat(filename, symbol3);

}

void generateCode(FILE* outputFile, char* symbol1, char* symbol2, char* symbol3, \
	int* ALabelNum, int lineNumber, char* filename)
{
	if ((strcmp(symbol1, "push") != 0) && (strcmp(symbol1, "pop") != 0))
	{
		//StackArithmetic
		//add
		if (strcmp(symbol1, "add") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=M+D\n");
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
		}
		//sub
		if (strcmp(symbol1, "sub") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=M-D\n");
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
		}

		//and
		if (strcmp(symbol1, "and") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=M&D\n");
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
		}
		//or
		if (strcmp(symbol1, "or") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=M|D\n");
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
		}

		//neg
		if (strcmp(symbol1, "neg") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=-M\n");
		}
		//not
		if (strcmp(symbol1, "not") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=!M\n");
		}

		//eq
		if (strcmp(symbol1, "eq") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M-D\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum);
			fprintf(outputFile, "D;JEQ\n");
			fprintf(outputFile, "D=0\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum + 1);
			fprintf(outputFile, "0;JMP\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum);
			fprintf(outputFile, "D=-1\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum + 1);
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=D\n");
			*ALabelNum += 2;
		}

		//lt
		if (strcmp(symbol1, "lt") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M-D\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum);
			fprintf(outputFile, "D;JLT\n");
			fprintf(outputFile, "D=0\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum + 1);
			fprintf(outputFile, "0;JMP\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum);
			fprintf(outputFile, "D=-1\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum + 1);
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=D\n");
			*ALabelNum += 2;
		}
		//gt
		if (strcmp(symbol1, "gt") == 0) {
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M-D\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum);
			fprintf(outputFile, "D;JGT\n");
			fprintf(outputFile, "D=0\n");
			fprintf(outputFile, "@ALable_%d\n", *ALabelNum + 1);
			fprintf(outputFile, "0;JMP\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum);
			fprintf(outputFile, "D=-1\n");
			fprintf(outputFile, "(ALable_%d)\n", *ALabelNum + 1);
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "M=D\n");
			*ALabelNum += 2;
		}
	}
	else
	{
		//MemoryAccess
		if (strcmp(symbol1, "push") == 0)	//push instruction
		{
			//Address of pointer to segment in A
			fprintf(outputFile, "@%s\n", getSegment(symbol2, symbol3, filename));
			if (strcmp(symbol2, "constant") == 0)
			{
				fprintf(outputFile, "D=A\n");
			}
			else
			{
				if ((strcmp(symbol2, "temp") == 0) || (strcmp(symbol2, "pointer") == 0))
					fprintf(outputFile, "D=A\n");
				else
					fprintf(outputFile, "D=M\n");	//D contains start address of Segment
				fprintf(outputFile, "@%s\n", symbol3);	//The index is in A register
				fprintf(outputFile, "A=A+D\n");		// A address has the memory of the value to be pushed
				fprintf(outputFile, "D=M\n");			// D has the value to be pushed
			}
			//Push the value in D and Increment SP
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "M=D\n");
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M+1\n");
		}
		if (strcmp(symbol1, "pop") == 0)		//Pop Instuction
		{
			// Address of pointer to segment in A
			fprintf(outputFile, "@%s\n", getSegment(symbol2, symbol3, filename));
			if (strcmp(symbol2, "constant") == 0)
			{
				printf("ERROR: Cannot POP into a constant segment in Line Number %d\n", lineNumber);
				exit(-1);
			}
			else
			{
				if ((strcmp(symbol2, "temp") == 0) || (strcmp(symbol2, "pointer") == 0))
					fprintf(outputFile, "D=A\n");
				else
					fprintf(outputFile, "D=M\n");
				fprintf(outputFile, "@%s\n", symbol3); // The index is in A register
				fprintf(outputFile, "D=A+D\n");		// D has the memory address to which we need to pop
				fprintf(outputFile, "@R13\n");			// Temporary Value
				fprintf(outputFile, "M=D\n");			// R13 has the address 
			}
			//Pop the value and decrement SP
			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "A=M\n");
			fprintf(outputFile, "A=A-1\n");
			fprintf(outputFile, "D=M\n");	// D has the value
			fprintf(outputFile, "@R13\n");	//A is now 13
			fprintf(outputFile, "A=M\n");	//Retrive Address
			fprintf(outputFile, "M=D\n");	//Store the Value in D

			fprintf(outputFile, "@SP\n");
			fprintf(outputFile, "M=M-1\n");	//Decrement SP

		}
	}
}
//The main() function takes the name of input VM file as command line
void main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage:%s filename.vm\n", argv[0]);
		return;
	}

	FILE* inputFile;
	FILE* outputFile;

	int i, j, k;	//Temporary variables
	int ALabelNum = 0;	//Lable Number for Arithmetic Instructions
	int lineNumber = 0;	//This keeps the count of number of lines read

	//The nextCommand string will store the command to be processed
	//This is called a buffer
	char nextCommand[MAXCHARPERLINE];
	//This is parsedCommand string is nextCommand without whitespaces
	char parsedCommand[MAXCHARPERLINE];

	//Temporary strings for holding symbols
	char symbol1[MAXCHARPERLINE];
	char symbol2[MAXCHARPERLINE];
	char symbol3[MAXCHARPERLINE];
	char symbol4[MAXCHARPERLINE];

	//Opening the input and output files
	inputFile = initializeParser(argv[1]);
	outputFile = initializeOutput(argv[1]);
	if (inputFile ==NULL || outputFile ==NULL) return;

	fileNameforLabel(argv[1], symbol4);
	//Reading the input file line after line till End of file
	//fgets() is used for reading.
	while (fgets(nextCommand, MAXCHARPERLINE, inputFile) != NULL)
	{
		//Inside this loop we parse the command got through nextCommand
		i = 0;
		j = 0;
		parsedCommand[0] = '\0'; 	//Initializing parsed command 

		//Go till the first non whitespace character
		while ((nextCommand[i] == ' ') || (nextCommand[i] == '\t'))
			i++;


		//Check and remove comment at any part of line
		while (nextCommand[i] != '\n')
		{
			if ((nextCommand[i] == '/') && (nextCommand[i + 1] == '/'))
			{
				break;
			}
			else {
				parsedCommand[j] = nextCommand[i];
			}
			i++;
			j++;
		}//while  ends
		parsedCommand[j] = '\n';
		parsedCommand[j + 1] = '\0';

		//Extracting Symbols
		i = 0;
		symbol1[0] = '\0';
		symbol2[0] = '\0';
		symbol3[0] = '\0';

		//Extract first symbol
		while ((parsedCommand[i] != ' ') && (parsedCommand[i] != '\t') && \
			(parsedCommand[i] != '\n'))
		{
			symbol1[i] = parsedCommand[i];
			i++;
		}
		symbol1[i] = '\0';

		//Goto Second symbol
		while ((parsedCommand[i] == ' ') || (parsedCommand[i] == '\t')) i++;
		//Check if end of line
		if (parsedCommand[i] != '\n')
		{
			//Extract Symbol 2
			j = 0;
			while ((parsedCommand[i] != ' ') && (parsedCommand[i] != '\t') && \
				(parsedCommand[i] != '\n'))
				symbol2[j++] = parsedCommand[i++];
			symbol2[j] = '\0';

			//Goto Symbol 3
			while ((parsedCommand[i] == ' ') || (parsedCommand[i] == '\t')) i++;
			//Check if end of line
			if (parsedCommand[i] != '\n')
			{
				//Extract Symbol 3
				j = 0;
				while ((parsedCommand[i] != ' ') && (parsedCommand[i] != '\t') && \
					(parsedCommand[i] != '\n'))
					symbol3[j++] = parsedCommand[i++];
				symbol3[j] = '\0';
			}
		}

		fprintf(outputFile, "// %s", parsedCommand);
		if (symbol1[0] != '\0')
			generateCode(outputFile, symbol1, symbol2, symbol3, \
				&ALabelNum, lineNumber, symbol4);
		
		fileNameforLabel(argv[1], symbol4);
		//(nextCommand -> parsedCommand)
	}// while 1 ends (fgets() each line )
	
	// It is not advisable to use
	// fgets() in uncontroled enviroment as it does not check
	// for the buffer bounds . This can cause a vulnerability called
	//Buffer overflow

	fclose(inputFile);
	fclose(outputFile);
}
