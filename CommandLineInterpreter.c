#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Defining the buffer-size and history-count
#define SIZE 1024
#define COUNT 100

// Function-declarations
int parser(char **arguments);
void execute(char **arguments, char *completepath);
int showhistory(char *history[], int now);
int clearhistory(char *history[]);

// Global-variables
char input[SIZE];	

// Main Function; Execution starts here
int main() {   
	char completepath[20];       
      char* rootpath= "/bin/";      
      int argcount;                      
      char* arguments[100];         		
      char *history[COUNT];		
      int i, current, now = 0;			

    	for (current = 0; current < COUNT; current++) {
            history[current] = NULL;
		}

	while(1) {
	      printf("Personalized-Shell>");                        
		if(!fgets(input,SIZE,stdin)) {    
			break;                                 
		      }
		     
		size_t inputlength= strlen(input);    
		if(input[inputlength-1] == '\n') {
			input[inputlength-1]='\0';
		      }
		    
		free(history[now]);
		history[now] = strdup(input);
		now = (now + 1) % COUNT;   

			           	     
		if(strcmp(input, "exit")==0) {    
			break;
			}
		else if(strcmp(input, "list")==0) {    
			strcpy(input, "ls"); 
		      }
		else if(strcmp(input, "show")==0) {  
		      strcpy(input, "pwd"); 
		    	}
		else if(strcmp(input, "now")==0) {  
		      strcpy(input, "date"); 
		    	}

		argcount=parser(arguments);  		 			         
		strcpy(completepath, rootpath);   
		strcat(completepath, arguments[0]);            
		   
		for(i=0; i<strlen(completepath); i++) {    
			if(completepath[i]=='\n') {      
			    completepath[i]='\0';   
			}
		    }

		if (strcmp(input, "showhistory") == 0) {
			showhistory(history, now);
			}
		else if (strcmp(input, "clearhistory") == 0) {	                        
			clearhistory(history);
			}			
		else 	{
			execute(arguments,completepath);
		    	}

		} //endofwhile(1)

 	return 0;
	} 


// Function to parse the input read from the personalized-CLI 
int parser(char **arguments) {
	char *split;                       
	int n=0;

	split = strtok(input," ");	
	while(split!=NULL){
	  arguments[n]=split;      
	  split = strtok(NULL," ");
	  n++;
	}
	arguments[n]=NULL;                     
	return n;	
}


// Function which spawns child process to service the command entered in the parent process (fork() and execvp())
void execute(char **arguments, char *completepath) {
	int processid;			   
	processid= fork();			
            
	if(processid==0) {               
		execvp(completepath,arguments);
		fprintf(stderr, "You have entered an Invalid command! Please try again...\n");
		}	    
	else {                    
		wait(NULL); 
	}
}


// Function to display history of entered commands
int showhistory(char *history[], int now) {
        int current = now-1;
        printf("...HISTORY...\n");
        while(history[current] != NULL && current>=0) {
            printf("%s\n",history[current]);
            current--;
        }
        return 0;
}

// Function to clear the history of entered commands
int clearhistory(char *history[]) {
        int current;
        for (current = 0; current < COUNT; current++) 
        {
           free(history[current]);
           history[current] = NULL;
        }
        return 0;
}

