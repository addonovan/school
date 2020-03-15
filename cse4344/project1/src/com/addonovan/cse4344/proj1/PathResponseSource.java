package com.addonovan.cse4344.proj1;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.WritableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

/**
 * A {@link ResponseSource} indicating that the given content is a
 * file on the disk, which should be transferred directly into the
 * output socket, instead of first being read into memory.
 */
public class PathResponseSource implements ResponseSource {

    /** The path to the resource */
    private final Path path;

    /** The length of the content */
    private final long length;

    /** The MIME-type of the file (or application/octet-stream if none). */
    private final String contentType;

    /**
     * @param path
     *          The path to the resource which will be the body of the response.
     */
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
