package server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public final class Server {

    private final int port;

    public Server(int port) {
        this.port = port;
    }

    public void start() {
        try {
            final ServerSocket server = new ServerSocket(port);
            Logger.info("Accepting connections on port %d", port);

            while (!server.isClosed()) {
                try {
                    final Socket client = server.accept();

                    // spawn a thread to process the client
                    Thread thread = new Thread(new ClientHandler(client));
                    thread.setName("ClientThread-" + client.getPort());
                    thread.start();
                }
                catch (Exception e) {
                    Logger.warn("Failed to handle client:%n%s", Util.getStackTrace(e));
                }
            }

        } catch (IOException e) {
            throw new RuntimeException("Server exited unexpected", e);
        }
    }

}
