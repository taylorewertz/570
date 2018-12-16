/**
Taylor Ewertz
Carroll
CS570
09/17/18

This program is a extension of program zero, adding more special metacharacters that react differently getchar() read them in.
When the $ metacharacter is read as the first character in a word it is not printed out and the rest of the word is counted as negative number.
The ~ metacharacter is read then the ~ character itself is not printed out, instead there is the home directory in its place. The |, <, > are now characters that react specially when read in.
The < character when followed by another < is printed out as <<. The \ character is skipped when it is in the beginning of a word. Now when the word being read is 255 characters long that string is split into a different word.
UPDATE: the $ and & conditions were changed to fit the p2 specs.
**/


#include <stdio.h>
#include <stdlib.h>
#include "getword.h"
#include "p2.h"

int DSFLAG;
int TFLAG;
int CFLAG;

int getword( char *w){

	int iochar, x;
	long int i;
	int wordcount = 0;
	int dsign = 0;
	char *home = getenv("HOME");
	size_t len = strlen(home);
	char arr[len];
	strcpy(arr,home);
	DSFLAG = 0;
	TFLAG = 0;


	iochar = getchar();

	//ignores white space before first character.

	while(iochar == ' ')
		iochar = getchar();

	//when getchar hits a $ in the front of the word i call getchar again to skip to the next character in the array and place a null there. Also increases a dsign flag to one so the program will be able to change to negative word lengths
	//if the character after a $ is a ; a newline or an EOF i put the $ back onto stdin and it returns a wordlength of zero.
	// this now checks if this next character is a ~ and a ~ is printed in the buffer.
	//UPDATE just skips the leading $
	//UPDATE 11/28/18: multiple flags set to make sure different things and the $ is not in the word	
	

	if(iochar == '$'){
		
		CFLAG = 1;
		DSFLAG = 1;
		dsign = 1;
		x = getchar();
		if(x == ';' || x == EOF || x == '\n'){
			ungetc(x,stdin);
			*w = '\0';
			return 0;
		}
		else if(x == '~'){
			*w = x;
			w++;
			*w = '\0';
			return -1;
		}
		x = ungetc(x,stdin);
		iochar = getchar();
		

	}
	// when getchar encounters a newline it places a null chaacter and returns a wordlength of 0

	if(iochar == '\n' || iochar == ';'){
		*w = '\0';
		return 0;
	}

	//when getchar gets an EOF it places a null in the pointer of w and returns a word length of -255

	if(iochar == EOF){
		*w = '\0';
		return -255;
	}

	// when iochar hits a ~ character, a flag is set and the character it self is skipped so its not in the word.

	if(iochar == '~'){


		TFLAG = 1;
		iochar = getchar();

		/**
		for(i = 0; i < sizeof(arr); i++){
						
			iochar = arr[i];
			*w = iochar;
			wordcount++;
			w++;
		}
		iochar = getchar();
		**/
	}	
	//when a > or a | character is hit as the first character then it is placed in the buffer and in next spot a null character is placed so the output is correct with a word count of 1

	if(iochar == '>' || iochar == '|'){
		*w = iochar;
		w++;
		*w = '\0';
		return 1;
	}

	// checks if the character after an & is a newline if so a flag is set to prove that the program should run something in the background, if not put the thing back on the stream then put then go normally if the & is just a special character.
	if(iochar == '&'){

		x = getchar();

		if(x == '\n'){
			BGFLAG++;
		}
		x = ungetc(x, stdin);

		*w = iochar;
		w++;
		*w = '\0';
		return 1;
	}

	// when a < character is hit getchar is called again to check the next chacracter to see if it another < if it is then << is printed and if not just the < is printed.

	if(iochar == '<'){

		x = getchar();
		if(x == '<'){
			HIFLAG++;
			*w = iochar;
			w++;
			*w = x;
			w++;
			*w = '\0';
			return 2;
		}
		x = ungetc(x,stdin);

		*w = iochar;
		w++;
		*w = '\0';
		return 1;
	}
	// processes special characters stored in the array when hit in the middle of the word. when certain characters are hit i use ungetc to put it back on the stream so it can be read again on the next pass.


	while(iochar != EOF){
	
		if(iochar == ' ' || iochar == EOF){
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
			iochar = ungetc(iochar,stdin);
			*w = '\0';
			return wordcount;
		}		
		if(iochar == '<' || iochar == '>' ||iochar == '|'){
			iochar = ungetc(iochar,stdin);
			*w = '\0';
			return wordcount;
		}
		if(iochar == '\\'){
			iochar = getchar();
		}
		if(wordcount == STORAGE - 1){
			iochar = ungetc(iochar, stdin);
			*w = '\0';
			return wordcount;
		}
		*w = iochar;
		w++;
		wordcount++;
		iochar = getchar();
	}
	if(dsign == 0)
		return wordcount;
	else
		return -wordcount;
}
