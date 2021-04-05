#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define COMMANDSIZE 500
#define COMMANDHISTORY 100

int numCommands = 0;

typedef struct History{

    char cmd[COMMANDSIZE];
    int cmdValue;

} history;


void commandInterfaceHandler();
void checkIfFileExists(char *fileName);

int main(int argc, char const *argv[])
{
    
    commandInterfaceHandler();

    return 0;
}


void commandInterfaceHandler(){

    //Allocate an initial block of space
    char *command = (char*)malloc(COMMANDSIZE * sizeof(char));
    char *delimiter = "\t\n ";
    history *myHistory = (history*)malloc(COMMANDHISTORY * sizeof(history));

    while(1){

        fflush(stdout);

        printf("# ");

        fgets(command, COMMANDSIZE, stdin);

        char *tokenizedInput = strtok(command, delimiter);

        strcpy(myHistory[numCommands].cmd, command); //Store the current command in command history
        myHistory[numCommands].cmdValue = numCommands; //Store the number of the most recent command

        if(strcmp(command, "byebye") == 0){

            exit(1); //Exit the shell program

        } else if(strcmp(tokenizedInput, "dwelt") == 0){

            tokenizedInput = strtok(NULL, delimiter); //Get the file name

            //Check if it's a file or directory
            //checkInputType(tokenizedInput);

            checkIfFileExists(tokenizedInput);

            
        }
        
    }
}


void checkIfFileExists(char *fileName){

    if(access(fileName, F_OK) == 0){

        printf("Dwelt Indeed.\n"); //File exists

    } else {

        printf("Dwelt not.\n"); //File doesn't exist

    }
}

