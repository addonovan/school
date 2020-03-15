package com.addonovan.cse4344.proj1;

public final class Logger {

    /**
     * Logs an informational message.
     *
     * @param format
     *          The output formatter.
     * @param params
     *          Any parameters to the string format.
     */
    public static void info(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[INFO] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

    /**
     * Logs an informational message.
     *
     * @param format
     *          The output formatter.
     * @param params
     *          Any parameters to the string format.
     */
    public static void warn(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[WARN] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

    /**
     * Logs an informational message.
     *
     * @param format
     *          The output formatter.
     * @param params
     *          Any parameters to the string format.
     */
    public static void error(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[ERROR] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

}
