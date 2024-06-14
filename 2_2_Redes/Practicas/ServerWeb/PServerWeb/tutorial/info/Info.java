package es.udc.redes.tutorial.info;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;
public class Info {

    public static String nombreArchivo(Path path){
        String nombre = path.getFileName().toString();
        int pospunto = nombre.lastIndexOf('.');
        if (pospunto == -1)
            throw new IllegalArgumentException("Error al obtener el nombre del archivo (Puede deberse a no tener extensión)");
        else
            return nombre.substring(0, pospunto);
    }
    public static String sacarExtension(Path path){
        String nombre = path.getFileName().toString();
        int pospunto = nombre.lastIndexOf('.');
        if (pospunto == -1 || pospunto == nombre.length() - 1)
            throw new IllegalArgumentException("Error al obtener la extensión (Puede deberse a que no tiene)");
        return nombre.substring(pospunto + 1);
    }
    public static String sacarTipo(Path path) {
        if (Files.isDirectory(path)) {
            return "directory";
        } else if (Files.isRegularFile(path)) {
            String extension = sacarExtension(path);
            return switch (extension.toLowerCase()) {
                case "txt", "docx", "pdf", "csv", "bin" -> "text";
                case "jpeg", "jpg", "png", "gif", "bmp", "tiff" -> "image";
                default -> "unknown";
            };
        }
        return "unknown";
    }
    public static void main(String[] args) throws IOException {
        try {
            if (args[0] == null)
                throw new IllegalArgumentException();

            Path path = Paths.get(args[0]);

            boolean exists = Files.exists(path);
            if(!exists)
                throw new IllegalArgumentException("Archivo no encontrado");

            System.out.println("Tamaño - " + Files.size(path) + "B");
            System.out.println("Última modificación - " + Files.getLastModifiedTime(path));
            System.out.println("Nombre del archivo - " + nombreArchivo(path));
            System.out.println("Extensión - " + sacarExtension(path));
            System.out.println("Tipo - " + sacarTipo(path));
            System.out.println("Ruta absoluta - " + path.toAbsolutePath());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
