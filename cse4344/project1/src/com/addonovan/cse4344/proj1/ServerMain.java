package com.addonovan.cse4344.proj1;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

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
        server.start(4, request -> {
            Logger.info("%s %s", request.getMethod(), request.getPath());
            request.getHeaders().forEach((key, value) -> {
                Logger.info("HEADER %s = %s", key, value);
            });

            HttpResponse response = new HttpResponse();

            Path path = Paths.get(".", request.getPath());

            if (Files.notExists(path)) {
                response.setStatus(HttpResponse.Status.NotFound);
                response.setResponseSource(new StringResponseSource("Not Found"));
            } else {
                response.setResponseSource(new PathResponseSource(path));
            }

            return response;
        });
    }

}
