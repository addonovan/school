package com.addonovan.cse4344.proj1;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

public final class HttpResponse {

    public enum Status {
        Ok(200, "OK"),
        NotFound(404, "Not Found"),
        InternalServerError(500, "Internal Server Error");

        final String code;

        final String message;

        Status(int code, String message) {
            this.code = Integer.toString(code);
            this.message = message;
        }
    }

    private Status status = Status.Ok;

    private long contentLength = 0;

    public void setStatus(Status status) {
        this.status = status;
    }

    public void setContentLength(long contentLength) {
        this.contentLength = contentLength;
    }

    public void writeTo(OutputStream os) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));

        bw.write("HTTP/1.1 ");
        bw.write(status.code);
        bw.write(" ");
        bw.write(status.message);
        bw.write("\r\n");

        bw.write("Content-Length: ");
        bw.write(Long.toString(contentLength));
        bw.write("\r\n");

        bw.write("Content-Type: text/plain; charset=utf-8\r\n");

        bw.write("\r\n");

        // write the headers, then the rest of the contents
        bw.flush();
    }
}
