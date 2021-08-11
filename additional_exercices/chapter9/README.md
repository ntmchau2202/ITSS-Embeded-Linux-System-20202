# Chapter 9 description

The aim of this chapter is to understand how to create a communication between 2 programs using TCP socket, and furthermore, network communication

## Exercises
### Requirements

Create an echo client-server, in which the server will send back the message sent from client in reversed order. When the client send `quit`, the server stop processing message

### Solution

Using socket functions as described in the handouts

### How to use

- Build `server` and `client` respectively with `server.c` and `client.c`

- Run server first, and then client. Type anything in `client` to send

- We can observer that the server will echo back to client the reversed message

- When the client terminates by sending `quit`, the server stops receiving from this client and waits for another connection
