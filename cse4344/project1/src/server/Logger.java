package server;

/**
 * @author Austin Donovan - UTA 1001311620
 */
public final class Logger {

    public static void info(String format, Object... params) {
        synchronized (System.out) {
            Thread currentThread = Thread.currentThread();
            System.out.printf("[INFO] [%s|%d] ", currentThread.getName(), currentThread.getId());
            System.out.printf(format, params);
            System.out.println();
            System.out.flush();
        }
    }

    public static void warn(String format, Object... params) {
        synchronized (System.out) {
            Thread currentThread = Thread.currentThread();
            System.out.printf("[WARN] [%s|%d] ", currentThread.getName(), currentThread.getId());
            System.out.printf(format, params);
            System.out.println();
            System.out.flush();
        }
    }

    public static void error(String format, Object... params) {
        synchronized (System.out) {
            Thread currentThread = Thread.currentThread();
            System.err.printf("[ERROR] [%s|%d] ", currentThread.getName(), currentThread.getId());
            System.err.printf(format, params);
            System.err.println();
            System.err.flush();
        }
    }

}
