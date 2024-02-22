# File-Transfer-Socket
A client-server based file transfer socket that can transfer any file from a client to a server.

Written purely in C.

## Compilation
Compile client script: ```gcc client.c -o client```  
Compile server script: ```gcc server.c -o server```

## Usage
Client-side script: ```./client <source filename>```  
Server-side script: ```./server <destination filename>```

## Example
Client-side (file sender) command: ```./client ./clone_me.jpg```  
Server-side (file receiver) command: ```./server ./cloned_image.jpg```
