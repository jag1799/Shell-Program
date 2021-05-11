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


void kernelInterface();
void commandParser(char *command, history *myHistory);
void checkIfFileExists(char *fileName);
int checkInputType(char *tokenizedInput);
char *getCurrentDirectory();
void changeCurrentDirectory(char *tokenizedInput);
void makeNewFile(char *tokenizedInput);
char *getSourceFileContents(char *sourceFile, char *sourceFileContents, int sourceFileSize);
void destinationFileContents(char *destinationFileName, char *sourceFileContents);
void printHistory(history *myHistory, int numCommands);
void clearHistory(history *myHistory, int numCommands);
void executeCommand(int commandNumber, history *myHistory, int numCommands);
int charToInt(char tokenizedInput);
void executeProgram(char** programPath, char* arguments[]);

char currentDir[256];


int main(int argc, char const *argv[])
{
    
    kernelInterface();

    return 0;
}


void kernelInterface(){

    char *command = (char*)malloc(COMMANDSIZE * sizeof(char));
    history *myHistory = (history*)malloc(COMMANDHISTORY * sizeof(history));
    getCurrentDirectory();

    while(1){
        
        fflush(stdout);
        printf("# ");

        fgets(command, COMMANDSIZE, stdin);

        commandParser(command, myHistory);

    }
}


void commandParser(char *command, history *myHistory){

    //Allocate an initial block of space
    //char *command = (char*)malloc(COMMANDSIZE * sizeof(char));
    char *delimiter = "\t\n ";
    
    int sourceFileSize = 500;

    char *tokenizedInput = strtok(command, delimiter);

    strcpy(myHistory[numCommands].cmd, tokenizedInput); //Store the current command in command history
    myHistory[numCommands].cmdValue = numCommands; //Store the number of the most recent command

    numCommands++;

    if(strcmp(command, "end") == 0){

        exit(1); //Exit the shell program

    } else if(strcmp(tokenizedInput, "exist") == 0){ //Check if a file or directory exists

        tokenizedInput = strtok(NULL, delimiter); //Get the file name

        strcat(myHistory[numCommands-1].cmd, " ");
        strcat(myHistory[numCommands-1].cmd, tokenizedInput);

        //Check if it's a file or directory
        int type = checkInputType(tokenizedInput);

        if(type == 1){

            //Directory type
            printf("Directory exists.\n");

        } else {

            checkIfFileExists(tokenizedInput);

        }
    } else if(strcmp(tokenizedInput, "currentdir") == 0){ //Check the current working directory

        printf("Current Directory: %s\n", currentDir);

    } else if(strcmp(tokenizedInput, "movetodir") == 0){ //Move to the designated directory

        tokenizedInput = strtok(NULL, delimiter);

        strcat(myHistory[numCommands-1].cmd, " ");
        strcat(myHistory[numCommands-1].cmd, tokenizedInput);

        changeCurrentDirectory(tokenizedInput);

    } else if(strcmp(tokenizedInput, "fnew") == 0){ //Make a new file in the current directory

        tokenizedInput = strtok(NULL, delimiter); //Get the file name

        makeNewFile(tokenizedInput);
    
    } else if(strcmp(tokenizedInput, "copy") == 0){ //Copy the contents of one file over to another, new, file

        tokenizedInput = strtok(NULL, delimiter); //

        //Note, tokenizedInput[5] is where the '-' symbol is.
        //Thus, tokenizedInput[6] would be where the file name starts
        char sourceFileName[20];
        char destinationFileName[20];
        int i, j = 5;

        for(i = 0; i < 20; i++){

            sourceFileName[i] = tokenizedInput[j];
            j++;
        }

        sourceFileName[i] = '\0';

        //Something to hold the file contents
        char *sourceFileContents = (char*)malloc(sourceFileSize * sizeof(char));

        sourceFileContents = getSourceFileContents(sourceFileName, sourceFileContents, sourceFileSize);

        tokenizedInput = strtok(NULL, delimiter); //Get the destination file name

        j = 3;
        for(i = 0; i < 20; i++){

            //Store the destination file separately from the tokenized input (removes the "to-" bit)
            destinationFileName[i] = tokenizedInput[j];
            j++;

        }

        //Store the contents of the source file in the new one for a copy
        destinationFileContents(destinationFileName, sourceFileContents);

    } else if(strcmp(tokenizedInput, "history") == 0){

        tokenizedInput = strtok(NULL, delimiter);

        if(tokenizedInput != NULL){

            if(strcmp(tokenizedInput, "clr") == 0){

                clearHistory(myHistory, numCommands);
                numCommands = 0;

            }

        } else {

            printHistory(myHistory, numCommands);

        }

    } else if(strcmp(tokenizedInput, "replay") == 0){

        tokenizedInput = strtok(NULL, delimiter);

        //Convert the number into an actual integer for execution
        int commandNumber = charToInt(tokenizedInput[0]);

        executeCommand(commandNumber, myHistory, numCommands);

    }
    
}


int charToInt(char tokenizedInput){

    int commandNum = tokenizedInput - '0';

    return commandNum;
}


void executeCommand(int commandNumber, history *myHistory, int numCommands){


    if(commandNumber > numCommands){

        printf("Error: Command value requested exceeds number of commands used.\n");

    } else {

        char *cmdToExecute = myHistory[commandNumber].cmd;

        commandParser(cmdToExecute, myHistory);
    }

}


void printHistory(history *myHistory, int numCommands){

    int i;
    for(i = 0; i < numCommands; i++){

        printf("[%d] - %s\n", myHistory[i].cmdValue, myHistory[i].cmd);

    }
}


void clearHistory(history *myHistory, int numCommands){

    int i;
    for(i = 0; i < numCommands-1; i++){

        memset(myHistory[i].cmd, 0, sizeof(COMMANDSIZE));

        myHistory[i].cmdValue = 0;
        
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

        printf("File %s exists.\n", fileName); //File exists

    } else {

        printf("File %s does not exist.\n", fileName); //File doesn't exist

    }
}


char *getCurrentDirectory(){

    if(getcwd(currentDir, sizeof(currentDir)) == NULL){

        perror("getcwd error");

    }
}


char *getSourceFileContents(char *sourceFile, char *sourceFileContents, int sourceFileSize){

    
    //Check if the file exists
    if(access(sourceFile, F_OK) == 0){

        //Read the file
        FILE *sourcePtr = fopen(sourceFile, "r");
        
        //FIXME: CANNOT READ AN ARBITRARILY SIZED FILE. SEE YOUR OS BOOKMARKS IN FIREFOX
        char c;
        int size = 0;
        while((c = fgetc(sourcePtr)) != EOF){

            if(size >= sourceFileSize-1){

                sourceFileSize += 500;

                sourceFileContents = realloc(sourceFileContents, sourceFileSize);

            }

            sourceFileContents[size++] = c;
        }

        sourceFileContents[size++] = '\0';
        
        fclose(sourcePtr);

        return sourceFileContents;

    } else {

        printf("Source file does not exist.\n");

    }
}


void destinationFileContents(char *destinationFileName, char *sourceFileContents){

    if(access(destinationFileName, F_OK) == 0){

        printf("Destination file already exists.\n");

    } else {

        //Check if the directory the file is in exists (DOING LATER)

        FILE *destinationPtr = fopen(destinationFileName, "w");

        fprintf(destinationPtr, "%s", sourceFileContents);

        fclose(destinationPtr);
    }
}

