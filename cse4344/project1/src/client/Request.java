package client;

public final class Request {

    private static final int DEFAULT_PORT = 8080;

    private static final String DEFAULT_RESOURCE = "index.html";

    public static Request fromArgs(String[] args) {
        if (args.length == 0 || args[0].equals("--help")) {
            Logger.error("Usage: fetch_resource <server_address> [port] [resource]");
            System.exit(1);
        }

        String serverAddress = args[0];
        int port = DEFAULT_PORT;
        String resource = DEFAULT_RESOURCE;

        if (args.length == 2) {
            try {
                port = Integer.parseInt(args[1]);
            } catch (NumberFormatException e) {
                resource = args[1];
            }
        } else {
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
