package com.addonovan.cse4344.proj1;

import java.io.IOException;
import java.io.OutputStream;

/**
 * A source of data for an {@link HttpResponse} object to pull from.
 */
public interface ResponseSource {

    /**
     * @return The length of the source (in bytes).
     */
    long getLength();

    /**
     * @return The type of the source content.
     */
    String getContentType();

    /**
     * @param os
     *          The {@link OutputStream} which sends data to the client. This
     *          cannot be closed after the write operation is completed.
     * @throws IOException If writing to {@code os} encounters any problems.
     */
    void writeTo(OutputStream os) throws IOException;

}
