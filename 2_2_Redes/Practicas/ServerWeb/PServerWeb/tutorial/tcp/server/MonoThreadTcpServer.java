package es.udc.redes.tutorial.tcp.server;

import java.net.*;
import java.io.*;

/**
 * MonoThread TCP echo server.
 */
public class MonoThreadTcpServer {

    public static void main(String argv[]) {
        if (argv.length != 1) {
            System.err.println("Format: es.udc.redes.tutorial.tcp.server.MonoThreadTcpServer <port>");
            System.exit(-1);
        }
        ServerSocket svsocket = null;
        try {
            // Create a server socket
            int puerto = Integer.parseInt(argv[0]);
            svsocket = new ServerSocket(puerto);
            // Set a timeout of 300 secs
            svsocket.setSoTimeout(300000);

            while (true) {
                // Wait for connections
                Socket socketCliente = svsocket.accept();
                // Set the input channel
                BufferedReader sInput = new BufferedReader(new InputStreamReader(socketCliente.getInputStream()));
                // Set the output channel
                PrintWriter sOutput = new PrintWriter(socketCliente.getOutputStream(), true);
                // Receive the client message
                String mensajeCliente = sInput.readLine();
                System.out.println("SERVER: Received " + mensajeCliente + " from " + socketCliente.getInetAddress() + ":" + socketCliente.getPort());
                // Send response to the client
                System.out.println("SERVER: Sending " + mensajeCliente + " to " + socketCliente.getInetAddress() + ":" + socketCliente.getPort());
                sOutput.println(mensajeCliente);
                // Close the streams
                sOutput.close();
                sInput.close();
                socketCliente.close();
            }
        // Uncomment next catch clause after implementing the logic            
        } catch (SocketTimeoutException e) {
            System.err.println("Nothing received in 300 secs ");
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        } finally {
            try {
                svsocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
