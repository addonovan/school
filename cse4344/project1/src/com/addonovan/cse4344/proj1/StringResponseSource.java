package com.addonovan.cse4344.proj1;

import java.io.IOException;
import java.io.OutputStream;

public class StringResponseSource implements ResponseSource {

    /** The content to respond with. */
    private final String content;

    private String contentType = "text/plain";

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
