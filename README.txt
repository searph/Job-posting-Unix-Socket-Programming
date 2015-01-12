# Job-posting-Unix-Socket-Programming
Code file:directory_server.c/file_server.c/client.c

directory_server.c:Receive the registration information from 3 file servers and create "directory.txt". Then after receiving
the request from 2 clients, it can check "directory.txt","resource.txt" and "topology.txt" to choose the nearest file server
for each client which has the doc the client need.

file_server.c: Send the the registration information to directory server. After that, it will wait until a client request for
some certain doc and then it send the doc to the client.

client.c: First sending a request to directory server to ask for the nearest one file server which has the doc it needs.
Ather that, it will send the request to the certain file server and receive the doc from that file server.

How to run: First, you should complie all the three file. After that kill all the redundant processes which have not been closed and are still running in background.(especially the file_server) 
Then run the prigramme in the order directory_server, file_server and client. You will see the suitable messages on the screen.






