# TCP-Client-Server-Application
Implement a TCP client- server application to implement a service that returns the sounds made by animals. The name of each requested animal will be specified by a client . The server will start in passive mode listening on a specified port for a transmission from a client. Separately, the client will be started and will contact the server on a given IP address and port number that must be entered via the command line. 
 
### For communication with server client has to register itself with server.
## The server, which will be called SoundServer, will operate as follows:
*  Return the sound made by animals named by a client after the client connects to the server using the SOUND instruction,
*  Accepts a STORE message used for storing new (animal, sound) pairs,
*  Accepts a QUERY message used to ask which animals it knows,
*  Accepts a BYE message that results in closing of the current session between the client and the server.
*  Accepts an END message that results in closing of the current session and termination of the server.

