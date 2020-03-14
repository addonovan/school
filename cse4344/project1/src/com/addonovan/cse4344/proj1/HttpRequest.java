package com.addonovan.cse4344.proj1;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public final class HttpRequest {

    public static HttpRequest from(InputStream is) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(is));

        HttpRequest request = new HttpRequest();

        // parse route
        String initialLine = br.readLine();
        String[] parts = initialLine.split(" ");
        request.method = Method.valueOf(parts[0]);
        request.path = parts[1];
        request.protocol = parts[2];

        // read headers
        while (true) {
            String line = br.readLine();
            if (line == null || line.isEmpty()) break;

            String[] split = line.split(":");

            request.headers.put(split[0], split[1].trim());
        }

        return request;
    }

    public enum Method {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
        HEAD
    }

    private Method method;

    private String path;

    private String protocol;

    private HashMap<String, String> headers = new HashMap<>();

    private HttpRequest() {}

    public Method getMethod() {
        return method;
    }

    public String getPath() {
        return path;
    }

    public String getProtocol() {
        return protocol;
    }

    public Map<String, String> getHeaders() {
        return Collections.unmodifiableMap(headers);
    }

}
