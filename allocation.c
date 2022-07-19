#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Structs
typedef struct partition {
    int address;
    int size;
    int PID;
}Partition;

typedef struct hole {
    int address;
    int size;
}Hole;

// Globlal variables
int MAX_MEMORY;
Partition partitions[100];
Hole holes[100];

// Main Functions for the program
void Allocate(char* PID, int size, char *type); // Riley 
/*
Allocates memory from a hole to a process based on the algorithm chosen.
Type = 'F' for first fit, 'B' for best fit, 'W' for worst fit.
*/
void Deallocate(char* PID); // Riley
/*
Deallocates memory from a process to a hole.
*/
void Status(); // Christine
/*
Prints the status of the memory manager.
*/
void Compact(); // Christine
/*
Compacts the memory manager by shifting all current processes addresses to the top and combining all holes into one larger hole.
*/

// Helper Functions for the program
void printError(char *error); 
/*
Prints an error message to the screen.
*/
void lowercaseString(char *str); 
/*
Converts a string to lowercase.
*/
int isValidPID(char* PID); 
/*
Checks if a PID is valid.
*/
int isNumber(char* str); 
/*
Checks if a string is a number.
*/

int main(int argc, char *argv[]) {
    
    // initialize first hole
    if(argc == 2) {
        MAX_MEMORY = atoi(argv[1]);
        Hole memory = {0, MAX_MEMORY};
        holes[0] = memory;
        printf("HOLE INITIALIZED AT ADDRESS %d WITH %d BYTES\n", holes[0].address, holes[0].size);
    }
    else {
        printError("ERROR Invalid number of arguments.\n");
        return 1;
    }
    
    while(1){
        char *input = malloc(sizeof(char) * 100);
        printf("allocator>");
        fgets(input, 100, stdin);
        input[strcspn(input, "\n")] = '\0'; // remove newline from input

        if(input[0] == '\0') { // empty input = do nothing (segmentation fault if not)
            continue;
        }

        lowercaseString(input);

        char* arguments[3];
        char* token = strtok(input, " ");
        int tokenCount = 0;

        // get all arguments from input
        while(token != NULL){
            arguments[tokenCount] = token;
            token = strtok(NULL, " ");
            tokenCount++;
        }
        
        // RQ (Require Memory / Allocate): Needs 4 arguments and must check if they are valid arguments
        if(strcmp(arguments[0], "rq") == 0){
            if(tokenCount == 4 && isValidPID(arguments[1]) && isNumber(arguments[2]) && isalpha(*arguments[3])){
                Allocate(arguments[1], atoi(arguments[2]), arguments[3]);
            }
            else{
                printError("ERROR Expected expression: RQ \"PID\" \"Bytes\" \"Type\"");
            }
        }
        // RL (Release Memory / Deallocate): Needs 2 arguments and must check if they are valid arguments
        else if(strcmp(arguments[0], "rl") == 0){
            if(tokenCount == 2 && isValidPID(arguments[1])){
                Deallocate(arguments[1]);
            }
            else{
                printError("ERROR Expected expression: RL \"PID\"");
            }
        }
        // STATUS: Needs 1 argument
        else if(strcmp(arguments[0], "status") == 0){
            if(tokenCount == 1){
                Status();
            }
            else{
                printError("ERROR Expected expression: STATUS");
            }
        }
        // C (Compact): Needs 1 argument
        else if(strcmp(arguments[0], "c") == 0){
            if(tokenCount == 1){
                Compact();
            }
            else{
                printError("ERROR Expected expression: C");
            }
        }
        // EXIT: Needs 1 argument
        else if(strcmp(arguments[0], "exit") == 0){
            if(tokenCount == 1){
                printf("Exiting program.\n");
                break;
            }
            else{
                printError("ERROR Expected expression: EXIT");
            }
        }
        // If command is not recognized, print error message and continue
        else{
            printError("ERROR Invalid command.");
        }
    }
}

void printError(char* error){
    printf("\033[1;31m");
    printf("===== %s =====\n", error);
    printf("\033[0m");
}
void lowercaseString(char* str) {
    for(int i = 0; str[i] != '\0'; i++){
        str[i] = tolower(str[i]);
    }
}
int isValidPID(char* PID){
    if(PID[0] != 'p' || isNumber(&PID[1]) == 0){
        printError("ERROR Invalid PID");
        return 0;
    }
    return 1;
}
int isNumber(char* str){
    for(int i = 0; str[i] != '\0'; i++){
        if(!isdigit(str[i])){
            printError("ERROR Invalid number");
            return 0;
        }
    }
    return 1;
}

void Allocate(char* PID, int size, char* type) {
    // TODO: implement first fit, best fit, and worst fit algorithms
    printf("Allocating %d bytes to PID %s using %s algorithm\n", size, PID, type);

    // first fit
    if(strcmp(type, "f") == 0){
        
    }
    // best fit
    else if(strcmp(type, "b") == 0){
        
    }
    // worst fit
    else if(strcmp(type, "w") == 0){
        
    }

    else{
        printError("ERROR Invalid algorithm type");
        return;
    }
}

void Deallocate(char* PID) {
    // TODO: implement deallocation
    printf("Deallocating PID %s\n", PID);
}

void Status() {
    // TODO: implement status
    printf("Status:\n");
}

void Compact() {
    // TODO: implement compact
    printf("Compacting\n");
}