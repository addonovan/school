package server;

public final class Main {

    public static void main(String[] args) {
        final int port;
        if (args.length == 1) {
            port = Integer.parseInt(args[0]);
        } else {
            port = 8080;
        }

        final Server server = new Server(port);
        server.start();
    }

}
