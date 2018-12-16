/**
Taylor Ewertz
CS570
Carroll
11/28/2018

This program is a simplified linux shell, that handles input and output redirection, it handles up to ten pipes, a mock cd command, a mock hereis command when a << is seen. Also it handles a command named "environ" that prints and changes environ variable. If a $ before a valid enivronment variable it prints the value of the enivroment variable. the ~ character allows this shell to look up the the word behind in the /etc/passwd file. The cd command changed the directory and if it did it changed the prompt. Also & as the last character in the command sends it to the background.
**/

#include "p2.h"

char argv[FULLSTORAGE];
char *arr[MAXITEM];
int charcount;
int pipearray[MAXPIPES];
int dsarray[MAXPIPES];
int PIPEFLAG = 0, INFLAG = 0, OUTFLAG = 0, BGFLAG = 0, ERFLAG = 0, ENVFLAG = 0, HIFLAG;
extern int DSFLAG, TFLAG, CFLAG;
int dev, pipecounter, in, out, temp;
char *IN, *OUT, *DS, *DELIM, *ENV;
FILE *TEMP;
int main(){

	int wrdctr;
	pid_t pid, childpid, gchildpid;
	char cwd[PATH_MAX];
	int CDFLAG;
	char *homedir, *dir, *base;
	setpgid(0,0);
	(void)signal(SIGTERM, sighandler);

	for(;;){


		/* if the cd command had been used print the basename of the directory before the prompt */
	
		if(CDFLAG == 1)
			printf("%s", base);

		printf(":570: ");

		wrdctr = parse();

		/* if the error flag is 1 it sets the check of the $ to 0 so environ will work correctly  or if the ~ check cant find a match go to next iteration*/ 

		if(ERFLAG == 1){
			CFLAG = 0;
			continue;
		}

		/* if the first word is an EOF, breaks out of the loop */

		if(charcount == -255)
			break;
		/*if no arguments are entered, program reissues the prompt */
		
		if(wrdctr == 0)
			continue;
	
		/* error if < is the first word in arr */		

		if(arr[0] == NULL){
			fprintf(stderr, "Invalid NULL command\n");
			continue;
		} 
	
		/* checks if there is too many pipes or there is a null in the pipe array where there should be then goes into the pipecode function */
			
		if(PIPEFLAG != 0){
			int e;

                        if(PIPEFLAG > 10){
                                perror("Too many pipes!\n");
                                continue;
                        }
				
                        for(pipecounter = 0; pipecounter < PIPEFLAG; pipecounter++){
                                if(arr[pipearray[pipecounter]] == NULL){
                                        fprintf(stderr, "PIPE Invalid NULL\n");
					e = 1;
                                }
                        }
			if(e == 1)
				continue;
			
                        pipecode();
                        continue;
			
                }


		/* '<' input redirection */

		if(INFLAG != 0){
		
			if(INFLAG > 2){
                                fprintf(stderr,"Ambiguous Inuput Redirect\n");
                                continue;
                        }
                        if(IN == NULL){
                                fprintf(stderr, "Missing name for redirection\n");
                                continue;
                        }

			if((in = open(IN, O_RDONLY)) == -1){
				fprintf(stderr,"Cannot Open %s\n", IN);
				continue;
			}
		}
		
		/* '>' output redirection */

		if(OUTFLAG != 0){

			int flag = O_RDWR | O_CREAT | O_EXCL;
			

			if(OUTFLAG > 2){
                                fprintf(stderr, "Ambiguous Output Redirect\n");
                                continue;
                        }
                        if(OUT == NULL){
                                fprintf(stderr, "Missing name for redirection\n");
                                continue;
                        }

			if((out = open(OUT, flag, S_IRUSR | S_IWUSR)) == -1){
				perror(OUT);
				continue;
			}
		
		}


		/*  HEREIS */

                if(HIFLAG != 0){

                        char *line;
                        size_t length = 0;
                        char *delim;

			/* if the delimiter space is null it fails out if it isnt then you append the newline to make sure the delimiter will match when reading */

                        if(DELIM == NULL){
                                fprintf(stderr, "NULL delimiter\n");
                                continue;
                        }
                      	else{
                                delim = DELIM;
                                strcat(delim,"\n");
			}

			/* open a file, read each line from stdin, check if the line matches the delimiter, if it doesnt write to the file, if it does break out of the loop */
		
			if((TEMP = fopen("temp", "w")) == NULL){
                                fprintf(stderr, "Cannot open %s\n", TEMP);
                     		return;
                        }
                        while(getline(&line, &length, stdin) != EOF){

                                if(strcmp(line, delim) != 0)
                                        fputs(line, TEMP);
                                else
                                        break;
                        }
                        fclose(TEMP);
			
                        
			if((temp = open("temp", O_RDONLY)) < 0){
                        	fprintf(stderr, "Cant open %s\n", "temp");
                       		continue;
                        }
                           
                }
	
		/* ENVIRON */
		
		if(strcmp(arr[0], "environ") == 0){

			char *env, *path, *path2;

			/* if only one argument is passed to environ, then it prints out that environment variable. if two arguments are passed then the second argument is set as the value of the first argument. */
						
			if(arr[1] != NULL && arr[2] == NULL){

		      	/* checks to see if the environment variable exist and prints it  if it sees a $ it will print out the already full environment path or value*/

				if(CFLAG == 1){
					printf("%s\n", arr[1]);
				}
		      		else if((env = getenv(arr[1])) == NULL){	
					continue;
				}
		     	 	else if((env = getenv(arr[1])) != NULL){
					printf("%s\n", env);
				}
				
			}	
		    	else if(arr[1] != NULL && arr[2] != NULL){
		      
		      		env = arr[1];

		      	/* checks to see if the file or directory or environment variable exists and sets env to path */

			      	path = arr[2];
				path2 = arr[2];

				if((path = getenv(path)) != NULL){
					if(setenv(env, path,1) == -1){
						perror("could not set environment\n");
					}
				}
			      	else if(access(path2, F_OK) == 0){
					if(setenv(env, path2, 1) == -1)
				  		fprintf(stderr, "Could Not Set Environment\n");
		      		}		
		      		else{
					fprintf(stderr, "%s: Path or environment variable does not exist\n", arr[2]);
					continue;
		      		}
				
		    
			}
		 	else if(arr[3] != NULL){
		      		perror("Too Many Arguments\n");
		    	}
		    	continue;
		}


		/* CD code - allows cd with no other arguments perform like a normal shell by going to home directory then makes sure the basename of the directory it changed to in front of the :570: prompt */

		if(strcmp(arr[0], "cd") == 0){

			if(wrdctr > 2)
				fprintf(stderr, "Too Many Arguments!\n");
			else if (wrdctr == 1){
				if((homedir = getenv("HOME")) == NULL)
					perror("Cannot find home directory!\n");
				else{
					CDFLAG = 1;
					chdir(homedir);
					dir = getcwd(cwd, sizeof(cwd));
					base = basename(dir);
				}	
			
			}
			else if(wrdctr == 2){
				CDFLAG = 1;
				chdir(arr[1]);
				dir = getcwd(cwd, sizeof(cwd));
				base = basename(dir);
				
			}
			continue;
		}
		
		fflush(stdout);
		fflush(stderr);
		CHK(childpid = fork());
		if(childpid == 0){

			if(BGFLAG != 0 && INFLAG == 0){
				
				if((dev = open("/dev/null", O_RDONLY)) < 0){
					perror("Failed to open /dev/null\n");
					exit(1);
				}
				
				if((dup2(dev, STDIN_FILENO)) < 0){
					perror("/dev/null dup2\n");
					exit(1);
				}
				if((close(dev)) < 0){
					perror("cant close dev file descriptor\n");
					exit(1);
				}
			}
			if(HIFLAG != 0){

				if((dup2(temp, STDIN_FILENO)) < 0){
					perror("Failed to dup hereis temp file\n");
					exit(1);
				}
				if((close(temp)) < 0){
					perror("fail to close hereis temp file\n");
					exit(1);
				}
				if((remove("temp")) < 0){
					perror("failed to remove hereis temp file\n");
					exit(1);
				}
			}
			
			if(INFLAG != 0){
                                if((dup2(in,STDIN_FILENO)) < 0){
					fprintf(stderr, "Cant dup %s\n", in);
					exit(1);
				}
                                if((close(in)) < 0){
					perror("failed to close in file\n");
					exit(1);
				}
                        }

			if(OUTFLAG != 0){
				if((dup2(out,STDOUT_FILENO)) < 0){
                                        fprintf(stderr, "Cant dup %s\n", out);
                                        exit(1);
                                }
                                if((close(out)) < 0){
                                        perror("failed to close out file\n");
					exit(1);
                                }

			}
			if(execvp(arr[0] , arr) < 0){
				perror("Command not Found\n");
				exit(9);
			}
		}
		else{

			if(BGFLAG != 0){
				printf("%s [%d]\n", *arr, childpid);
				BGFLAG = 0;
				continue;
			}
			else{
				for(;;){
					pid = wait(NULL);
					if(pid == childpid)
						break;
				}	
			}
		}
	}
	killpg(getpgrp(), SIGTERM);
	printf("p2 terminated.\n");
	exit(0);

}
int parse(){

	int x = 0;
	int word = 0;
	int location = 0;
        char *INPUT;
	char *p;
        char *PATH;
	char *last;
        char *check;
        char *FINAL;
	char *slash;
        FILE *fp;
        size_t len =0;
	int scheck = 0;
	IN = NULL;
	OUT = NULL;
	DELIM = NULL;
	PIPEFLAG = 0;
	INFLAG = 0;
	OUTFLAG = 0;
	BGFLAG = 0;
	HIFLAG = 0;
	ERFLAG = 0;

	for(;;){

		charcount = getword(argv + location);

		if(BGFLAG != 0)
			break;
		if(charcount == 0)
			break;
		else if(charcount == -255)
			break;
		if(TFLAG != 0){

			/* if a ~ appears in the stream open the /etc/passwd file check if the input has a / in it if it does append the "/EXAMPLE" on the end. then check if the input is within the file and strtok until the directory is the output then put it into the buffer, if / is not in the input do the same thing without appending. */  
					
			if((fp = fopen("/etc/passwd", "r")) == NULL){
				fprintf(stderr, "Cannot open file: %s\n", fp);
				break;
			}
			else{

				INPUT = argv + location;
		
				p = strstr(INPUT, "/");
				if(p){
	
					slash = strtok(INPUT, "/");
					slash = strtok(NULL, "/");

					while((getline(&PATH, &len, fp)) != EOF){


                                                if((check = strstr(PATH, INPUT)) != NULL){

                                                        FINAL = strtok(check, ":");
                                                        FINAL = strtok(NULL, ":");
                                                        FINAL = strtok(NULL, ":");
                                                        FINAL = strtok(NULL, ":");
                                                        FINAL = strtok(NULL, ":");
                                                        FINAL = strtok(NULL, ":");
							strcat(FINAL, "/");
							strcat(FINAL, slash);
                                                        break;
                                                }

                                        }

				}
				else{

	     				while((getline(&PATH, &len, fp)) != EOF){

						if((check = strstr(PATH, INPUT)) != NULL){
					
							FINAL = strtok(check, ":");
							FINAL = strtok(NULL, ":");
							FINAL = strtok(NULL, ":");
							FINAL = strtok(NULL, ":");
                                        	        FINAL = strtok(NULL, ":");
							FINAL = strtok(NULL, ":");		
							break;
						}
					
					}
				}
				if(FINAL == NULL){
					fprintf(stderr, "No Match: ~%s\n", (argv +location));
					ERFLAG = 1;
				}
				else{
					arr[x++] = FINAL;
				}
			}
		}
		if(DSFLAG != 0){

			/*  if the $ is seen  it checks if that location is a environment variable, if it is if the redirect flag are set make the IN and OUT that value and then put the new the value which is now an environment variable value into the stream. */ 

                     	if((DS = getenv(argv + location)) == NULL){
                        	fprintf(stderr,"%s: Undefined variable\n", (argv+location));
                                	ERFLAG = 1;
                        }
                        else{
                        	if(INFLAG == 1)
                                	IN = DS;
                                else if(OUTFLAG == 1)
                                	OUT = DS;
                                else
                               		arr[x++] = DS;	
                	}
                }
		else if(*(argv + location) == '<' && charcount == 1 || INFLAG == 1){

			/* saves the namea  of the input file */			
				
			INFLAG++;
			if(INFLAG == 2){
				IN = argv + location;
			}

		}
		else if(*(argv + location) == '>' && charcount == 1 || OUTFLAG == 1){

			/* saves the name of the output file */
		
			OUTFLAG++;
			if(OUTFLAG == 2)
				OUT = argv + location;
		}
		else if(*(argv + location) == '|' && charcount == 1){
			
			/* saves the locations of the pipes into an array so the execvps will know where to execute */

			PIPEFLAG++;
			arr[x] = NULL;			
			x++;
			if(HIFLAG != 0){
				x++;
				x++;
				pipearray[PIPEFLAG - 1] = x;
			}
			else
				pipearray[PIPEFLAG - 1] = x;
		}
		else if(HIFLAG != 0){

			/* saves the value for the use of an delimiter */

			if(HIFLAG == 2){
				
				DELIM = argv + location;
			}
			HIFLAG++;
		}
		else{
			if(TFLAG == 0)
				arr[x++] = argv + location;
		}
		location += abs(charcount) + 1;
		word++;
	
	}
	arr[x] = NULL;
	return word;

}
void pipecode(){

	int fd[PIPEFLAG * 2];
	pid_t middle, third;
	int p, i, c, z;
	
	if(INFLAG != 0){
		if(INFLAG > 2){
                	fprintf(stderr, "Ambiguous Input Redirect\n");
                	return;
                }
                if(IN == NULL){
                	perror("Missing name for redirection\n");
                       	return;
                }
		if((in = open(IN,O_RDONLY)) == -1){
			perror(IN);
			return;
		}
        }
	if(OUTFLAG != 0){
		
		int flag = O_RDWR | O_CREAT | O_EXCL;

                if(OUTFLAG > 2){
                	fprintf(stderr,"Ambiguous Output Redirect\n");
                      	return;
                }
                if(OUT == NULL){
                	perror("Missing name\n");
                       	return;
              	}
		if((out = open(OUT, flag, S_IRUSR | S_IWUSR)) == -1){
			perror(OUT);
			return;
		}
	}
	if(HIFLAG != 0){

       		char *line;
                size_t length = 0;
                FILE *TEMP;
                char *delim = DELIM;
                strcat(delim,"\n");

		if(delim == NULL){
			fprintf(stderr, "NULL Delimiter\n");
			return;
		}
		
                if((TEMP = fopen("temp", "w")) == NULL){
                	fprintf(stderr, "Cannot open %s\n", TEMP);
                        return;
                }
                while(getline(&line, &length, stdin) != EOF){
			printf("%s\n", delim);

                	if(strcmp(line, delim) != 0)
                        	fputs(line, TEMP);
                       	else
                        	break;
                }
                fclose(TEMP);
		
		if((temp = open("temp", O_RDONLY)) == -1){
                	fprintf(stderr,"Cannot Open %s\n", "temp");
                	return;
                }

        }

	fflush(stdout);
	fflush(stderr);

	CHK(third = fork());
	if(third == 0){

		/* pipe to ensure vertical piping */

		for(p = 0; p < PIPEFLAG; p++){
			CHK(pipe(&fd[2*p]));
		}

		/* redirect temp file into stdin and removes the file */

		if(HIFLAG != 0){
		
                        CHK(dup2(temp, STDIN_FILENO));
                        CHK(close(temp));
                        CHK(remove("temp"));
                }

		/* redirect input file to stdin */	

		if(INFLAG != 0){
                        CHK(dup2(in, STDIN_FILENO));
                        CHK(close(in));
                }

		/* redirects the output file to stdout */

		if(OUTFLAG != 0){
			CHK(dup2(out, STDOUT_FILENO));
			CHK(close(out));
		}
	
		/* forks the children the amount of pipes there is */

		for(i = 0; i < PIPEFLAG; i++){

			fflush(stdout);
			fflush(stderr);
			CHK(middle = fork());
			if(middle == 0){

				if(PIPEFLAG == 1){
				
					// ONE

					CHK(dup2(fd[1], STDOUT_FILENO));
					CHK(close(fd[0]));
					CHK(close(fd[1]));
					if(execvp(arr[0], arr) < 0){
						perror("Command Not Found\n");
						exit(9);
					}
				}


			}
			else{

				if(PIPEFLAG != 1){

					/* redirect pipe read end to the stdin*/
				
					if(i != PIPEFLAG - 1){;
						CHK(dup2(fd[(i*2) +2], STDIN_FILENO));
					}

					/* redirect write to STDOUT */

					CHK(dup2(fd[(i*2) + 1], STDOUT_FILENO));

					/* close file descirptors */

					for(c = 0; c < PIPEFLAG * 2; c++){
						CHK(close(fd[c]));
					}

					/* exec all commands */

					if(i == PIPEFLAG - 1){

						if(execvp(arr[0], arr) < 0){
							perror("Command Not Found\n");
							exit(9);
						}
					}
					else{
						if(execvp(arr[pipearray[(PIPEFLAG -2) - i]], arr + pipearray[(PIPEFLAG - 2) - i]) < 0){
							perror("Command Not Found\n");
							exit(9);
						}
					}

				}
			
			}
		}

		/* STDIN to read */	
		CHK(dup2(fd[0], STDIN_FILENO));
		for(z = 0; z < PIPEFLAG *2; z++){
			CHK(close(fd[z]));
		}
		if(execvp(arr[pipearray[PIPEFLAG - 1]], arr + pipearray[PIPEFLAG - 1]) < 0){
			perror("Command Not Found\n");
			exit(9);
		}
		for(;;){
			pid_t pid;
			pid = wait(NULL);
			if(pid == third)
				break;
		}
		
	}
	/* wait for all processes to complete */
	for(;;){
        	pid_t pid;
                pid = wait(NULL);
                if(pid == third)
                	break;
     	}
}
void sighandler(){
}
