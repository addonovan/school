package com.addonovan.cse4344.proj1;

import java.io.IOException;
import java.io.OutputStream;

public interface ResponseSource {

    long getLength();

    String getContentType();

    void writeTo(OutputStream os) throws IOException;

}
