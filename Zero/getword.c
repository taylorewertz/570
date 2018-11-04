/**
Taylor Ewertz
Carroll
CS570
09/05/18

This program is a lexical analyzer that takes input from standard input, with ; and $ being special characters.
**/


#include <stdio.h>
#include "getword.h"

int getword( char *w){

	int iochar, x;
	int wordcount = 0;
	int dsign = 0;	
	
	iochar = getchar();

	//ignores white space before first character.

	while(iochar == ' ')
		iochar = getchar();

	//when getchar hits a $ in the front of the word i call getchar again to skip to the next character in the array and place a null there. Also increases a dsign flag to one so the program will be able to change to negative word lengths
	//if the character after a $ is a ; a newline or an EOF i put the $ back onto stdin and it returns a wordlength of zero
	
	if(iochar == '$'){
		dsign = 1;
		x = getchar();
		if(x == ';' || x == EOF || x == '\n'){
			ungetc(x,stdin);
			*w = '\0';
			return 0;
		}
		ungetc(x,stdin);
		iochar = getchar();
		*w = '\0';
	}
	// when getchar encounters a newline it places a null chaacter and returns a wordlength of 0

	if(iochar == '\n'){
		*w = '\0';
		return 0;
	}

	//when getchar gets an EOF it places a null in the pointer of w and returns a word length of -255

	if(iochar == EOF){
		*w = '\0';
		return -255;
	}

	//when getchar gets a ; if the first character in a word a null character is placed in the array and the next character and returns a length of 0
		
	if(iochar == ';'){
		*w = '\0';
		w++;
		*w = '\0';
		return -0;
	}

	// processes special characters stored in the array.


	while(iochar != EOF){
	
		if(iochar == ' '){
			if(dsign == 0){
				*w = '\0';
				return wordcount;
			}
			else{
				*w = '\0';
				return -wordcount;
			}
		}
		if(iochar == '\n'){
			if(dsign == 0){
				iochar = ungetc(iochar,stdin);
				*w ='\0';
				return wordcount;
			}
			else{
				iochar = ungetc(iochar,stdin);
				*w ='\0';
				return -wordcount;
			}
		}
		if(iochar == ';'){
			if(dsign == 0){
				iochar = ungetc(iochar,stdin);
				*w = '\0';
				return wordcount;
			}
			else{
				*w = '\0';
				return wordcount;
			}
		}
		*w = iochar;
		w++;
		wordcount++;
		iochar = getchar();
		*w = '\0';
	}
	if(dsign == 0)
		return wordcount;
	else
		return -wordcount;
}
