/**
    Taylor Ewertz
    Carroll
    CS570
    09/17/18
    
This program is a extension of program zero, adding more special metacharac
ters that react differently getchar() read them in.
When the $ metacharacter is read as the first character in a word it is not
printed out and the rest of the word is counted as negative number.
The ~ metacharacter is read then the ~ character itself is not printed out,
instead there is the home directory in its place. The |, <, > are now character
s that react specially when read in.
The < character when followed by another < is printed out as <<. The \ char
acter is skipped when it is in the beginning of a word. Now when the word being 
read is 255 characters long that string is split into a different word.
  **/
  
  
  #include <stdio.h>
  #include <stdlib.h>
  #include "getword.h"
  
  int getword( char *w){
  
      int iochar, x, i;
      int wordcount = 0;
      int dsign = 0;
      char *home = getenv("HOME");
      size_t len = strlen(home);
      char arr[len];
      strcpy(arr,home);
      iochar = getchar();
  
      //ignores white space before first character.
  
      while(iochar == ' ')
          iochar = getchar();
  
      //when getchar hits a $ in the front of the word i call getchar again to
skip to the next character in the array and place a null there. Also increases 
a dsign flag to one so the program will be able to change to negative word lengt
hs
      //if the character after a $ is a ; a newline or an EOF i put the $ back
onto stdin and it returns a wordlength of zero.
     // this now checks if this next character is a ~ and a ~ is printed in t
he buffer.
      
      if(iochar == '$'){
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
  
      // when iochar hits a ~ character, the for loop will place each element of the array that has the home directory in it into iochar then into the buffer 
to get the correct output.
      // then when the full directory is there, the last element is printed twice so i called getchar again to skip that character and get the right output.
 
      if(iochar == '~'){
     
          for(i = 0; i < sizeof(arr); i++){
                          
              iochar = arr[i];
              *w = iochar;
              wordcount++;
              w++;
          }
          iochar = getchar();
      }    
  
      //when iochar hits a \ character a null is put in that spot to so it is skipped and doesnt show in the output.
      
      if(iochar == '\\'){
          *w = '\0';
      }  
      //when a > or a | character is hit as the first character then it is placed in the buffer and in next spot a null character is placed so the output is c
orrect with a word count of 1
  
      if(iochar == '>' || iochar == '|'){
          *w = iochar;
          w++;
          *w = '\0';
          return 1;
      }
  
      // when a < character is hit getchar is called again to check the next chacracter to see if it another < if it is then << is printed and if not just the
< is printed.
  
      if(iochar == '<'){
  
          x = getchar();
          if(x == '<'){
              *w = iochar;
              w++;
              *w = x;
              w++;
              *w = '\0';
              return 2;
          }
          iochar = ungetc(x,stdin);
          *w = '<';
          w++;
          *w = '\0';
          return 1;
      }
      // processes special characters stored in the array when hit in the middle of the word.
  
  
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
          if(iochar == '<' || iochar == '>'){
              iochar = ungetc(iochar,stdin);
              *w = '\0';
              return wordcount;
          }
          if(iochar == '\\'){
              iochar = getchar();
              if(iochar == '\n'){
                  *w = '\0';
                  return wordcount;
              }    
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
