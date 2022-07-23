# Contiguous Memory Allocation Simulation (CP386 A4 Spring 2022)

### Developers:
Riley Huston & Christine Nguyen are both Wilfrid Laurier University students studying computer science. 

### Features:
This program simulates contiguous memory allocation algorithms such as best fit, worst fit and first fit. It also contains auxiliary functions which displays current allocated processes in memory and where holes are located, as well as the ability to "compact" the processes to avoid external fragmentation. This project was done in WSL and the makefile was provided by our professor.

### How to Use:
The program continues to take keyboard input until the user enters "Exit" as the string. Otherwise, the user can enter several commands such as RQ, RL, Status and C.

RQ stands for request and takes 3 parameters, the PID of the process, how many bytes it requires and which allocation algorithm to use (b for best fit, w for worst fit and f for first fit). 
For example: RQ P0 1000 b

RL stands for release and takes 1 parameter, the PID of the process.
For example: RL P0

Status shows all processes and holes in memory. 

C stands for compact, it moves all processes up in a contiguous manner, leaving one hole.

Exit terminates the program.

The keyboard input is error checked and non case sensitive. 

### Development Log:
The first thing we did is read over the assignment documentation and extract required information such as the commands and what functions seemed to be necessary. From there we mapped out the functions in further detail and assigned work. Riley is to code the allocation and de-allocation and I, Christine, am to write the README.md, as well as code the status and compact features. Riley went on to create a template for the code for the both of us to complete by writing the names of the functions with a brief description of what it is expected to accomplish, as well as writing main().

As he worked on allocate and deallocate, there was some issues regarding how to handle processes and holes. The initial idea was to use arrays, but due to being static it was difficult to modify either one due to the empty array slot in the case of deletion. This could potential cause an issue where all the indexes of the processes were scattered. This was rectified by discussion and ultimately building a linked list and using pointers. Once that was complete, I was able to begin status and compact. 

Status was pretty simple, a loop through each linked list, parition and hole, printing their fields to console. Compact was much more difficult solely due to attempting to free all unnecessary holes from memory, using the free() command. I tried a few approaches but we ended up scraping the use of free all together, as the way I approached compact was to keep the head of the hole linked list and "delete" the rest of the list. Since the use of free was not directly necessary, we have omitted it. 

Later, Riley added colour for readability.

### Test Cases
For the RQ command, we ensure that the process can be allocated to any hole regardless of where it is in the list, when applicable for all algorithms (best, worst, first fit), as well as if no holes fit. These cover our edge cases. We also have additional checks to see if there are no holes, the size for the process is 0 or less, the PID given is already in use and if the given algorithm is invalid.

For the RL command, similar to RQ, we ensure all edge cases are covered by testing the release of processes in any spot in the list. We also took care to monitor situations in which RL is called if there are no processes in the list, as well as if the RL PID does not exist in the list. We have also tested RL which make holes collide (on the right, left and both) and how to merge them into one hole.

For Status, we confirmed it runs properly in the event of no processes and holes as well as in the intended cases where holes and processes exist.

For compact, we have tested it with a variety of hole numbers, such as no holes, 1 hole and multiple holes, and tested it with no processes and with already compacted processes. 

### Use Cases
An example of when to use this program would be in an educational setting for someone to learn how contigious allocation works in practicality and not just theory.

### Formal Contributions List:
allocate() and deallocate(), written by Riley

status(), compact() and README.md written by Christine

### Screenshots:
![image](https://user-images.githubusercontent.com/64939307/180585208-0e5b508f-1828-4533-a0d3-70dfa1098bad.png)
![image](https://user-images.githubusercontent.com/64939307/180585213-8a16b696-66d6-467b-b690-eaa08b7127e3.png)

### License:
The license for our code is open source / free use. If used, please include our names somewhere in the new project. 
