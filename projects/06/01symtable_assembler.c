/*******************************************************
 * Author : Prof. V. Kamakoti
 * RISE LAB, CSE Dept,
 * IIT Madras
 * Date Created: 28 Nov, 2018
 * Date last updated: 28 Nov,2018
 * *****************************************************/

 // This is the HACK Assembler that shall convert
 // HACK Assembly program in Mnemonics to 16-bit Binary
 // HACK Machine Language program

 //This is done as project 06 define in 
 // The Elements of Computing Book by Noam Nisan and Shimon Schocken

#include <stdio.h>
#include <string.h>

#include "symtable.h"

//The following function initializes the Symbol Table
// This fill up the predefined symbols as given in the table
// in Section 6.2.3 (Pg. 110 ) of the book

void InitializeSymbolTable() {
	memset(SymbolTable, 0, sizeof(struct SymbolTableEntry) * MAXENTRIES);

	strcpy(SymbolTable[0].symbol, "SP");
	strcpy(SymbolTable[1].symbol, "LCL");
	strcpy(SymbolTable[2].symbol, "ARG");
	strcpy(SymbolTable[3].symbol, "THIS");
	strcpy(SymbolTable[4].symbol, "THAT");
	strcpy(SymbolTable[5].symbol, "R0");
	strcpy(SymbolTable[6].symbol, "R1");
	strcpy(SymbolTable[7].symbol, "R2");
	strcpy(SymbolTable[8].symbol, "R3");
	strcpy(SymbolTable[9].symbol, "R4");
	strcpy(SymbolTable[10].symbol, "R5");
	strcpy(SymbolTable[11].symbol, "R6");
	strcpy(SymbolTable[12].symbol, "R7");
	strcpy(SymbolTable[13].symbol, "R8");
	strcpy(SymbolTable[14].symbol, "R9");
	strcpy(SymbolTable[15].symbol, "R10");
	strcpy(SymbolTable[16].symbol, "R11");
	strcpy(SymbolTable[17].symbol, "R12");
	strcpy(SymbolTable[18].symbol, "R13");
	strcpy(SymbolTable[19].symbol, "R14");
	strcpy(SymbolTable[20].symbol, "R15");
	strcpy(SymbolTable[21].symbol, "SCREEN");
	strcpy(SymbolTable[22].symbol, "KBD");
	
	SymbolTable[0].address = 0;
	SymbolTable[1].address = 1;
	SymbolTable[2].address = 2;
	SymbolTable[3].address = 3;
	SymbolTable[4].address = 4;

	SymbolTable[5].address = 0;
	SymbolTable[6].address = 1;
	SymbolTable[7].address = 2;
	SymbolTable[8].address = 3;
	SymbolTable[9].address = 4;
	SymbolTable[10].address = 5;
	SymbolTable[11].address = 6;
	SymbolTable[12].address = 7;
	SymbolTable[13].address = 8;
	SymbolTable[14].address = 9;
	SymbolTable[15].address = 10;
	SymbolTable[16].address = 11;
	SymbolTable[17].address = 12;
	SymbolTable[18].address = 13;
	SymbolTable[19].address = 14;
	SymbolTable[20].address = 15;
	SymbolTable[21].address = 0x4000;  //16384
	SymbolTable[22].address = 0x6000;  //24576

	//Initialize all other entries to NULL symbol and -1 as address
	for (int i = 23; i < MAXENTRIES; i++)
	{
		strcpy(SymbolTable[i].symbol, "\0");
		SymbolTable[i].address = -1;
	}

}

void addEntry(struct SymbolTableEntry item)
{
	//Check if Entry already exists
	int i = 0;
	//Keep checking till the Entry is either there are we have scanned all the Entries entered so far

	while ((strcmp(SymbolTable[i].symbol, item.symbol) != 0) &&
		(strcmp(SymbolTable[i].symbol, "\0") != 0) && (i < MAXENTRIES))
	{
		i++;
	}
	// The symbol is not contained in the symbol Table, we add it.
	if (strcmp(SymbolTable[i].symbol, "\0") == 0) {
		strcpy(SymbolTable[i].symbol, item.symbol);
		SymbolTable[i].address = item.address;
	}
	//The symbol is contained in the Symbol Table but address is -1
	// This is the case that arises when we have already processed @END much before
	// and currently processing (END), we need to update the address

	if ((strcmp(SymbolTable[i].symbol, item.symbol) == 0) &&
		(SymbolTable[i].address == -1))
	{
		SymbolTable[i].address = item.address;
	}

	// We have more symbols then what is permitted. This must be reported.
	if (i > MAXENTRIES) printf("Error: Symbol Table full. \
    Increase MAXENTRIES by two folds and recompile the Assembler\n");

}// function addEntry end

//The following function checks Membership of a given symbol in the symbol table
//It returns the address associated with the symbol if it is found
//else it return -1
//This is invoked only in Pass 2 during which all symbols are assigned an integer address

int containGetAddress(char* symbol) {
	int i = 0;
	while ((i < MAXENTRIES) && (SymbolTable[i].address != -1))
	{
		//If symbol is found return its address
		if (strcmp(SymbolTable[i].symbol, symbol) == 0)
			return (SymbolTable[i].address);
		i++;
	}
	return -1;// Will reach here only if symbol is not in the symbol table

}

/*
void main() {
	printf("HACK assembler test ... \n");
	struct SymbolTableEntry item;
	//PASS 1

	InitializeSymbolTable();

	strcpy(item.symbol, "New");
	item.address = 16;
	addEntry(item);
	printf("New is in entry %d\n", containGetAddress("New"));

	strcpy(item.symbol, "New1");
	item.address = 17;
	addEntry(item);
	printf("New1 is in entry %d\n", containGetAddress("New1"));

	return;
}
*/