package server;

import java.io.*;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.Scanner;
import java.util.function.Consumer;

/**
 * @author Austin Donovan - UTA 1001311620
 */
public final class ClientHandler implements Runnable {

    private final Socket socket;

    private final Scanner input;

    private final OutputStream output;

    public ClientHandler(Socket socket) throws IOException {
        this.socket = socket;
        this.input = new Scanner(socket.getInputStream());
        this.output = socket.getOutputStream();
    }

    @Override
    public void run() {
        Logger.info("Worker thread started");
        Logger.info("Handling connection from %s on port %d",
                socket.getInetAddress(),
                socket.getLocalPort()
        );

        try (socket) {
            handle();
            output.flush();
        } catch (Exception e) {
            Logger.error(
                    "Encountered unhandled exception when processing request: %n%s",
                    Util.getStackTrace(e)
            );
        }

        Logger.info("Worker thread exiting");
    }

    private void handle() throws IOException {
        Path resource = Paths.get(".", getRequestedFile());
        Logger.info("Fetching %s", resource.toString());

        processHeaders(key -> Logger.info("HEADER %s", key));

        sendResponse(resource);
    }

    private String getRequestedFile() {
        final String method = input.next(); // assume this is GET
        final String resource = input.next();
        final String protocol = input.nextLine(); // move the cursor forward to the end of the line

        Logger.info("%s %s %s", method, resource, protocol);

        return resource;
    }

    private void processHeaders(Consumer<String> onEachHeader) {
        while (input.hasNextLine()) {
            final String line = input.nextLine();

            // empty/blank line signifies the end of the headers
            if (line.isBlank()) break;

            onEachHeader.accept(line);
        }
    }

    private void sendResponse(Path resource) throws IOException {
        OutputStreamWriter osw = new OutputStreamWriter(output);
        if (Files.notExists(resource) || !Files.isRegularFile(resource)) {
            Logger.info("Resource does not exist or is inaccessible");

            osw.write("HTTP/1.0 404 Not Found\r\n");
            osw.write("Content-Length: 10\r\n");
            osw.write("\r\n");
            osw.write("Not Found\n");
            osw.flush();
        } else {
            Logger.info("Responding with resource");

            osw.write("HTTP/1.0 200 OK\r\n");

            // write the content length
            osw.write("Content-Length: ");
            osw.write(Long.toString(Files.size(resource)));
            osw.write("\r\n");

            // write the content type and default to application/octet-stream
            osw.write("Content-Type: ");
            osw.write(Optional.ofNullable(Files.probeContentType(resource))
                    .orElse("application/octet-stream"));
            osw.write(" charset=utf-8\r\n");

            osw.write("\r\n");
            osw.flush();

            // copy the resource straight into the body
            Files.copy(resource, output);
        }
    }
}
