package com.addonovan.cse4344.proj1;

import java.io.*;

public final class HttpResponse {

    public enum Status {
        Ok(200, "OK"),
        BadRequest(400, "Bad Request"),
        NotFound(404, "Not Found"),
        InternalServerError(500, "Internal Server Error");

        final String code;

        final String message;

        Status(int code, String message) {
            this.code = Integer.toString(code);
            this.message = message;
        }
    }

    /** The status of this response */
    private Status status;

    /** The source for the content of this response. */
    private ResponseSource responseSource;

    /**
     * Creates a new {@code 200 OK} response with no body.
     */
    public HttpResponse() {
        this(Status.Ok, new StringResponseSource(""));
    }

    /**
     * @param status
     *          {@link #status}
     * @param source
     *          {@link #responseSource}
     */
    public HttpResponse(Status status, ResponseSource source) {
        this.status = status;
        this.responseSource = source;
    }

    public void setStatus(Status status) {
        this.status = status;
    }

    public void setResponseSource(ResponseSource responseSource) {
        this.responseSource = responseSource;
    }

    /**
     * Sends this response down the connection to the current client via
     * the {@code os}. This will not close {@code os}.
     *
     * @param os
     *          The connection to the client.
     * @throws IOException If writing to {@code os} produces any problems.
     */
    public void writeTo(OutputStream os) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));

        bw.write("HTTP/1.1 ");
        bw.write(status.code);
        bw.write(" ");
        bw.write(status.message);
        bw.write("\r\n");

        bw.write("Content-Length: ");
        bw.write(Long.toString(responseSource.getLength()));
        bw.write("\r\n");

        bw.write("Content-Type: ");
        bw.write(responseSource.getContentType());
        bw.write(";  charset=utf-8\r\n");

        // finish the headers
        bw.write("\r\n");
        bw.flush();

        // write the body
        responseSource.writeTo(os);

        os.flush();
    }
}
