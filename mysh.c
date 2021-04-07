#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define COMMANDSIZE 500
#define COMMANDHISTORY 100

int numCommands = 0;

typedef struct History{

    char cmd[COMMANDSIZE];
    int cmdValue;

} history;


void commandInterfaceHandler();
void checkIfFileExists(char *fileName);
int checkInputType(char *tokenizedInput);
char *getCurrentDirectory();
void changeCurrentDirectory(char *tokenizedInput);
void makeNewFile(char *tokenizedInput);

char currentDir[256];


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

    getCurrentDirectory();

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
            int type = checkInputType(tokenizedInput);

            if(type == 1){

                //Directory type
                printf("Abode is.\n");

            } else {

                checkIfFileExists(tokenizedInput);

            }
        } else if(strcmp(tokenizedInput, "whereami") == 0){

            
            printf("Current Directory: %s\n", currentDir); //Display current working directory

        } else if(strcmp(tokenizedInput, "movetodir") == 0){

            tokenizedInput = strtok(NULL, delimiter);

            changeCurrentDirectory(tokenizedInput);

        } else if(strcmp(tokenizedInput, "maik") == 0){

            tokenizedInput = strtok(NULL, delimiter); //Get the file name

            makeNewFile(tokenizedInput);
        }
        
    }
}


void makeNewFile(char *tokenizedInput){

    if(access(tokenizedInput, F_OK) == 0){

        printf("File already exists.\n");

    } else {

        FILE *fp = fopen(tokenizedInput, "w");

        fprintf(fp, "Draft");
        fclose(fp);
    }
}


void changeCurrentDirectory(char *tokenizedInput){

    DIR *directory = opendir(tokenizedInput);

    if(directory){

        strcpy(currentDir, tokenizedInput);

        closedir(directory);

    } else {

        printf("Directory does not exist.\n");

    }
}


int checkInputType(char *tokenizedInput){

    struct stat path_stat;

    if(S_ISDIR(path_stat.st_mode) && !stat(tokenizedInput, &path_stat)){

        return 1; //It's a directory

    } else {

        return 2; //It's a file
    }
}

void checkIfFileExists(char *fileName){

    if(access(fileName, F_OK) == 0){

        printf("Dwelt Indeed.\n"); //File exists

    } else {

        printf("Dwelt not.\n"); //File doesn't exist

    }
}


char *getCurrentDirectory(){

    if(getcwd(currentDir, sizeof(currentDir)) == NULL){

        perror("getcwd error");

    }
}
