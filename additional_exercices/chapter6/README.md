# Chapter 6 description

The aim of this chapter is to understand how memory is organized in the system and how to take advantage of it

## Exercise
### Requirement

Make 2 processes that use the same memory map of 100 TestData instance

### Solution

Using functions for manipulating memory as described in the handouts

### How to use

- Build `writer` and `reader` respectively from `writer.c` and `reader.c`

- Run `writer` first. Before terminating the program, run `reader`

- We can indicate from the terminals' results that the 2 programs share the same memory segment
