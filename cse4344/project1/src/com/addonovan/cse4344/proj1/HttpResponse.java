package com.addonovan.cse4344.proj1;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

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

    private Status status;

    private ResponseSource responseSource;

    public HttpResponse() {
        this(Status.Ok, new StringResponseSource(""));
    }

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
