package com.addonovan.cse4344.proj1;

public final class Logger {

    public static void info(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[INFO] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

    public static void warn(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[WARN] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

    public static void error(String format, Object... params) {
        Thread thread = Thread.currentThread();
        synchronized (System.out) {
            System.out.printf("[ERROR] (%s-%d) ", thread.getName(), thread.getId());
            System.out.printf(format, params);
            System.out.println();
        }
    }

}
