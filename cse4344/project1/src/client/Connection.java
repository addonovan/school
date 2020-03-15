package client;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.util.Scanner;

public final class Connection {

    public static String send(Request request) {
        final Socket socket;
        try {
            socket = sendRequest(request);
        } catch (IOException e) {
            throw new RuntimeException("Failed to open socket", e);
        }

        try {
            return receiveResponse(socket);
        } catch (IOException e) {
            throw new RuntimeException("Failed to read response", e);
        }
    }

    private static Socket sendRequest(Request request) throws IOException {
        final Socket socket = new Socket(request.serverAddress, request.port);

        // send the minimum HTTP request
        OutputStreamWriter osw = new OutputStreamWriter(socket.getOutputStream());
        osw.write("GET ");
        osw.write(request.resource);
        osw.write(" HTTP/1.0\r\n"); // HTTP/1.0 because that forces connections to be closed immediately

        osw.write("Connection: close\r\n"); // make extra sure other servers will close this immediately

        osw.flush();

        return socket;
    }

    private static String receiveResponse(Socket socket) throws IOException {
        final Scanner scanner = new Scanner(socket.getInputStream());

        Logger.info("Receiving response");
        Logger.info("\tprotocol = %s", scanner.next());
        Logger.info("\tstatus = %d (%s)", scanner.nextInt(), scanner.nextLine());

        readHeaders(scanner);
        return readBody(scanner);
    }

    private static void readHeaders(Scanner scanner) {
        while (scanner.hasNextLine()) {
            final String header = scanner.nextLine();

            // blank line signifies the end of headers
            if (header.isBlank()) break;

            Logger.info("\tHEADER %s", header);
        }
    }

    private static String readBody(Scanner scanner) {
        StringBuilder sb = new StringBuilder();

        while (scanner.hasNextLine()) {
            sb.append(scanner.nextLine()).append("\n");
        }

        while (scanner.hasNext()) {
            sb.append(scanner.next()).append(" ");
        }

        return sb.toString();
    }

}
