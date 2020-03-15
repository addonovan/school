## Project 1 

Austin Donovan  
1001311620  

## Prerequisites

This requires a minimum of jdk 11 to be installed.

## Instructions 

### Linux
The `compile.sh` script will automatically compile and produce two
jar files, name `client.jar` and `server.jar`. 

#### Client
The client can be run like:
```bash
$ java -jar client.jar <server_address> [port] [resource]
```

#### Server
The server can be run like:
```bash
$ java -jar server.jar [port]
```

### Windows
For windows, I have included the IntelliJ IDEA project files which
also contain the run configurations to launch the client and server.

#### Client

The `client.Main` Run Configuration can be used to run the client,
as well as provide it with arguments, in the format stated in the 
requirements.

#### Server

The `server.Main` Run Configuration can be used to run the client,
as well as provide it with arguments, in the format stated in the
requirements. 
