package com.addonovan.cse4344.proj1;

import java.io.PrintWriter;
import java.io.StringWriter;

public final class Util {

    private Util() {}

    /**
     * @return
     *      The stack trace of the given {@link Throwable} as a
     *      {@link String}, so that it can be logged more nicely.
     */
    public static String getStackTrace(Throwable t) {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        t.printStackTrace(pw);
        return sw.toString();
    }

}
