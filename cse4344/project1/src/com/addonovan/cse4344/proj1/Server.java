package com.addonovan.cse4344.proj1;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

public class Server {

    private final ServerSocket socket;

    private final List<Socket> availableClients = new LinkedList<>();

    public Server(int port) throws IOException {
        socket = new ServerSocket(port);
    }

    private void handleConnection(HttpRequest request) throws IOException {
        Logger.info("%s %s", request.getMethod(), request.getPath());
    }

    public void start(int workPoolSize) {
        Thread.currentThread().setName("Main");

        Logger.info("Accepting connections on port %d", socket.getLocalPort());

        // spawn the threadpool to handle connections
        for (int i = 0; i < workPoolSize; i++) {
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
            while (true) {
                synchronized (availableClients) {
                    if (!availableClients.isEmpty()) {
                        return availableClients.remove(0);
                    }
                }

                // cycle holding onto the lock periodically so we don't deadlock
                // while the main thread is trying to add new clients into the
                // queue
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

                    HttpRequest request = HttpRequest.from(client.getInputStream());
                    handleConnection(request);
                    client.close();

                } catch (Exception e) {
                    e.printStackTrace();
                    Logger.error("Uncaught exception:\n", Util.getStackTrace(e));
                }
            }
        }

    }

}
