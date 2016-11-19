package com.seceve.cryptopals.challenge32;

/*
 * File name: App.java
 * Created by: WRH
 * Email: o@seceve.com
 * http://www.cryptopals.com/sets/4/challenges/32
 */

import java.lang.Thread;
import java.util.Deque;
import java.util.Map;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;

import io.undertow.Undertow;
import io.undertow.server.HttpHandler;
import io.undertow.server.HttpServerExchange;
import io.undertow.server.handlers.PathHandler;

public class App implements Runnable{
	
	private PathHandler pathHandler;
	private Undertow server;
	private Exception Exception;
	private String fileSignature;
	
	private String fileName = "test.png";
	private String key = "128c0dzb29x72uv1299";
	
    public App(){
    	try{
    		fileSignature = getSignature(fileName);
    	} catch (Exception e) {
    		System.out.println("ERROR: Could not find test.png!");
    		System.exit(0);
    	}
    	System.out.println(fileSignature);
    	pathHandler = new PathHandler();
    	pathHandler.addPrefixPath("/", new HttpHandler() {
			public void handleRequest(HttpServerExchange exchange) throws Exception {
				exchange.setStatusCode(500);
			}
		});
		pathHandler.addExactPath("/test", new HttpHandler() {
			public void handleRequest(HttpServerExchange exchange) throws Exception {
				Map<String, Deque<String>> queryMap = exchange.getQueryParameters();
				String file;
				String signature;
                // Use your "insecure_compare" function to verify the HMACs on incoming requests, and test that the whole contraption works. Return a 500 if the MAC is invalid, and a 200 if it's OK.
				try{
					file = queryMap.get("file").getFirst();
					signature = queryMap.get("signature").getFirst();
					if(!file.equals(fileName)) throw Exception;
					if(!InsecureCompare(signature,fileSignature)) throw Exception;
				} catch (Exception e){
					exchange.setStatusCode(500);
				}
			}
		});	
    }
    
    private boolean InsecureCompare(String s0, String s1) throws Exception{
    	for(int i=0; i < s0.length();i++){
    		if(!s0.substring(i, i+1).equals(s1.substring(i, i+1))) return false;
            // Reduce the sleep in your "insecure_compare" until your previous solution breaks. (Try 5ms to start.)
    		Thread.sleep(5);
    	}
    	return s0.length() == s1.length();
    }
    
    private String getSignature(String fileName) throws Exception{
    	BufferedInputStream in = new BufferedInputStream(
    			new FileInputStream(System.getProperty("user.dir")+'\\'+fileName));
    	ByteArrayOutputStream out = new ByteArrayOutputStream();
    	byte[] temp = new byte[1024];
    	int size;
    	while((size = in.read(temp)) != -1) out.write(temp,0,size);
    	byte[] fileBytes = out.toByteArray();
    	SecretKeySpec secretKeySpec = new SecretKeySpec(key.getBytes(),"HmacSHA1");
    	Mac mac = Mac.getInstance("HmacSHA1");
    	mac.init(secretKeySpec);
    	byte[] macBytes = mac.doFinal(fileBytes);
    	StringBuffer stringBuffer = new StringBuffer();
    	for(byte b : macBytes){
    		int i = b & 0xff;
    		String s = Integer.toHexString(i);
    		stringBuffer.append(s.length()<2 ? '0'+s:s);
    	}
    	return stringBuffer.toString();
    }
    
	public void run() {
        // HTTP Server
		server = Undertow.builder().addHttpListener(80, "localhost").setHandler(pathHandler).build();
		server.start();
	}
	
	public static void main( String[] args ){
        Thread app = new Thread(new App());
        app.start();
    }
}
