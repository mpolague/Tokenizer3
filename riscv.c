#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#define INPUT_LIM 50 //limit on how many characters a user can input

int64_t* r; // Array of 32 64-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 64-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	r = malloc(reg_amount * sizeof(int64_t)); // 32 * 8 bytes
	for(int i = 0; i < 32; i++){
		r[i] = 0;
	}
}


/**
 * Print out all of the contents that are inside of the registers.
 */
void print_regs(){
  int col_size = 0;
  for(int i = 0; i < 8; i++){
    printf("X%2i:%.*lld ", i, col_size, (long long int) r[i]);
    printf("X%2i:%.*lld ", i+8, col_size, (long long int) r[i+8]);
    printf("X%2i:%.*lld ", i+16, col_size, (long long int) r[i+16]);
    printf("X%2i:%.*lld ", i+24, col_size, (long long int) r[i+24]);
  }
  printf("\n");
}


int string_compare(char str1[], char str2[]){
  int ctr = 0;
  while(str1[ctr] == str2[ctr]){
    if(str1[ctr] == '\0' || str2[ctr] == '\0')
      break;
    ctr++;
  }
  if(str1[ctr] == '\0' && str2[ctr] == '\0')
    return 0;
  else
    return -1;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
  char **token = tokenize(instr); //tokenizes the string
  //created an array containing the first POSSIBLE words
  char *first[] = {"LW", "LD", "SW", "SD", "ADD", "ADDI", "SLLI", "SRLI", "AND", "OR", "XOR"};
  int check_instr;
  char *instru[1];
  char *firstReg[1];
  char *secReg;
 
  int size_first = sizeof first / sizeof *first; //size of of array of first

  //-------------------TAKING CARE OF FINDING THE INSTRUCTION----------------------------
  for(int i = 0; i < size_first; i++){ //will iterate through the list and check if it exists
    check_instr = string_compare(token[0], first[i]);
    if(check_instr == 0){ //if found, it equals 0
      instru[0] = first[i];
      break;
    }
  }
  if(check_instr == 0){
    printf("-> INSTRUCTION: %s \n", instru[0]);
  }
  else{
    return false;
  }
  //-------------------THE FIRST REGISTER WILL BE THE SECOND TOKEN----------------------
  firstReg[0] = token[1];
  printf("-> DEST REGISTER: %s \n", firstReg[0]);
  char *xOut = strtok(firstReg[0], "X"); //still not tokenized
  int xOut2 = atoi(xOut); //converts string to int
  r[xOut2] = read_address(0x00, "mem.txt"); //taking whatever value
  printf("------> This is what is in r[0x00] is : %ld \n", r[xOut2]);
  
  //------------------CHECKING IF INSTR = LW or LD or SW or SW -------------------------
  if(instru[0] == "LW" || instru[0] == "LD" || instru[0] == "SW" || instru[0] =="SD"){
    secReg = strtok(token[2], "("); //still not tokenized
    char *imm = secReg; //the immediate of the second register
    char *secondReg; //actual second register
    
    while(secReg != NULL){
      secondReg = secReg;
      secReg = strtok(NULL, "(");
    }
    
    printf("-> IMMEDIATE FOR SECOND REGISTER: %s\n", imm);
    printf("-> SECOND REGISTER: %s\n", secondReg);
  } 
  //--------CHECKING IF INSTRUCTION IS   "ADD"  or  "AND"  or   "OR"  or   "XOR"----------------
  char *secondRegExtra[1];
  char *thirdRegExtra[1];
  
  if(instru[0] == "ADD" || instru[0] == "AND" || instru[0] == "OR" || instru[0] == "XOR"){
    secondRegExtra[0] = token[2];
    printf("-> SECOND REGISTER: %s\n", secondRegExtra[0]);
    char *xOut3 = strtok(secondRegExtra[0], "X"); //still not tokenized
    int xOut4 = atoi(xOut3); //converts string to int
    r[xOut4] = read_address(0x08, "mem.txt"); //taking whatever value
    printf("------> This is what is in r[0x08] is : %ld \n", r[xOut4]);
   
    thirdRegExtra[0] = token[3];
    printf("-> THIRD REGISTER: %s\n", thirdRegExtra[0]);
    char *xOut6;
    xOut6 = strtok(thirdRegExtra[0], "X"); //still not tokenized
    int xOut7 = atoi(xOut6); //converts string to int
    r[xOut7] = read_address(0x10, "mem.txt"); //taking whatever value
    printf("------> This is what is in r[0x10] is : %ld \n", r[xOut7]);
    
    if(instru[0] == "ADD"){
      r[xOut2] = r[xOut4] + r[xOut7]; //ADD INSTRUCTION
    }
    if(instru[0] == "AND"){ //AND INSTRUCTION
      r[xOut2] = r[xOut4] & r[xOut7]; 
    }
    if(instru[0] == "OR"){ //OR INSTRUCTION
      r[xOut2] = r[xOut4] | r[xOut7];; 
    }
    if(instru[0] == "XOR"){ //XOR INSTRUCTION
      r[xOut2] = r[xOut4] ^ r[xOut7];; 
    }
    write_address(r[xOut4], 0x20, "mem.txt");

    printf("-----------------> %s was replaced with : %ld \n", firstReg[0], r[xOut2]);
    printf("\n");
  }
  //-------------------CHECKING IF INSTRUCTION IS   "ADDI"   or   "SLLI"   or   "SRLI" -----------
  char *immediates[1];
  char *secondRegImm[1];
  if(instru[0] == "ADDI" || instru[0]== "SLLI" || instru[0]=="SRLI"){
    secondRegImm[0] = token[2];
    immediates[0] = token[3];
    printf("-> SECOND REGISTER %s\n", secondRegImm[0]);
    char *xOut8= strtok(secondRegImm[0], "X"); //still not tokenized
    int xOut9 = atoi(xOut8); //converts string to int
    r[xOut9] = read_address(0x08, "mem.txt"); //taking whatever value
    printf("------> This is what is in r[0x10]: %ld \n", r[xOut9]);
    
    printf("-> IMMEDIATE: %s\n", immediates[0]);
    write_address(immediates[0], 0x20, "mem.txt");
    printf("-----------------> %s was replaced with : %ld \n", firstReg[0], immediates[0]);
  }
  
  return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(int64_t data_to_write, int64_t address, char* mem_file){
	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int64_t write = write_address(data_to_write, address, mem_file);
	if(write == (int64_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int64_t read = read_address(address, mem_file);
	
	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}


/** 
 * Your code goes in the main
 * 
 */
int main(){
  // Do not write any code between init_regs
  init_regs(); // DO NOT REMOVE THIS LINE

  // Below is a sample program to a write-read. Overwrite this with your own code.
	
  //TOKENIZER CALLS
  printf("---------------------------------------------------------------------------\n");
  printf("--Hello, this program only takes in instructions with spaces. Not commas.--\n");
  printf("---------------------------------------------------------------------------\n");
 
  char user_input[INPUT_LIM]; 
  //will ask for user input until they enter ctrl+D
  init_regs(); //DO NOT REMOVE THIS LINE
  //----THIS IS HOW WE ACCESS DATA FROM THE MEM.TXT AND OVERWRITE IT AS WELL----
  //int64_t data = 0xFFF;
  //int64_t address = 0x20;
  //char* fileName = "mem.txt";
  //write_read_demo(data, address, fileName);
  
  //----THIS IS HOW YOU GET THE ADDRESSES OF THE REGISTER ARRAY AND ITS ELEMENTS----
  //printf("------\n");
  //r[0] = 3;
  //printf("ZERO %ld \n",r[0]);
  //printf("second %ld \n", r[1]);
  //printf("------");
  
  while(1){
    printf("-> Enter an instruction or use ctrl+D on windows to exit the program\n");
    printf("$ ");
    fgets(user_input,INPUT_LIM,stdin); //get user input
    char *str = user_input;

    //----------------CHECK IF CONTROL D PRESSED -------------------
    if (str[0] == '#'){ //FOR NOW EOF = /n
      break;
    }
    //--------------------------------------------------------------
    
    bool correctlyExec =  interpret(str); //calls interpret function from riscv.c file
    if (correctlyExec == 0){ //if not executed correctly
      printf("---> Sorry. This was not executed correctly.\n");
    }
  }
  printf("------------------------\n");
  printf("-- Printing registers: -- \n");
  printf("------------------------\n");
  print_regs();
  printf("\n");  

  return 0;
}
