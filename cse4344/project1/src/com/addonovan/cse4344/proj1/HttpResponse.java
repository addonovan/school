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

    public Status status = Status.Ok;

    public String content = "";

    public void writeTo(OutputStream os) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));

        bw.write("HTTP/1.1 ");
        bw.write(status.code);
        bw.write(" ");
        bw.write(status.message);
        bw.write("\n");

        bw.write("Content-Length: ");
        bw.write(Integer.toString(content.length()));
        bw.write("\n");

        bw.write("Content-Type: text/plain; charset=utf-8\n");

        bw.write("\n");

        bw.write(content);

        bw.flush();
    }
}
