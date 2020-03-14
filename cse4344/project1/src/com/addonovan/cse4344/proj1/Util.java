package com.addonovan.cse4344.proj1;

import java.io.PrintWriter;
import java.io.StringWriter;

public final class Util {

    private Util() {

    }

    public static String getStackTrace(Exception e) {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        e.printStackTrace(pw);
        return sw.toString();
    }

}
