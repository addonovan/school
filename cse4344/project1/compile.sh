#!/bin/bash

rm -rf sources.txt
echo "./src/client/Logger.java" >> sources.txt
echo "./src/client/Request.java" >> sources.txt
echo "./src/client/Connection.java" >> sources.txt
echo "./src/client/Main.java" >> sources.txt
echo "./src/server/ClientHandler.java" >> sources.txt
echo "./src/server/Util.java" >> sources.txt
echo "./src/server/Logger.java" >> sources.txt
echo "./src/server/Server.java" >> sources.txt
echo "./src/server/Main.java" >> sources.txt

rm -rf bin
mkdir -p bin
javac -d bin @sources.txt
rm -rf sources.txt

function compile {
  rm -rf "$1.jar"

  rm -rf bin/manifest.mf
  echo "Manifest-Version: 1.0" >> bin/manifest.mf
  echo "Main-Class: $1.Main" >> bin/manifest.mf

  cd bin
  jar cfm "../$1.jar" manifest.mf *
  cd ..
}

compile "client"
compile "server"

rm -rf bin
