# TCP-Client-Server-Application
Implement a TCP client- server application to implement a service that returns the sounds made by animals. The name of each requested animal will be specified by a client . The server will start in passive mode listening on a specified port for a transmission from a client. Separately, the client will be started and will contact the server on a given IP address and port number that must be entered via the command line. 
 
### For communication with server client has to register itself with server.
## The server, which will be called SoundServer, will operate as follows:
*  Return the sound made by animals named by a client after the client connects to the server using the SOUND instruction,
*  Accepts a STORE message used for storing new (animal, sound) pairs,
*  Accepts a QUERY message used to ask which animals it knows,
*  Accepts a BYE message that results in closing of the current session between the client and the server.
*  Accepts an END message that results in closing of the current session and termination of the server.

### Use the following messages to communicate with server : 
1. register - To register with server
2. login - To communicate with server (if client is already registered)
3. sound - To confirm that connection has established
4. store - To store new animal-voice pair
5. query - To get names of all stored animals
6. bye   - To instruct server that client no longer needs this session
7. end   - To instruct server to shut down
8. animal name - To get voice of given animal

Client: SOUND
Server: SOUND: OK

### To get the voice of given animal
Client: DOG
Server: A DOG SAYS WOOF

### If client enters name of animal that is not stored in server's memory
Client: CAT
Server: I DON’T KNOW CAT

### To store a new animal-voice pair
Client: STORE
CAT
MEOW
SERVER: STORE: OK

### To get names of all animals stored in server's memory
Client: QUERY
SERVER: DOG
HORSE
SNAKE
COW
SHEEP
CAT
QUERY: OK

### Client is shutdown,not the server
Client: BYE
SERVER: BYE: OK

### Server and all clients are shutdown
Client: END
Server: END: OK




