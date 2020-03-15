package com.addonovan.cse4344.proj1;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.WritableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.Optional;

public class PathResponseSource implements ResponseSource {

    private final Path path;

    private final long length;

    private final String contentType;

    public PathResponseSource(Path path) {
        this.path = path;

        long length;
        String contentType;
        try {
            length = Files.size(path);
            contentType = Optional.ofNullable(Files.probeContentType(path))
                    .orElse("application/octet-stream");
        } catch (Exception e) {
            length = 0;
            contentType = "application/octet-stream";
        }
        this.length = length;
        this.contentType = contentType;
    }

    @Override
    public long getLength() {
        return length;
    }

    @Override
    public String getContentType() {
        return contentType;
    }

    @Override
    public void writeTo(OutputStream os) throws IOException {
        try (final FileChannel input = FileChannel.open(path)) {
            final WritableByteChannel output = Channels.newChannel(os);
            input.transferTo(0, length, output);

            // make sure we don't accidentally close the output channel because
            // we don't want to close the output stream
        }
    }
}
