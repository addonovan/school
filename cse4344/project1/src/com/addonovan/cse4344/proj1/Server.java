package com.addonovan.cse4344.proj1;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Server {

    private final ServerSocket socket;

    private final ArrayList<Socket> availableClients = new ArrayList<>();

    public Server(int port) throws IOException {
        socket = new ServerSocket(port);
    }

    private void handleConnection(Socket client) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));

        bw.write("HTTP/1.1 200 OK\n");
        bw.write("Content-Length: 13\n");
        bw.write("Content-Type: text/plain; charset=utf-8\n");
        bw.write("\n");
        bw.write("Hello world!\n");
        bw.flush();
    }

    public void start() {
        Thread.currentThread().setName("Main");

        Logger.info("Accepting connections on port %d", socket.getLocalPort());

        // spawn the threadpool to handle connections
        for (int i = 0; i < 4; i++) {
            Thread worker = new Thread(new ConnectionHandler(i));
            worker.start();
        }

        while (!socket.isClosed()) {
            try {
                Socket client = socket.accept();
                synchronized (availableClients) {
                    availableClients.add(client);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class ConnectionHandler implements Runnable {

        private final int id;

        public ConnectionHandler(int id) {
            this.id = id;
        }

        private Socket getNextConnection() throws InterruptedException {
            Logger.info("Waiting for connection...");
            while (true) {
                synchronized (availableClients) {
                    if (!availableClients.isEmpty()) {
                        return availableClients.remove(0);
                    }
                }

                Thread.sleep(10);
            }
        }

        @Override
        public void run() {
            Thread.currentThread().setName("Worker" + id);
            Logger.info("Thread started");

            while (true) {
                try {

                    Socket client = getNextConnection();
                    Logger.info(
                            "Handling connection from %s:%d",
                            client.getInetAddress(),
                            client.getPort()
                    );

                    handleConnection(client);
                    client.close();

                } catch (Exception e) {
                    Logger.error("Uncaught exception:\n", Util.getStackTrace(e));
                    break;
                }
            }

            Logger.info("Thread exited");
        }

    }

}
