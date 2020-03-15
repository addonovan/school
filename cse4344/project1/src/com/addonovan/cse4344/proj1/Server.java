package com.addonovan.cse4344.proj1;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Paths;
import java.util.LinkedList;
import java.util.List;

public class Server {

    /** The server socket listening for incoming connections. */
    private final ServerSocket socket;

    /**
     * A list of clients available to process. Each client is handled
     * asynchronously from the main thread, which accepts connections,
     * by a set of background worker threads.
     */
    private final List<Socket> availableClients = new LinkedList<>();

    /**
     * Initializes a server which will listen on the given {@code port}.
     *
     * @param port
     *          The port to listen to.
     */
    public Server(int port) {
        try {
            socket = new ServerSocket(port);
        } catch (IOException e) {
            throw new RuntimeException("Failed to initialize server", e);
        }
    }

    private HttpResponse handleConnection(HttpRequest request) {
        Logger.info("%s %s", request.getMethod(), request.getPath());

        HttpResponse response = new HttpResponse();
        response.setContentPath(Paths.get(".", request.getPath()));

        return response;
    }

    /**
     * Starts the server, which will accept all incoming connections
     * and add them to the work pool to be processed by a background
     * thread.
     *
     * @param workPoolSize
     *          The number of worker threads to run.
     */
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

    /**
     * A {@code ConnectionHandler} will wait for an active connection
     * and grab it out of the list of {@link #availableClients} and
     * then proceed to fulfill and respond to the request.
     */
    private class ConnectionHandler implements Runnable {

        /** The internal id for this connection handler */
        private final int id;

        /**
         * @param id
         *          {@link #id}
         */
        public ConnectionHandler(int id) {
            this.id = id;
        }

        /**
         * Blocks the thread while waiting for a client to connect
         * and be ready to serve.
         *
         * @return The next client that was available.
         */
        private Socket getNextConnection() {
            while (true) {
                synchronized (availableClients) {
                    if (!availableClients.isEmpty()) {
                        return availableClients.remove(0);
                    }
                }

                // cycle holding onto the lock periodically so we don't deadlock
                // while the main thread is trying to add new clients into the
                // queue
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    Logger.warn(
                            "Failed to wait between checking for available clients%n%s",
                            Util.getStackTrace(e)
                    );
                }
            }
        }

        @Override
        public void run() {
            Thread.currentThread().setName("Worker" + id);
            Logger.info("Thread started");

            while (!socket.isClosed()) {
                try {

                    Socket client = getNextConnection();
                    Logger.info(
                            "Handling connection from %s:%d",
                            client.getInetAddress(),
                            client.getPort()
                    );

                    HttpRequest request = HttpRequest.from(client.getInputStream());
                    HttpResponse response = handleConnection(request);

                    response.writeTo(client.getOutputStream());

                    client.close();
                } catch (Throwable t) {
                    Logger.error("Uncaught exception:\n%s", Util.getStackTrace(t));
                }
            }

            Logger.info("Thread ended");
        }

    }

}
