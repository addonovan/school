package client;

public final class Main {

    public static void main(String[] args) {
        final Request request = Request.fromArgs(args);

        try {
            Logger.info("Sending request");
            String body = Connection.send(request);
            Logger.info("Request complete");

            System.out.println(body);
        } catch (Exception e) {
            Logger.error("Connection failed!");
            e.printStackTrace();
        }
    }

}
