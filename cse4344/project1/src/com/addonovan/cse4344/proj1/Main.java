package com.addonovan.cse4344.proj1;

import java.io.IOException;

public final class Main {

    public static void main(String[] args) throws IOException {
        Server server = new Server(8080);
        server.start();
    }

}
