package com.addonovan.cse4344.proj1;

public final class ServerMain {

    /**
     * Parse the command-line arguments for the server.
     *
     * @param args
     *          Command line arguments from {@link #main}.
     * @return The port number to run the server on.
     */
    private static int parseArgs(String[] args) {
        if (args.length == 0) {
            return 8080;
        }

        try {
            return Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.err.printf("Could not parse port number from '%s'%n", args[0]);
        }

        System.exit(1);
        throw new RuntimeException("Unreachable");
    }

    public static void main(String[] args) {
        final int port = parseArgs(args);

        Server server = new Server(port);
        server.start(4);
    }

}
