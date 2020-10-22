#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#define INPUT_LIM 50 //limit on how many characters a user can input

int64_t* r; // Array of 32 64-bit registers
int lines = 0;

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
  int col_size = 8;
  for(int i = 0; i < 8; i++){
    printf("X%02i:%.*lld ", i, col_size, (long long int) r[i]);
    printf("X%02i:%.*lld ", i+8, col_size, (long long int) r[i+8]);
    printf("X%02i:%.*lld ", i+16, col_size, (long long int) r[i+16]);
    printf("X%02i:%.*lld\n", i+24, col_size, (long long int) r[i+24]);

    
    //    printf("X%02i:%.lld ", i, col_size, (long long int) r[i]);
    //    printf(" X%02i:%.lld ", i+8, col_size, (long long int) r[i+8]);
    //    printf(" X%02i:%.lld ", i+16, col_size, (long long int) r[i+16]);
    //    printf(" X%02i:%.lld\n ", i+24, col_size, (long long int) r[i+24]);
  }
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
  
  //------------------CHECKING IF INSTR = LW or LD or SW or SW -------------------------
  if(instru[0] == "LW" || instru[0] == "LD" || instru[0] == "SW" || instru[0] =="SD"){
    secReg = strtok(token[2], "("); //still not tokenized
    char *imm = secReg; //the immediate of the second register
    char *secondReg; //actual second register
    char *copySec;
    while(secReg != NULL){
      secondReg = secReg;
      secReg = strtok(NULL, "(");
    }
    secondReg = strtok(secondReg, ")");
    while(secondReg != NULL){
      copySec = secondReg;
      secondReg = strtok(NULL, ")");
    }
    
    printf("-> IMMEDIATE FOR SECOND REGISTER: %s\n", imm);
    printf("-> SECOND REGISTER: %s\n", copySec);
    char *se = strtok(copySec, "X"); //still not tokenized
    int toInt = atoi(se); //converts string to int
    //r[toInt] = read_address(0x08, "mem.txt"); //taking whatever value
    int Immed = atoi(imm);
    
    if(instru[0] == "LW"){
      r[xOut2] = r[toInt]+Immed;
      write_address(r[xOut2], lines, "mem.txt");
    }
    if(instru[0] == "LD"){
      r[xOut2] = r[toInt]+Immed;
      write_address(r[xOut2], lines, "mem.txt");
    }
    if(instru[0] == "SW"){
      r[toInt] = r[toInt] + Immed;
      r[toInt] = r[xOut2];
      write_address(r[toInt], lines, "mem.txt");
    }
    if(instru[0] =="SD"){
      r[toInt] = r[toInt] + Immed;
      r[toInt] = r[xOut2];
      write_address(r[toInt], lines, "mem.txt");
    }

    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }
    
  } 
  //--------CHECKING IF INSTRUCTION IS   "ADD"  or  "AND"  or   "OR"  or   "XOR"----------------
  char *secondRegExtra[1];
  char *thirdRegExtra[1];
  
  if(instru[0] == "ADD" || instru[0] == "AND" || instru[0] == "OR" || instru[0] == "XOR"){
    secondRegExtra[0] = token[2];
    printf("-> SECOND REGISTER: %s\n", secondRegExtra[0]);
    char *xOut3 = strtok(secondRegExtra[0], "X"); //still not tokenized
    int xOut4 = atoi(xOut3); //converts string to int
   
    thirdRegExtra[0] = token[3];
    printf("-> THIRD REGISTER: %s\n", thirdRegExtra[0]);
    char *xOut6;
    xOut6 = strtok(thirdRegExtra[0], "X"); //still not tokenized
    int xOut7 = atoi(xOut6); //converts string to int
    
    if(instru[0] == "ADD"){
      r[xOut2] = r[xOut4] + r[xOut7]; //ADD INSTRUCTION
    }
    if(instru[0] == "AND"){ //AND INSTRUCTION
      r[xOut2] = r[xOut4] & r[xOut7]; 
    }
    if(instru[0] == "OR"){ //OR INSTRUCTION
      r[xOut2] = r[xOut4] | r[xOut7]; 
    }
    if(instru[0] == "XOR"){ //XOR INSTRUCTION
      r[xOut2] = r[xOut4] ^ r[xOut7];
    }
    write_address(r[xOut2], lines, "mem.txt");
    
    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }
    printf("\n");
  }
  //-------------------CHECKING IF INSTRUCTION IS   "ADDI"   or   "SLLI"   or   "SRLI" -----------
  char *immediates[1];
  char *secondRegImm[1];
  if(instru[0] == "ADDI"){
    secondRegImm[0] = token[2];
    immediates[0] = token[3];
    printf("-> SECOND REGISTER %s\n", secondRegImm[0]);
    char *xOut8= strtok(secondRegImm[0], "X"); //still not tokenized
    int xOut9 = atoi(xOut8); //converts string to int
    
    printf("-> IMMEDIATE: %s\n", immediates[0]);
    //write_address(immediates[0], 0x20, "mem.txt");
    r[xOut9] = atoi(immediates[0]);
    write_address(r[xOut9], lines, "mem.txt");
    
    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }

  } //---------------------------------------------------------------
  if(instru[0]== "SLLI" || instru[0] == "SRLI"){
    char *sec[1];
    sec[0] = token[2]; //REGISTER 2
    printf("-> SECOND REGISTER %s\n", sec[0]);
    char *xOut10= strtok(sec[0], "X"); //still not tokenized
    int xOut11 = atoi(xOut10); //converts string to int
    
    char *immediates[1];
    immediates[0] = token[3];
    printf("-> IMMEDIATE: %s\n", immediates[0]);
    int howMuch = atoi(immediates[0]);
    char *new;
    if(instru[0]== "SLLI"){
      r[xOut2] = r[xOut11] << howMuch;
    }
    
    if(instru[0] == "SRLI"){
      r[xOut2] = r[xOut11] >> howMuch;
    }
    
    write_address(r[xOut2], lines, "mem.txt");
    
    if(lines == 0 || lines%10 == 0){
      //put this in like 0 by creating a variable "0x" then add 08
      lines = lines + 8;
    }else{
      //increment lines + 10
      lines = lines + 10;
    }

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
