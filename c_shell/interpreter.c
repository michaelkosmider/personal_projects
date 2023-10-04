#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

struct pcb {

	int pid;
	int pc[2];
	int page_table[1000];
	int current_page;
	FILE * file;
	int file_is_loaded;
	int is_complete;
	char file_name[100];

};
void update_oldest_page();
void age_pages(struct pcb * cur_pcb);
int load_page(struct pcb * cur_pcb);
void init_pc();
void print_pcb_info(struct pcb * cur_pcb);
void init_page_loader();
int rr();
int scheduler();
int echo();
int help();
int quit();
int badcommand();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();

//variables:
//ready queue has size 3 because there will be at most 3 concurrent processes.
int ready_queue_head = 0;
struct pcb* ready_queue[3];
struct pcb* pcbholder[3];
int unique_pid = 0;
int exec_is_running = 0;
int MAX_ARGS_SIZE = 50;
int oldest_page[2] = {0,0};


//function implementations:

void init_ready_queue() {
	for(int i = 0; i < 3; i++) {
		ready_queue[i] = NULL;
	}
}

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		if (args_size < 3) return badcommand();
		if (args_size > 7) {
			printf("Bad command: Too many tokens\n");
			return 1;
		}	
		char buffer[1000];
		strcpy(buffer,command_args[2]);
		for(int i = 3; i < args_size; i++){
			strcat(buffer," ");
			strcat(buffer,command_args[i]);
		}
		return set(command_args[1], buffer);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "echo")==0) {
		if (args_size != 2) return badcommand();
		return echo(command_args[1]);

	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size != 1) return badcommand();
		return system("ls -1");

	} else if (strcmp(command_args[0], "fmemory")==0) {
		print_framememory(atoi(command_args[1]));
		return 0;

	} else if (strcmp(command_args[0], "resetmem")==0) {
		resetmem();
		return 0;

	} else if (strcmp(command_args[0], "exec")==0) {
		if (args_size == 1) return badcommand();
		if (strcmp(command_args[args_size-1], "RR")!=0) {
			return badcommand();
		}

		exec_is_running = 1;

		//run generates pcbs and adds them into queue
		for(int i = 1; i < args_size-1; i++) {
			int x = run(command_args[i]);
		}

		exec_is_running = 0;
		return scheduler(command_args[args_size-1]);

	}else return badcommand();
}

int echo(char* value){
	if(*value != '$'){
		printf("%s\n",value);
	}
	else {
		print(value+1);
	}
	return 0;
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n \
echo <text>		Prints input to screen\n \
exec <prog1> <prog2> <prog3> RR	Executes multiple scripts\n";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	system("rm -rf store/*");
	system("rm -rf store");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int set(char* var, char* value){

	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){

	int errCode = 0;
	FILE *p = fopen(script,"rt"); // the program is in a file
	if(p == NULL){
		return badcommandFileDoesNotExist();
	}
    fclose(p);

	//check if readyqueue is full
	int i = 0;
	while(i < 3 && ready_queue[i] != NULL) i++;
	if(i == 3) {
		printf("Ready queue is full\n");
		return errCode;
	}
	
	char command_cp[100];
	char command_test[100];
	char new_filename[100];

	strcpy(new_filename, "store/");
	strcat(new_filename, script);

	strcpy(command_cp, "cp ");
	strcat(command_cp, script);
	strcat(command_cp, " ");
	strcat(command_cp, new_filename);

	strcpy(command_test, "test -f ");
	strcat(command_test, new_filename);

	int x = system(command_test);

	while(x == 0) {
		strcat(command_cp, "i");
		strcat(command_test, "i");
		strcat(new_filename, "i");
		x = system(command_test);
	}
	system(command_cp);
	
	FILE *f = fopen(new_filename,"rt");
	if(p == NULL){
		printf("nah\n");
	}
	struct pcb* new_pcb = malloc(sizeof(struct pcb));
	unique_pid++;
	new_pcb->pid = unique_pid;
	new_pcb->pc[0] = -1;
	new_pcb->pc[1] = -1;
	new_pcb->file = f;
	
	for(int i = 0; i < 1000; i++) {
		new_pcb->page_table[i] = -1;
	}
	new_pcb->current_page = 0;
	new_pcb->file_is_loaded = 0;
	new_pcb->is_complete = 0;
	strcpy(new_pcb->file_name, new_filename);
	ready_queue[i] = new_pcb;

	if(exec_is_running == 0) {

		return scheduler("RR");
	}
	else{
			
		return 0;
	}
	return 0;
	
}

int scheduler(char* policy) {

	for(int i = 0; i < 3; i++) {
		pcbholder[i] = ready_queue[i];      
	}

	init_page_loader();
	init_pc();

	if(strcmp(policy, "RR")==0) {
		
		int ret = rr();
		fresetmem();
		return ret;
	}

	return 0;
	
}

void init_pc() {
	for(int i = 0; i < 3; i++) {

		if(ready_queue[i] == NULL) continue;

		struct pcb * cur_pcb = ready_queue[i];
		if(cur_pcb->page_table[0] == -1) {
			printf("Error: the first page of this program, %s, has not yet been loaded\n",cur_pcb->file_name);
			continue;
		}
		else {
			cur_pcb->pc[0] = 0;
			cur_pcb->pc[1] = 0;
		}

	}	
}

void init_page_loader() {

	for(int i = 0; i < 3; i++) {

		if(ready_queue[i] == NULL) continue;

		struct pcb * cur_pcb = ready_queue[i];

		for(int i = 0; i < 2; i++) {
			int x = load_page(cur_pcb);
			if(x == 1) break;
		}

	}
	
}

int load_page(struct pcb * cur_pcb) {

	FILE * p = cur_pcb->file;
	char line[1000];
	int flag = 0;

	//update the page table
	int frame = get_first_empty();
	if(frame == -1) {
		
		struct pcb * temp = pcbholder[oldest_page[0]];
		int index = temp->page_table[oldest_page[1]];
		temp->page_table[oldest_page[1]] = -1;

		printf("Page fault! Victim page contents:\n");
		for(int i = 0; i < 3; i++) {
			printf("%s",mem_get_value_by_index(index+i));
		}
		printf("End of victim page contents.\n");

		remove_page(index);
		frame = get_first_empty();

		update_oldest_page();

	}

	cur_pcb->page_table[cur_pcb->current_page] = frame;
	cur_pcb->current_page = cur_pcb->current_page + 1;

	//put next 3 instructions into frame memory

	for(int i = 0; i < 3; i++) {
	fgets(line,999,p);
	set("instruction",line);
	if(feof(p)){
		for(int j = i + 1; j < 3; j++) {
			set("instruction","empty");
		}
		flag = 1;
		break;
	}
	memset(line, 0, sizeof(line));
	}	
	
	if(flag == 1) {
		cur_pcb->file_is_loaded = 1;
	}

	return flag;

}

void update_oldest_page() {
	for(int i = 0; i < 3; i++) {
		if(pcbholder[(oldest_page[0]+1+i)%3] == NULL) continue;
		oldest_page[0] = (oldest_page[0]+1+i)%3;
		while(pcbholder[oldest_page[0]]->page_table[oldest_page[1]] == -1) oldest_page[1]++;
		break;
	}
}

int rr() {

	int errCode = 0;
	ready_queue_head = 0;
	char* instruction = NULL;
	struct pcb * cur_pcb;
	int nullcount = 0;

	while(1) {
		if(nullcount == 3) {
			return 0;
		}
		if(ready_queue[ready_queue_head] == NULL) {
			nullcount++;
			ready_queue_head = (ready_queue_head + 1)%3;
		}
		else{
			nullcount = 0;
			cur_pcb = ready_queue[ready_queue_head];

			for(int i = 0; i < 2; i++) {
				if(cur_pcb->page_table[cur_pcb->pc[0]] == -1) {
					if(cur_pcb->file_is_loaded == 1) {
						cur_pcb->is_complete = 1;
					}
					else {
						load_page(cur_pcb);
						break;
					}
				}
				if(cur_pcb->is_complete == 0) {
					instruction = mem_get_value_by_index(cur_pcb->page_table[cur_pcb->pc[0]]+cur_pcb->pc[1]);
				}

				if(strcmp(instruction, "empty") == 0 || cur_pcb->is_complete == 1) {

					ready_queue[ready_queue_head] = NULL;
					fclose(cur_pcb->file);
					break;
					
				}

				errCode = execute_line(instruction);
				free(instruction);

				cur_pcb->pc[1]++;
				if(cur_pcb->pc[1] > 2) {

					cur_pcb->pc[1] = 0;
					cur_pcb->pc[0] = cur_pcb->pc[0] + 1;

				}
				
			}

			ready_queue_head = (ready_queue_head + 1)%3;

		}
	}

	return errCode;

}

