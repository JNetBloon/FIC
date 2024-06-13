package es.udc.redes.webserver;

import java.net.*;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.*;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Locale;


public class ServerThread extends Thread {

    private Socket socket;

    public ServerThread(Socket s) {
        // Store the socket s
        this.socket = s;
    }

    private String obtenerDate() {
        LocalDate date = LocalDate.now();
        LocalDateTime localDateTime = LocalDateTime.now();
        ZonedDateTime gmtDateTime = localDateTime.atZone(ZoneId.of("GMT"));

        String dateLocal;
        try {
            DateTimeFormatter format = DateTimeFormatter.ofPattern("E, d MMM yyyy HH:mm:ss 'GMT'", Locale.ENGLISH);
            dateLocal = gmtDateTime.format(format);
            //System.out.println(dateLocal);
        } catch (DateTimeException exc) {
            // Manejar una excepción de formato de fecha y hora
            System.out.printf("%s can't be formatted!%n", date);
            throw exc;
        }
        return dateLocal;
    }
    private void noValido() {
        try {
            PrintWriter output = new PrintWriter(socket.getOutputStream(), true);

            // Enviar cabecera HTTP
            output.println("HTTP/1.0 400 Bad Request");
            String dateLocal = obtenerDate();
            output.println("Date: " + dateLocal);
            output.println(); // Fin de la cabecera

            output.close();
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }

    private void paginaCargada() {
        try {
            PrintWriter output = new PrintWriter(socket.getOutputStream(), true);
            output.println("HTTP/1.0 304 Not Modified");
            String dateLocal = obtenerDate();
            output.println("Date: " + dateLocal);
            output.println();

            output.close();
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        }
    }

    public static String sacarExtension(Path path){
        String nombre = path.getFileName().toString();
        int pospunto = nombre.lastIndexOf('.');
        if (pospunto == -1 || pospunto == nombre.length() - 1)
            throw new IllegalArgumentException("Error al obtener la extensión (Puede deberse a que no tiene)");
        return nombre.substring(pospunto + 1);
    }

    public void run() {
        try {
            // This code processes HTTP requests and generates
            // HTTP responses
            // Uncomment next catch clause after implementing the logic
            //

            BufferedReader sInput = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            // Petición HTML
            ArrayList<String> lecturaInput = new ArrayList<>();
            String lectura;
            while ((lectura = sInput.readLine()) != null && !lectura.isEmpty()) {
                lecturaInput.add(lectura);
            }

            if (lecturaInput.isEmpty() || lecturaInput.get(0) == null) {
                noValido();
                return;
            }

            String[] datosInput = lecturaInput.get(0).split(" ");

            //Ver si es un GET
            boolean optionHEAD = false;
            if (!datosInput[0].equals("GET")) {
                if(!datosInput[0].equals("HEAD")){
                    noValido();
                    return;
                } else {
                    optionHEAD = true;
                }
            }

            //Buscar el fichero solicitado
            String estado;
            String directorioActual = System.getProperty("user.dir");
            String rutaArchivo = directorioActual + File.separator + "p1-files" + File.separator + datosInput[1];
            File file = new File(rutaArchivo);

            if (!file.exists()) { //Si no existe devuele el de error
                directorioActual = System.getProperty("user.dir");
                rutaArchivo = directorioActual + File.separator + "p1-files" + File.separator + "error404.html";
                file = new File(rutaArchivo);
                estado = "HTTP/1.0 404 Not Found";
            } else { //Si existe, sabemos que el estado es correcto
                estado = "HTTP/1.0 200 OK";
            }

            //Imprime el mensaje recibido
            System.out.println("Received [\n");
            for (String linea : lecturaInput)
                System.out.println(linea);
            System.out.println("\n]from " + socket.getInetAddress() + ":" + socket.getPort());

            //->->->Respuesta<-<-<-

            //DATE
            String dateLocal = obtenerDate();

            //SERVER
            String server = "Mapache/6.2.0 (Unix)";

            //LAST-MODIFIED
            for (String linea : lecturaInput) {
                if (linea.startsWith("If-Modified-Since:")) {
                    String fecha = linea.substring(linea.indexOf(":") + 1).trim(); //Guarda la fecha de la solicitud
                    ZonedDateTime fechaSOLICITUD = ZonedDateTime.parse(fecha, DateTimeFormatter.RFC_1123_DATE_TIME); //La pasa a un formato especifico
                    ZonedDateTime fechaARCHIVO = ZonedDateTime.ofInstant(Instant.ofEpochMilli(file.lastModified()), ZoneId.of("GMT")); //Guarda la fecha del archivo

                    if (fechaARCHIVO.isBefore(fechaSOLICITUD)) { //Compara
                        paginaCargada();
                        return;
                    }
                }
            }

            //CONTENT-LENGTH
            long conLen = Files.size(file.toPath());

            //CONTENT-TYPE  -> INVESTIGAR FUNCION QUE LO HACE POR TI
            String extension = sacarExtension(file.toPath());
            String mime = switch (extension) {
                case "html" -> "text/html";
                case "txt" -> "text/plain";
                case "gif" -> "image/gif";
                case "png" -> "image/png";
                default -> "application/octet-stream";
            };

            //->->->ENVIAR PÁGINA<-<-<-
            PrintWriter sOutput = new PrintWriter(socket.getOutputStream(), true);
            sOutput.println(estado);
            sOutput.println("Date: " + dateLocal);
            sOutput.println(server);
            sOutput.println("Last-Modified: " + obtenerDate());
            sOutput.println("Content-Length: " + conLen);
            sOutput.println("Content-Type: " + mime);
            //Linea en blanco
            sOutput.println();
            //Archivo
            if(!optionHEAD){
                FileInputStream fileInputStream = new FileInputStream(file);
                BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(socket.getOutputStream());
                byte[] buffer = new byte[(int) conLen]; // Crear un buffer del tamaño de la longitud del archivo
                int bytesRead;
                while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                    bufferedOutputStream.write(buffer, 0, bytesRead); // Escribir en el BufferedOutputStream
                }
                bufferedOutputStream.flush(); // Asegurarse de que todos los bytes se envíen
                fileInputStream.close();
            }

            sOutput.close();
            sInput.close();

        } catch (SocketTimeoutException e) {
            System.err.println("Nothing received in 300 secs");
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}