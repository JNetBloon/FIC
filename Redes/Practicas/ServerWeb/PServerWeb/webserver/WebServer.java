package es.udc.redes.webserver;

import es.udc.redes.tutorial.tcp.server.ServerThread;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class WebServer {
    
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Format: es.udc.redes.tutorial.tcp.server.TcpServer <port>");
            System.exit(-1);
        }
        ServerSocket svsocket = null;
        try {
            int puerto = Integer.parseInt(args[0]);
            svsocket = new ServerSocket(puerto);
            // Set a timeout of 300 secs
            svsocket.setSoTimeout(300000);
            while (true) {
                // Wait for connections
                Socket socketCliente = svsocket.accept();
                // Create a ServerThread object, with the new connection as parameter
                es.udc.redes.webserver.ServerThread SvTh = new es.udc.redes.webserver.ServerThread(socketCliente);
                // Initiate thread using the start() method
                SvTh.start();
            }
            // Uncomment next catch clause after implementing the logic
        } catch (SocketTimeoutException e) {
            System.err.println("Nothing received in 300 secs");
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        } finally{
            try {
                svsocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
