package client;

public final class Logger {

    public static void info(String format, Object... params) {
        synchronized (System.out) {
            System.out.print("[INFO] ");
            System.out.printf(format, params);
            System.out.println();
            System.out.flush();
        }
    }

    public static void warn(String format, Object... params) {
        synchronized (System.out) {
            System.out.print("[WARN] ");
            System.out.printf(format, params);
            System.out.println();
            System.out.flush();
        }
    }

    public static void error(String format, Object... params) {
        synchronized (System.out) {
            System.err.print("[ERROR] ");
            System.err.printf(format, params);
            System.err.println();
            System.err.flush();
        }
    }

}
