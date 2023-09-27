
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "interpreter.h"
#include "shellmemory.h"

int execute_line(char* ptr);
int MAX_USER_INPUT = 1000;
int parseInput(char ui[]);

// Start of everything
int main(int argc, char *argv[]) {
	if(system("mkdir store") != 0) {
		system("rm -rf store/*");
	}
	init_ready_queue();
	printf("%s\n", "Shell version 1.1 Created January 2022");
	int x = FRAME;
	int y = VAR;
	
	help();
	printf("Frame Store Size = %d, Variable Store Size = %d\n",x,y);
	char prompt = '$';  				// Shell prompt
	char userInput[MAX_USER_INPUT];		// user's input stored here
	int previousWasOEF = 0;

	//init user input
	for (int i=0; i<MAX_USER_INPUT; i++)
		userInput[i] = '\0';
	
	//init shell memory
	mem_init();

	while(1) {		
							
		if(!previousWasOEF) printf("%c ",prompt);
		//this variable simply exists to avoid printing the prompt twice when switching from batch to user input
		previousWasOEF = 0;

		//if we reach end of input file, redirect stdin
		if(!fgets(userInput, MAX_USER_INPUT-1, stdin)) {
			previousWasOEF = 1;
			FILE *tty;

			tty = freopen("/dev/tty", "r", stdin);
			if (tty == NULL) {
				perror("Unable to open terminal for reading");
				exit(1);
			}
			continue;

		}

		char *ptr = userInput;
		execute_line(ptr);
		memset(userInput, 0, sizeof(userInput));

	}
	
	return 0;

}

int execute_line(char* ptr) {
	int errorCode = 0;					// zero means no error, default
	char command[MAX_USER_INPUT];

	//init command
	for (int i=0; i<MAX_USER_INPUT; i++)
		command[i] = '\0';

	int index = 0;
	while(1) {
		if(*ptr == '\0' || *ptr == ';') {
			errorCode = parseInput(command);
			if (errorCode == -1) exit(99);	// ignore all other errors
			//init next command
			for (int i=0; i<MAX_USER_INPUT; i++)
				command[i] = '\0';
			index = 0;
			
			if(*ptr == '\0') {
				break;
			}
			else{
				ptr++;
			}
		}
		else{ 
			command[index] = *ptr;
			ptr++;
			index++;
		}

	}
	return errorCode;
}

// Extract words from the input then call interpreter
int parseInput(char ui[]) {
 
	char tmp[200];
	char *words[100];							
	int a,b;							
	int w=0; // wordID

	for(a=0; ui[a]==' ' && a<1000; a++);		// skip white spaces

	while(ui[a] != '\0' && a<1000) {

		for(b=0; ui[a]!='\0' && ui[a]!=' ' && a<1000; a++, b++)
			tmp[b] = ui[a];						// extract a word
	 
		tmp[b] = '\0';

		words[w] = strdup(tmp);

		w++;
		
		if(ui[a] == '\0'){
			break;
		}
		a++; 
	}

	return interpreter(words, w);
}
