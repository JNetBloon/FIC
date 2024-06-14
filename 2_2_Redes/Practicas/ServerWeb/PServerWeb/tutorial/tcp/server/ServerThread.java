package es.udc.redes.tutorial.tcp.server;
import java.net.*;
import java.io.*;

/** Thread that processes an echo server connection. */

public class ServerThread extends Thread {

  private Socket socket;

  public ServerThread(Socket s) {
    // Store the socket s
    this.socket = s;
  }

  public void run() {
    try {
      socket.setSoTimeout(300000);

      // Set the input channel
      BufferedReader sInput = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      // Set the output channel
      PrintWriter sOutput = new PrintWriter(socket.getOutputStream(), true);
      // Receive the message from the client
      String mensajeCliente = sInput.readLine();
      System.out.println("SERVER: Received " + mensajeCliente + " from " + socket.getInetAddress() + ":" + socket.getPort());
      // Sent the echo message to the client
      System.out.println("SERVER: Sending " + mensajeCliente + " to " + socket.getInetAddress() + ":" + socket.getPort());
      sOutput.println(mensajeCliente);
      // Close the streams
      sOutput.close();
      sInput.close();
    // Uncomment next catch clause after implementing the logic
    } catch (SocketTimeoutException e) {
      System.err.println("Nothing received in 300 secs");
    } catch (Exception e) {
      System.err.println("Error: " + e.getMessage());
    } finally {
	// Close the socket
      try {
        socket.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
  }
}
