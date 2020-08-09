/******************************************
* Author: 
* 
* Date Created: 2020/08/08
* Date last updated: 2020/08/08
*/

// This is the first part of the JACK compiler that shall convert
// JACK programs to Stack based VM program

// This is done as first part of project 10 (tokenizer) defined in 
// The Elements of Computing Book by Noam Nisan and Shimon Schocken

// This follws Fig 10.5 Lexical Elements section in 
// Pg. 208 Chapter 10 of the reference book

// For sample tokenizer output see Page 220 of the reference book
#pragma warning(disable:4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXCHARPERLINE 80

// This function opens the input JACK file given in the command line
FILE* initializeInput(char* filename) {
	return (fopen(filename, "r"));
} //End initializeParser

//This function opens the file into which the vm output is written into 
// It takes the input file xxxx.jack and creates xxxx.vm as the Output filename
FILE* initializeOutput(char* filename) {
	int i = 0;
	while (filename[i] != '.') i++;
	filename[i + 1] = 't';
	filename[i + 2] = 'o';
	filename[i + 3] = 'k';
	filename[i + 4] = '\0';
	return(fopen(filename, "w"));
}

//The main() funciton takes the name of input jack file as command line
void main(int argc, char** argv) {
	FILE* inputFile;
	FILE* outputFile;

	int i, j, k;	//Temporary variables

	int lineNumber = 1; //This keeps the count of number of lines read
	bool partofComment = false;
	//The nextCommand string will store the command to be processed
	//This is called a buffer
	char nextCommand[MAXCHARPERLINE];
	//This parsedCommand string is nextCommand without whitespces
	char parsedCommand[MAXCHARPERLINE];
	//Temp strings
	char symbol1[MAXCHARPERLINE];
	//Stage 1: Tokenization

	//Opening the input and output files 
	inputFile = initializeInput(argv[1]);
	outputFile = initializeOutput(argv[1]);

	//Reading the inut file line after line till End of file
	//fgets() is used for reading.

	fprintf(outputFile, "<tokens>\n");
	while (fgets(nextCommand,MAXCHARPERLINE,inputFile) != NULL)
	{
		//Inside this loop we parse the command got through nextcommand
		i = 0;
		j = 0;
		parsedCommand[0] = '\0';	//Initializing parsed command 
		//
		// Section 1: Decommenting the line
		if (partofComment == true)	//Is current line is part of multiline comment
		{
			while (nextCommand[i] !='\n')	//Check if coment ends within this line
			{
				if ((nextCommand[i] == '*') && (nextCommand[i + 1] == '/'))
				{
					partofComment = false;
					i = i + 2;
					break;
				}
				i++;
			}
		}

		if (partofComment == false)		//comment had ended in this line or 
			//line is not part of comment
		{
			//Go till the first non whitespace character
			while ((nextCommand[i] == ' ') || (nextCommand[i] == '\t'))
			{
				i++;
			}
			// Check and remove comment at any part of line

			while (nextCommand[i] !='\n')
			{
				if ((nextCommand[i] == '/') && (nextCommand[i + 1] == '/'))
				{
					//The remaning part of line is comment
					break;
				}
				else
				{
					if ((nextCommand[i] == '/') && (nextCommand[i+1] == '*'))
						// A multiline comment starts
					{
						partofComment = true;
						while (nextCommand[i] !='\n')
						{
							if ((nextCommand[i] == '*') && (nextCommand[i+1] == '/'))
							{
								//Multiline comment ends within the line itself
								partofComment = false;
								i++;
								break;
							}
							i++;
						}
						if (partofComment == true) break;
						//Multiline comment does did not end within this line
					}
					else
					{
						parsedCommand[j] = nextCommand[i];
						j++;
					}
				}
				i++;
			}

			parsedCommand[j] = '\n';
			parsedCommand[j + 1] = '\0';

			//parsedCommand will have the non-commented part of any input line
			//That will be tokenized
			i = 0;
			while (parsedCommand[i] != '\n')
			{
				while ((parsedCommand[i] == ' ') || (parsedCommand[i] == '\t'))
				{
					i++;	//Remove white space
				}
				if ((parsedCommand[i] == '_') || \
					((parsedCommand[i] >= 'a') && (parsedCommand[i] <= 'z')) || \
					((parsedCommand[i] >= 'A') && (parsedCommand[i] <= 'Z')))
				{
					//Find for and Identifier or Keyword
					j = 0;
					while ((parsedCommand[i] == '_') || \
						((parsedCommand[i] >= 'a') && (parsedCommand[i] <= 'z')) || \
						((parsedCommand[i] >= 'A') && (parsedCommand[i] <= 'Z')))
					{
						symbol1[j] = parsedCommand[i];
						i++;
						j++;
					}
					symbol1[j] = '\0';
					if ((strcmp(symbol1,"class")==0) || (strcmp(symbol1, "constructor") == 0) || \
						(strcmp(symbol1, "function") == 0) || (strcmp(symbol1, "method") == 0 ) || \
						(strcmp(symbol1, "field") == 0) || (strcmp(symbol1, "static") == 0) || \
						(strcmp(symbol1, "var") == 0) || (strcmp(symbol1, "int") == 0) || \
						(strcmp(symbol1, "char") == 0) || (strcmp(symbol1, "boolean") == 0) || \
						(strcmp(symbol1, "void") == 0) || (strcmp(symbol1, "true") == 0) || \
						(strcmp(symbol1, "false") == 0) || (strcmp(symbol1, "null") == 0) || \
						(strcmp(symbol1, "this") == 0) || (strcmp(symbol1, "let") == 0) || \
						(strcmp(symbol1, "do") == 0) || (strcmp(symbol1, "if") == 0) || \
						(strcmp(symbol1, "else") == 0) || (strcmp(symbol1, "while") == 0) || \
						(strcmp(symbol1, "return") == 0) )

						fprintf(outputFile, "<keyword> %s </keyword>\n", symbol1);
					else
						fprintf(outputFile, "<identifier> %s </identifier>\n", symbol1);

				} //if IDENTIFIER
				else if (parsedCommand[i] == '\"')
				{
					//Find for an String constant
					i++;
					j = 0;
					while (parsedCommand[i] !='\"')
					{
						symbol1[j] = parsedCommand[i];
						i++;
						j++;
					}
					symbol1[j] = '\0';
					fprintf(outputFile, "<stringConstant> %s </stringConstant>\n", symbol1);
					i++;	//Already seen the (double quote (")) --increment to move forward
				}	//elseif STRING CONSTANT
				else if ((parsedCommand[i] >= '0') && (parsedCommand[i] <= '9')) 
				{
					//Find for an Integer Constant
					j = 0;
					while ((parsedCommand[i] >= '0') && (parsedCommand[i] <= '9'))
					{
						symbol1[j] = parsedCommand[i];
						i++;
						j++;
					}
					symbol1[j] = '\0';
					fprintf(outputFile, "<integerConstant> %s </integerConstant>\n", symbol1);
				}	//elseif INTEGER CONSTANT
				else
				{
					//check for symbol1
					switch (parsedCommand[i])
					{
					case '{':
					case '}':
					case '(':
					case ')':
					case '[':
					case ']':
					case '.':
					case ',':
					case ';':
					case '+':
					case '-':
					case '*':
					case '/':
					case '|':
					case '=':
					case '~':
						fprintf(outputFile, "<symbol> %c </symbol>\n",parsedCommand[i]);
						i++;
						break;
					case '&':
						fprintf(outputFile, "<symbol> &amp </symbol>\n");
						i++;
						break;
					case '<':
						fprintf(outputFile, "<symbol> &lt </symbol>\n", parsedCommand[i]);
						i++;
						break;
					case '>':
						fprintf(outputFile, "<symbol> &gt </symbol>\n", parsedCommand[i]);
						i++;
						break;
					case '\n':
						break;	//Needed for handling white spaces in the end before newline character
					default:
						printf("\n ERROR: in line number %d: Not knowing how to process <%s> with parsed \
								command as <%s> at position %d\n", lineNumber, nextCommand, parsedCommand, i);
						exit(0);
						break;
					}
				}	//ELSEIF SYMBOL
			}		// while ParsedCommand not reached end of line
		}	//If NOT partofComment
		lineNumber++;

	}	//End of while(fgets())
	
	// It is not advisable to use
	// fgets() in uncontrolled enviromnent as it does not check
	// for the buffer bounds. This can cause a vulunerabilitiy aclled
	// Buffer overflow
	fprintf(outputFile, "<tokens>\n");
	fclose(inputFile);
	fclose(outputFile);

}