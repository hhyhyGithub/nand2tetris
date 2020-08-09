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

//We shall define first an entry in the symbol table 

#define MAXLENGTH 20       //This is maximum length of any symbol or variable name
#define MAXENTRIES 1024    //Maximum number of entries in Symbol Table

//This is a single entry in Symboltable

struct SymbolTableEntry
{
	char symbol[MAXLENGTH];
	int address;
};

//Since the SymbolTable is going to be used by many functions
// we define it as a global variable as an array of
// MAXENTRIES of SymbolTableEntry. The following declaration 
// "CONSTRUCTS" the Symboltable
struct SymbolTableEntry SymbolTable[MAXENTRIES];

void InitializeSymbolTable();
void addEntry(struct SymbolTableEntry item);
int containGetAddress(char* symbol);
