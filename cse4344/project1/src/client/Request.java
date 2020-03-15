package client;

public final class Request {

    private static final int DEFAULT_PORT = 8080;

    private static final String DEFAULT_RESOURCE = "index.html";

    /**
     * Parses a {@link Request} from the command line arguments.
     *
     * @param args
     *          Command line arguments from {@link Main#main}
     * @return The parsed/defaulted arguments.
     */
    public static Request fromArgs(String[] args) {
        // print some help if the user requested it or didn't provide anything
        if (args.length == 0 || args[0].equals("--help")) {
            Logger.error("Usage: fetch_resource <server_address> [port] [resource]");
            System.exit(1);
        }

        // set everything to their defaults
        String serverAddress = args[0];
        int port = DEFAULT_PORT;
        String resource = DEFAULT_RESOURCE;

        if (args.length == 2) {
            // if there's only 2 arguments, try to parse the second one
            // as a port, but if that fails, we'll accept it as a resource

            try {
                port = Integer.parseInt(args[1]);
            } catch (NumberFormatException e) {
                resource = args[1];
            }
        } else {
            // if there's more arguments, then we expect [1] to be a number
            // and [2] can be whatever

            try {
                port = Integer.parseInt(args[1]);
                resource = args[2];
            } catch (NumberFormatException e) {
                Logger.error("Invalid port! Expected a number, found: %s", port);
                System.exit(1);
            }
        }

        return new Request(serverAddress, port, resource);
    }

    public final String serverAddress;

    public final int port;

    public final String resource;

    public Request(String serverAddress, int port, String resource) {
        this.serverAddress = serverAddress;
        this.port = port;
        this.resource = resource;
    }

}
