# Chapter 7.7 description

The aim of this labwork is to understand how to communicate between different proceeses using different methods to satisfiy requirements (IPC communication).

## Exercise 1
### Requirements
Make 2 programs that can communicate and share a data store

### Solution
Using message queue for communication and a shared memory for 2 program to transfer data between.

Detail description can be found in the source code

### How to run
- Build the 2 program program1 and program2 with the source code in `program1.c` and `program2.c`, respectively

- Run `program1` first, then run `program2`

- `program2` can freely send any number, and `program1` will perform actions to receive data, as well as add new number to the current shared memory

- Note that if a value of `-1` is sent, both programs terminates

## Exercise 2
### Requirement
Create a processes to monitor child processes to work in order

### Solution
In this program, use use an additonal library of doubly linked list to save information of incoming child processes using the same shared memory. `child.c` will spawn 3 processes which will make use of the shared memory to change values of the variables. `parent.c` will take notice of the comming order of these processes by putting them into a linked list (which can be consider as a queue here). After each done process, it will pick that process out of the queue and allow the next process to execute.

### How to run
- Build `parent` and `child` with `parent.c` and `child.c`, respectively

- Run `parent` first, then `child`

- We can see on the terminal of `child`, there are 3 process forked. Since they are monitored by `parent`, the value of x and y will be changed respectively from (0, 0), to (10, 0) and (0, 10) without any conflict.

