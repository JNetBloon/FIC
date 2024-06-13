package es.udc.redes.tutorial.copy;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class Copy {

    public static void main(String[] args) throws IOException {
        if (args[0] == null || args[1] == null)
            throw new IllegalArgumentException();

        FileInputStream in = null;
        FileOutputStream out = null;

        try {
            in = new FileInputStream(args[0]);
            out = new FileOutputStream(args[1]);
            int c;

            while ((c = in.read()) != -1) {
                out.write(c);
            }
            System.out.println("Copiado terminado");

        } finally {
            if (in != null) {
                in.close();
            }
            if (out != null) {
                out.close();
            }
        }

    }
    
}
