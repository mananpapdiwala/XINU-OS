How the program works:
The program has one integer to hold the number of rounds, one integer to hold the number of processes and one character array to hold the version.  The default values for process is 4, rounds is 5 and version is work.
The program will first look if process_ring has multiple arguments.  If there isn't the program will just run with the default values.  
If there is, the program will go through all the arguments and look for any of the arguments that resemble the arguments required for the assignment and change either the 
number of processes, the number of rounds, or the version.  If the arguments are not in the correct format the program will show the appropriate message.  The user could use -h or --help
for more information on the commands.  

The program has 4 versions.

1) Work

xsh $ process_ring -p 5 -r 6
Number of processes: 5
Number of rounds: 6
Ring ELEMENT 0 : Round 0 : Value 30
Ring ELEMENT 1 : Round 0 : Value 29
Ring ELEMENT 2 : Round 0 : Value 28
Ring ELEMENT 3 : Round 0 : Value 27
Ring ELEMENT 4 : Round 0 : Value 26
xsh $ Ring ELEMENT 0 : Round 1 : Value 25
Ring ELEMENT 1 : Round 1 : Value 24
Ring ELEMENT 2 : Round 1 : Value 23
Ring ELEMENT 3 : Round 1 : Value 22
Ring ELEMENT 4 : Round 1 : Value 21
Ring ELEMENT 0 : Round 2 : Value 20
Ring ELEMENT 1 : Round 2 : Value 19
Ring ELEMENT 2 : Round 2 : Value 18
Ring ELEMENT 3 : Round 2 : Value 17
Ring ELEMENT 4 : Round 2 : Value 16
Ring ELEMENT 0 : Round 3 : Value 15
Ring ELEMENT 1 : Round 3 : Value 14
Ring ELEMENT 2 : Round 3 : Value 13
Ring ELEMENT 3 : Round 3 : Value 12
Ring ELEMENT 4 : Round 3 : Value 11
Ring ELEMENT 0 : Round 4 : Value 10
Ring ELEMENT 1 : Round 4 : Value 9
Ring ELEMENT 2 : Round 4 : Value 8
Ring ELEMENT 3 : Round 4 : Value 7
Ring ELEMENT 4 : Round 4 : Value 6
Ring ELEMENT 0 : Round 5 : Value 5
Ring ELEMENT 1 : Round 5 : Value 4
Ring ELEMENT 2 : Round 5 : Value 3
Ring ELEMENT 3 : Round 5 : Value 2
Ring ELEMENT 4 : Round 5 : Value 1
ZERO!

This is an example of process_ring that has a version "work".  The user also decided to input "-p 5" to change number of processes to 5 and "-r 6" to change number of rounds to 6.
Each process will take turns to complete a round.

Question: What property of Xinu allows the working version of your program to print values in the correct order?
Semaphore synchronization allows our program to print the values in the correct order.  The semaphore for the last process is set to 1 and all the other semaphores are set to 0.
The function for these programs has a wait system call that decrements the semaphore for the previous process' semaphore and a signal system call to increment 
the semaphore for the current process.  Each process will wait for the previous process to complete and the first process will wait for the last process to complete.




2) Hang
xsh $ process_ring -v hang
Version: hang
Number of processes: 4
Number of rounds: 5

Why does your program hang?  What is the name of this behavior in an operating system?
The process hangs because all the processes are waiting for each other.  We set all the semaphores to 0.  The name of this behavior is deadlock.  


3) Loop
We cannot print Loop.

Why does your program go into an infinite loop?  What is the name of this behavior in an operating system.
We have caused the for loop in the function to not reach the terminating condition.  The program will run an infinite loop. This behavior is called an endless loop or infinite loop.  


4) Chaos
xsh $ process_ring -v chaos
Number of Processes: 4
Number of Rounds: 5
Ring Element 0 : Round 0 : Value : 20
Ring Element 0 : Round 1 : Value : 19
Ring Element 0 : Round 2 : Value : 18
Ring Element 0 : Round 3 : Value : 17
Ring Element 0 : Round 4 : Value : 16
Ring Element 1 : Round 0 : Value : 15
Ring Element 1 : Round 1 : Value : 14
Ring Element 1 : Round 2 : Value : 13
Ring Element 1 : Round 3 : Value : 12
Ring Element 1 : Round 4 : Value : 11
Ring Element 2 : Round 0 : Value : 10
Ring Element 2 : Round 1 : Value : 9
Ring Element 2 : Round 2 : Value : 8
Ring Element 2 : Round 3 : Value : 7
Ring Element 2 : Round 4 : Value : 6
Ring Element 3 : Round 0 : Value : 5
Ring Element 3 : Round 1 : Value : 4
Ring Element 3 : Round 2 : Value : 3xsh $
Ring Element 3 : Round 3 : Value : 2
Ring Element 3 : Round 4 : Value : 1
ZERO!

The chaos version does not use any synchronization method between the processes and has the processes running in random order.  This behavior is called race condition.  


We both worked independently on the assignment and had discussions on regular intervals to discuss the problems others were facing.
Both wrote the work version independently and was almost similar. Manan's argument validations code was merged and Ethan's code for different process version was merged.