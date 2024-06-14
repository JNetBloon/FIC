package es.udc.redes.tutorial.udp.server;

import java.net.*;

/**
 * Implements a UDP echo server.
 */
public class UdpServer {

    public static void main(String argv[]) {
        if (argv.length != 1) {
            System.err.println("Format: es.udc.redes.tutorial.udp.server.UdpServer <port_number>");
            System.exit(-1);
        }
        DatagramSocket socket = null;
        try {
            int puerto = Integer.parseInt(argv[0]);
            socket = new DatagramSocket(puerto);
            byte[] buffer = new byte[256];
            // Set maximum timeout to 300 secs
            socket.setSoTimeout(300000);

            while (true) {
                // Prepare datagram for reception
                DatagramPacket paquete = new DatagramPacket(buffer, buffer.length);

                // Receive the message
                socket.receive(paquete);

                // Prepare datagram to send response
                String mensajeCliente = new String(paquete.getData(), 0, paquete.getLength());
                InetAddress direccionCliente = paquete.getAddress();
                int puertoCliente = paquete.getPort();
                System.out.println("SERVER: Received " + mensajeCliente + " from " + direccionCliente + ":" + puertoCliente);

                // Send response
                byte[] bufferRespuesta = mensajeCliente.getBytes();
                DatagramPacket paqueteEnviar = new DatagramPacket(bufferRespuesta, bufferRespuesta.length, direccionCliente, puertoCliente);
                socket.send(paqueteEnviar);
                System.out.println("SERVER: Sending " + mensajeCliente + " to " + direccionCliente + ":" + puertoCliente);
            }

            // Uncomment next catch clause after implementing the logic
        } catch (SocketTimeoutException e) {
            System.err.println("No requests received in 300 secs ");
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        } finally {
            // Close the socket
            socket.close();
        }
    }
}
