#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define VAR_MEMORY_SIZE VAR
#define FRAME_MEMORY_SIZE FRAME

struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct varshellmemory[VAR_MEMORY_SIZE];
struct memory_struct frameshellmemory[FRAME_MEMORY_SIZE];

//prints the first num_lines lines of memory
//for debugging purposes
void print_varmemory(int num_lines) {
	for(int i = 0; i < num_lines && i < VAR_MEMORY_SIZE; i++) {
		printf("var: %s, value: %s \n", varshellmemory[i].var, varshellmemory[i].value);
	}
}
void print_framememory(int num_lines) {
	for(int i = 0; i < num_lines && i < FRAME_MEMORY_SIZE; i++) {
		printf("var: %s, value: %s \n", frameshellmemory[i].var, frameshellmemory[i].value);
	}
}

void resetmem() {
	for (int i=0; i<VAR_MEMORY_SIZE; i++){		
		varshellmemory[i].var = "none";
		varshellmemory[i].value = "none";
	}
}
void fresetmem() {
	for (int i=0; i<FRAME_MEMORY_SIZE; i++){		
		frameshellmemory[i].var = "none";
		frameshellmemory[i].value = "none";
	}
}

void remove_page(int index) {
	for(int i = 0; i < 3; i++) {
		frameshellmemory[index+i].var = "none";
		frameshellmemory[index+i].value = "none";
	}
}

//finds and returns first empty space in frame memory. Returns -1 if memory full.
int get_first_empty() {
	int empty = 0;
	while(strcmp(frameshellmemory[empty].var, "none") != 0) {
		empty++;
		if(empty > FRAME_MEMORY_SIZE-1) return -1;
	}
	return empty;
}

// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}


// Shell memory functions

void mem_init(){

	for (int i=0; i<VAR_MEMORY_SIZE; i++){		
		varshellmemory[i].var = "none";
		varshellmemory[i].value = "none";
	}
	for (int i=0; i<FRAME_MEMORY_SIZE; i++){
		frameshellmemory[i].var = "none";
		frameshellmemory[i].value = "none";		
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	
	int i;

	if(strcmp("instruction", var_in) == 0) {
		for (i=0; i<FRAME_MEMORY_SIZE; i++){
			if (strcmp(frameshellmemory[i].var, "none") == 0){
				frameshellmemory[i].var = strdup(var_in);
				frameshellmemory[i].value = strdup(value_in);
				return;
			} 
		}
	}

	for (i=0; i<VAR_MEMORY_SIZE; i++){
		if (strcmp(varshellmemory[i].var, var_in) == 0){
			varshellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<VAR_MEMORY_SIZE; i++){
		if (strcmp(varshellmemory[i].var, "none") == 0){
			varshellmemory[i].var = strdup(var_in);
			varshellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<VAR_MEMORY_SIZE; i++){
		if (strcmp(varshellmemory[i].var, var_in) == 0){

			return strdup(varshellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

char* mem_get_value_by_index(int index) {
	return strdup(frameshellmemory[index].value);
}
