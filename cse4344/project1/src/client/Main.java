package client;

/**
 * @author Austin Donovan - UTA 1001311620
 */
public final class Main {

    public static void main(String[] args) {
        final Request request = Request.fromArgs(args);

        try {
            Logger.info("Sending request");
            Logger.info(
                    "\tGET %s %s:%d",
                    request.resource,
                    request.serverAddress,
                    request.port
            );
            String body = Connection.send(request);
            Logger.info("Request complete");

            Logger.info("Body:%n%s", body);
        } catch (Exception e) {
            Logger.error("Connection failed!");
            e.printStackTrace();
        }
    }

}
