package com.addonovan.cse4344.proj1;

import java.io.IOException;
import java.io.OutputStream;

/**
 * A {@link ResponseSource} that is sourced by an in-memory string.
 */
public class StringResponseSource implements ResponseSource {

    /** The content to respond with. */
    private final String content;

    /** The content type of the response. Default is {@code "text/plain"} */
    private String contentType = "text/plain";

    /**
     * @param content
     *          {@link #content}
     */
    public StringResponseSource(String content) {
        this.content = content;
    }

    @Override
    public long getLength() {
        return content.length();
    }

    public void setContentType(String contentType) {
        this.contentType = contentType;
    }

    @Override
    public String getContentType() {
        return contentType;
    }

    @Override
    public void writeTo(OutputStream os) throws IOException {
        os.write(content.getBytes());
    }
}
