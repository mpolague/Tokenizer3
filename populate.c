#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_32bit 4294967295; // 32 bits of all 1s
#define SEED 3432

char* memory_str(int64_t address, int64_t data);
void populate(int64_t max_dwords, char* file);

int main(){
	srand(SEED); // Set the rand() seed
	populate(20000, "mem.txt"); // Add 20,000 lines of random data to mem.txt
}

// Constructs the memory line string and adds it at the appropriate address in mem.txt
char* memory_str(int64_t address, int64_t data){
	char* line_str = malloc(30 * sizeof(char));
	// line_str: "0x"
	line_str[0] = '0';
	line_str[1] = 'x';
	line_str[29] = '\0';
	
	// line_str: "0x00000008"
	char addr_str[9];
	// take the address int64_t convert it to a hex string with leading up to 8 leading zeroes
	sprintf(addr_str, "%08lX", address);
	strcpy((char*) &(line_str[2]), addr_str);
	
	// line_str: "0x00000008: "
	line_str[10] = ':';
	line_str[11] = ' ';
	
	char upper_str[9];
	int32_t upper_word = data >> 32; //data shifted right 32 bits
	
	char lower_str[9];
	int32_t lower_word = data & MAX_32bit; // AND with a mask of full 1s
	
	// line_str: "0x00000008: FFFFFFFF"
	sprintf(upper_str, "%08X", upper_word);
	strcpy(&line_str[12], upper_str);
	
	// line_str: "0x00000008: FFFFFFFF "
	line_str[20] = ' ';
	
	// line_str: "0x00000008: FFFFFFFF 88888888"
	sprintf(lower_str, "%08X", lower_word);
	strcpy(&line_str[21], lower_str);
	
	return line_str;
}


// Iteratively add random 64-bit ints until max_dwords double words are added
void populate(int64_t max_dwords, char* file){
	FILE* mem_file = fopen(file, "w");
	
	/* fopen() return NULL if unable to open file in given mode. */
   	 if (mem_file == NULL){
		/* Unable to open file hence exit */
		printf("\nUnable to open file.\n");
		printf("Please check whether file exists and you have read/write privilege.\n");
		return;
    	}
    	
    	for(int64_t i = 0; i < max_dwords; i++){
    		// creates 2 random 32-bit ints and concats them
    		int64_t data = (((int64_t)rand()) << 32) + ((int64_t) rand());
    		
    		char* mem_str = memory_str(i*8, data);
    		fputs(strcat(mem_str,"\n"), mem_file);
    	}
    	
    	fclose(mem_file);
}