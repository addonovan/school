package com.addonovan.cse4344.proj1;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

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

    private Path contentPath = null;

    private long contentSize = 0;

    private String contentType = "text/plain";

    public void setStatus(Status status) {
        this.status = status;
    }

    public void setContentPath(Path contentPath) {
        this.contentPath = contentPath;
        try {
            contentSize = Files.size(contentPath);

            // try to detect the content type
            contentType = Optional.ofNullable(Files.probeContentType(contentPath))
                    .orElse(contentType);
        } catch (Exception e) {
            contentSize = 0;
            this.status = Status.NotFound;
        }
    }

    public void writeTo(OutputStream os) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os));

        bw.write("HTTP/1.1 ");
        bw.write(status.code);
        bw.write(" ");
        bw.write(status.message);
        bw.write("\r\n");

        bw.write("Content-Length: ");
        bw.write(Long.toString(contentSize));
        bw.write("\r\n");

        bw.write("Content-Type: ");
        bw.write(contentType);
        bw.write(";  charset=utf-8\r\n");

        // finish the headers
        bw.write("\r\n");
        bw.flush();

        if (contentSize > 0) {
            Files.copy(contentPath, os);
        }

        os.flush();
    }
}
