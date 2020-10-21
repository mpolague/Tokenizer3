#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_32bit 4294967295; // 32 bits of all 1s

// Read the 16 hex digit string and return as a 64-bit int
int64_t read_address(int64_t address, char* file_name){
	int64_t line_address = address / 8;
	int64_t return_data;
	FILE* mem_file;
	char* str = malloc(50 * sizeof(char));
	char hex_str[17]; // 16 hex-digit string
	str[29] = '\0';
	hex_str[16] = '\0';
 
 	// open file with relative address
	mem_file = fopen(file_name, "r");
	if (mem_file == NULL){
		printf("Could not open file %s\n",file_name);
		return 1;
	}
	int64_t line = 0;
	bool done_searching = false;
	do{
		// For string: "0x00000000: 786F2EAB 53FB439A"
		// grab only "786F2EAB53FB439A" skipping over everything else
		char* line_str = fgets(str, 50, mem_file);
		for(int i = 0; i < 16; i++){
			if(i < 8)
				hex_str[i] = str[12+i];
			else // if it is the lower word, skip over the space
				hex_str[i] = str[13+i];
		}
		if(line >= line_address || line_str == NULL){
			done_searching = true;
			break;
		}
		line++;
	}while(!done_searching);
	
	// Convert "786F2EAB53FB439A" and turn into int64_t
	sscanf(hex_str, "%llX", (long long unsigned int*) &return_data);
	fclose(mem_file);
	return return_data;
}


// Write 64-bit int into mem file
int64_t write_address(int64_t data, int64_t address, char* file_name){
	int64_t line_address = address / 8;
	FILE* mem_file;
	FILE* temp_file;
	
	char* str = malloc(50 * sizeof(char));
	str[30] = '\n';
	str[30] = '\0';
 
 	// open file with relative address
	mem_file = fopen(file_name, "r");
	temp_file = fopen("replace.tmp", "w");
	
	/* fopen() return NULL if unable to open file in given mode. */
   	 if (mem_file == NULL || temp_file == NULL){
		/* Unable to open file hence exit */
		printf("\nUnable to open file.\n");
		printf("Please check whether file exists and you have read/write privilege.\n");
		return (int64_t) NULL;
    	}
    	
    	bool found_line = false;
	int64_t line = 0;
	while ((str=fgets(str, 50, mem_file)) != NULL){
		/* If current line is line to replace */
		if (line == line_address){
			char upper_str[9];
			int32_t upper_word = data >> 32; //data shifted right 32 bits
			
			
			char lower_str[9];
			int32_t lower_word = data & MAX_32bit; // AND with a mask of full 1s
			
			// line_str: "0x0000008: FFFFFFFF"
			sprintf(upper_str, "%08X", upper_word);
			strcpy(&str[12], upper_str);
			
			str[20] = ' ';
			
			// line_str: "0x00000008: FFFFFFFF 88888888"
			sprintf(lower_str, "%08X", lower_word);
			strcpy(&str[21], lower_str);
			str[29] = '\n';
			fputs(str, temp_file);
			found_line = true;
		}else{
		    	fputs(str, temp_file);
		}
		line++;
    	}
    	
    	// release resources back to OS
    	fclose(mem_file);
    	fclose(temp_file);
	
	rename("replace.tmp", file_name);
	
	if(found_line)
		return data;
	else
		return (int64_t) NULL;
}
