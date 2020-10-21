#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

int space_char(char c){
  if (c == '\n') //consider a new line as a space, newline at the end of fgets input
    return 1; 
  
  else if ( c == ' '|| c == '\t') //returns 1 if space or tab
    return 1;

  else
    return 0;
}

int non_space_char(char c){
  if (c == '\n')
    return 0;

  else if (c == ' ' || c == '\t')
    return 0;

  else
    return 1; //return 1 if not space,tab,or newline
}

char *word_start(char *str){
  char *temp = str;
  int i;

  for (i = 0; space_char(temp[i]);i++) //skips over any space before the start of a word
    ;
  
  if ((temp+i) == "\0"){ //if it reaches end without a char then it returns a NULL pointer
    temp = NULL;
    return temp;
  }
  
  return temp + i; //returns pointer at i since it iterated past the spaces
}

char *word_terminator(char *word){
  char *temp = word;
  int i;

  temp = word_start(word); //gets word start first

  for (i = 0; non_space_char(temp[i]);i++) //skips all non spaces
    ;

  return temp + i; //returns the pointer to space after a word
}

int count_words(char *str){
  int count = 0;
  int state = 0;

  while (*str){
    if (state == 0 && non_space_char(*str)){ //checks to see if its in a word and increments
      state = 1;
      ++count;
    }
    else if (state == 1 && space_char(*str)) //if not it doesnt increment count for word
      state = 0;
    *str++;
  }
  return count;
}

int len_word(char *str){
  char *start,*end;
  int len;

  start = word_start(str);
  end = word_terminator(str);

  len = end - start; //subtracts the addresses at word_start and word_terminator to get length

  return len;
}

char *copy_str(char *inStr, short len){
  char *copy = (char*) malloc((sizeof(char*))*(len+1)); //allocate space for a string 
  int i;

  for (i = 0; i < len; i++)
    copy[i] = inStr[i];  //copies the string given to the new copy being made

  copy[i] = '\0'; //set the last index to 0 to indicate the end of it

  return copy;
}

char **tokenize(char *str){
  int num_words = count_words(str);
  int i,len;
  char *temp = str;

  char **token = (char**) malloc((sizeof(char*))*(num_words+1)); //allocate space for token

  for (i = 0; i < num_words; i++){ //iterate through token up to num_words
    len = len_word(temp); 
    token[i] = copy_str(temp,len); //in each index, sets pointer to a copy of the string given
    temp = word_terminator(temp); //word_start and word_term give the start of a word each loop
    temp = word_start(temp);
  }
  token[i] = 0; //sets last index to 0 to know the end of token

  return token;
}

void print_tokens(char **tokens){
  for (int i = 0; tokens[i] != 0;i++){
    printf("token %d: ",i);
    printf("%s",tokens[i]); //print string in each index until it hits 0 in the last index
    printf("\n");
  }
}

void free_tokens(char **tokens){
  for (int i = 0; tokens[i] != 0;i++)
    free(tokens[i]); //frees individual pointers stored in token first
  free(tokens); //frees token itself
}

