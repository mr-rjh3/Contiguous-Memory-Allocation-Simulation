#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Structs
typedef struct partition {
    int address;
    int size;
    char* PID;
    struct partition * next;
}Partition;

typedef struct hole {
    int address;
    int size;
    struct hole* next;
}Hole;

// Globlal variables
int MAX_MEMORY;
Partition* partitionList = NULL;
Hole* holeList = NULL;

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
        Hole memory = {0, MAX_MEMORY, NULL};
        holeList = &memory;
        //holes[0] = memory;
        printf("HOLE INITIALIZED AT ADDRESS %d WITH %d BYTES\n", memory.address, memory.size);
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
                printError("ERROR Expected expression: RQ \"PID\" \"Bytes\" \"Algorithm\"");
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
                exit(0);
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

    // Check to see if size is valid
    if(size <= 0){
        printError("ERROR Invalid size. Must be greater than 0.");
        return;
    }
    // Check to see if PID is currently being used
    Partition* current = partitionList;
    while(current != NULL){
        if(strcmp(current->PID, PID) == 0){
            printError("ERROR PID is already in use.");
            return;
        }
        current = current->next;
    }
    
    Hole* prevHole = NULL;
    Hole* currHole = holeList;
    // first fit
    if(strcmp(type, "f") == 0){
        printf("Attempting to allocate %d bytes to process %s using FIRST FIT algorithm\n", size, PID);
        while(currHole->next != NULL && currHole->size < size){
            printf("HOLE AT ADDRESS %d WITH %d BYTES\n", currHole->address, currHole->size);
            prevHole = currHole;
            currHole = currHole->next;
        }
    }
    // best fit
    else if(strcmp(type, "b") == 0){
        printf("Attempting to allocate %d bytes to process %s using BEST FIT algorithm\n", size, PID);
        Hole* bestFit = holeList;
        Hole* bestFitPrev = NULL;
        while(currHole != NULL){
            if(currHole->size < bestFit->size && currHole->size >= size){
                bestFit = currHole;
                bestFitPrev = prevHole;
            }
            prevHole = currHole;
            currHole = currHole->next;
        }
        currHole = bestFit;
        prevHole = bestFitPrev;
        
    }
    // worst fit
    else if(strcmp(type, "w") == 0){
        printf("Attempting to allocate %d bytes to process %s using WORST FIT algorithm\n", size, PID);
        Hole* worstFit = holeList;
        Hole* worstFitPrev = NULL;
        while(currHole != NULL){
            if(currHole->size > worstFit->size){
                worstFit = currHole;
                worstFitPrev = prevHole;
            }
            prevHole = currHole;
            currHole = currHole->next;
        }
        currHole = worstFit;
        prevHole = worstFitPrev;
    }
    // if no fit found, print error message and return
    else{
        printError("ERROR Invalid algorithm type");
        return;
    }

    // if there was not enough memory, print error message and return
    if(currHole->size < size){
            printError("ERROR Not enough memory");
            return;
    }
    // if fit found, allocate memory to partition and update hole
    else{
        // create new partition
        Partition* newPartition = (Partition*)malloc(sizeof(Partition));
        newPartition->PID = PID;
        newPartition->size = size;
        newPartition->address = currHole->address;
        newPartition->next = NULL;
        
        if(partitionList == NULL){ // if partition list is empty, add to front
            partitionList = newPartition;
        }
        else{ // otherwise sort insertion into partition list
            Partition* currPartition = partitionList;
            Partition* prevPartition = NULL;
            while(currPartition != NULL && currPartition->address < newPartition->address){
                prevPartition = currPartition;
                currPartition = currPartition->next;
            }
            if(prevPartition == NULL){ // if new partition is at front of list, add to front
                newPartition->next = partitionList;
                partitionList = newPartition;
            }
            else{ // otherwise add to middle / end of list
                prevPartition->next = newPartition;
                newPartition->next = currPartition;
            }
        }
        
        if(currHole->size == size){ // if hole is the same size as partition, remove hole from hole List
            if(prevHole == NULL){ // if hole is first in list
                holeList = currHole->next;
            }
            else{ // if hole is not first in list
                prevHole->next = currHole->next;
            }
            free(currHole); // free hole
        }
        else{ // Otherwise the hole is larger than partition, update hole address and size in this case
            currHole->address = currHole->address + size;
            currHole->size = currHole->size - size;
        }
       
        printf("Memory allocated successfully.\n");
    }
}

void Deallocate(char* PID) {
    printf("Deallocating PID %s\n", PID);

    // Find PID in partition list
    Partition* currPartition = partitionList;
    Partition* prevPartition = NULL;
    while(currPartition != NULL && strcmp(currPartition->PID, PID) != 0){
        prevPartition = currPartition;
        currPartition = currPartition->next;
    }
    
    // if PID is not in partition list, print error message and return
    if(currPartition == NULL){ 
        printError("ERROR PID is not in use.");
        return;
    }
    else{ // Otherwise PID is in partition list, deallocate memory and remove from partition list
       
        // Create new hole from partition
        Hole* newHole = (Hole*)malloc(sizeof(Hole));
        newHole->address = currPartition->address;
        newHole->size = currPartition->size;

        // Remove partition from partition list
        if(prevPartition == NULL){ // if PID is first in list , remove from front
            partitionList = currPartition->next;
        }
        else{ // if PID is not first in list remove from middle / end of list
            prevPartition->next = currPartition->next;
        }
        free(currPartition);

        // add the new hole to hole list
        if(holeList == NULL){ // if hole list is empty, add to front
            holeList = newHole;
        }
        else{ // sort insertion into hole list
            Hole* currHole = holeList;
            Hole* prevHole = NULL;
            while(currHole != NULL && currHole->address < newHole->address){
                prevHole = currHole;
                currHole = currHole->next;
            }
            if(prevHole == NULL){ // if new hole is first in list
                newHole->next = holeList;
                holeList = newHole;
            }
            else{ // if new hole is not first in list
                newHole->next = currHole;
                prevHole->next = newHole;
            }

            // check if new hole can be merged with adjacent holes
            if(newHole->next != NULL && newHole->address + newHole->size == newHole->next->address){ // if we can merge with next hole then merge
                newHole->size = newHole->size + newHole->next->size;
                newHole->next = newHole->next->next;
            }
            if(prevHole != NULL && prevHole->address + prevHole->size == newHole->address){ // if we can merge with previous hole then merge
                prevHole->size = prevHole->size + newHole->size;
                prevHole->next = newHole->next;
            }
        }
    }
}

void Status() {
    Partition* currentP = partitionList;
    int taken = 0;
    
    // While the end of the linked list is not reached, 
    // Print the address information and increment the list
    printf("Paritions: \n");
    while(currentP != NULL) {
        printf("\tAddress [%d:%d] Process %s \n", currentP->address, currentP->address + currentP->size -1, currentP->PID);
        taken = taken + currentP->size;
        currentP = (currentP->next);
    }
    printf("\tTotal Allocated Memory = [%d]\n", taken);

    printf("Holes: \n");
    Hole* currentH = holeList;
    while(currentH != NULL) {
        printf("\tAddress [%d:%d] len %d\n", currentH->address, currentH->address + currentH->size -1, currentH->size);
        currentH = (currentH->next);
    }
    printf("\tFree memory = [%d]\n", MAX_MEMORY - taken);

}void Compact() {
    
    // Only compact if there is room to shuffle processes and if there are processes to compact
    if(holeList != NULL && partitionList != NULL) {
        Partition* currentP = partitionList;
        int memStart = 0;

        // While the end of the linked list is not reached,
        // Move the address start of the current partition to "memstart"
        // Update memstart to include the length of the current partition such that it holds the next available memory
        // Increment the list
        while(currentP != NULL) {
            currentP->address = memStart;
            memStart = memStart + currentP->size;
            currentP = (currentP->next);
        }

        // Update the head of the list to be the remainder memory hole
        holeList->address = memStart;
        holeList->size = MAX_MEMORY - memStart;

        // currentH holds the 2nd hole
        Hole* currentH = holeList->next;
        // Cutting off the list 
        holeList->next = NULL;
        // nextH is the next hole from currentH
        Hole* nextH = NULL;

        // Free the rest of the list
        // Free currentH using nextH as a temp variable to hold the rest of the hole List before reassigning currentH to nextH
        while(currentH != NULL) {
            nextH = currentH->next;
            free(currentH);
            currentH = nextH;
        }
        printf("Compaction process is successful\n");
    }

    else if(holeList == NULL) {
        printError("Compaction failed, no available memory.");
    }

    else {
        printError("No processes to compact.");
    }
}