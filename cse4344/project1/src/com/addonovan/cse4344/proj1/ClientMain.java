package com.addonovan.cse4344.proj1;

import java.io.*;
import java.net.Socket;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;
import java.util.Scanner;

public class ClientMain {

    /**
     * @param args
     *          Command line arguments from {@link #main}
     * @return Parsed command line arguments.
     */
    private static Arguments parseArgs(String[] args) {
        Arguments parsed = new Arguments();

        switch (args.length) {
            case 0:
                Logger.error("You must supply the server address");
                break;

            case 1:
                parsed.serverAddress = args[0];
                return parsed;

            case 2:
                parsed.serverAddress = args[0];

                try {
                    parsed.port = Integer.parseInt(args[1]);
                } catch (NumberFormatException e) {
                    parsed.file = args[1];
                }
                return parsed;

            case 3:
                parsed.serverAddress = args[0];
                parsed.file = args[2];

                try {
                    parsed.port = Integer.parseInt(args[1]);
                } catch (NumberFormatException e) {
                    Logger.error("Failed to parse port number: %s%n", args[1]);
                    e.printStackTrace();
                    System.exit(1);
                    break;
                }

                return parsed;
        }

        System.exit(1);
        throw new RuntimeException("Unreachable");
    }

    /**
     * Sends an HTTP request to the server for the given file.
     *
     * @param os
     *          The outbound connection to server.
     * @param fileName
     *          The name of the file to request.
     * @throws IOException If there were any problems communicating with the server.
     */
    private static void sendRequest(OutputStream os, String fileName) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));
        // GET /{filename} HTTP/1.1
        // Connection: close
        //

        bw.write("GET /");
        bw.write(fileName);
        bw.write(" HTTP/1.0\r\n");
        // if we're interacting with outside servers, this will make them immediately
        // close the connection after responding to us, so that we can just read the
        // body of the response out easily
        bw.write("Connection: close\r\n");
        bw.write("\r\n");

        bw.flush(); // flush to actually send the data to the server
    }

    /**
     * Reads the response of the body.
     *
     * @param is
     *          The inbound stream of data from the server.
     * @throws IOException If there is any problem reading data from the server.
     */
    private static void readResponse(InputStream is) throws IOException {
        Scanner scanner = new Scanner(is);
        scanner.next(); // skip the protocol specification
        String statusCode = scanner.next();
        String statusText = scanner.nextLine();

        if (statusCode.equals("200")) {
            Logger.info("Got 200 OK");
        } else {
            Logger.warn("Server responded with %s %s", statusCode, statusText);
        }

        // throw out all the headers
        while (!scanner.nextLine().isBlank());

        // read the rest of the information as the body
        StringBuilder sb = new StringBuilder();
        while (scanner.hasNextLine()) {
            sb.append(scanner.nextLine());
            sb.append("\n");
        }
        Logger.info("Content:%n%s", sb);
    }

    public static void main(String[] args) {
        Arguments parsed = parseArgs(args);

        final Socket socket;
        try {
            Logger.info("Opening connection to %s:%s", parsed.serverAddress, parsed.port);
            socket = new Socket(parsed.serverAddress, parsed.port);
        } catch (IOException e) {
            Logger.error("Failed to open connection to server:%n%s", Util.getStackTrace(e));
            return;
        }

        try {
            Logger.info("Requesting %s", parsed.file);
            sendRequest(socket.getOutputStream(), parsed.file);
        } catch (IOException e) {
            Logger.error("Failed to send connection to server:%n%s", Util.getStackTrace(e));
            return;
        }

        try {
            Logger.info("Waiting for response...");
            readResponse(socket.getInputStream());
        } catch (IOException e) {
            Logger.error("Failed to read response from server: %n%s", Util.getStackTrace(e));
        }

        try {
            socket.close();
        } catch (IOException e) {
            Logger.error("Failed to close socket:%n%s", Util.getStackTrace(e));
        }
    }

    private static final class Arguments {

        public String serverAddress = null;

        public int port = 8080;

        public String file = "index.htm";

    }

}
