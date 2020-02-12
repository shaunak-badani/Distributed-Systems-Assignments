import java.util.*;

public class test {
    public static void main(String[] args) {
        G_class g = new G_class();
        g.example_function();
        g.call_h();
        H h = new H(4);
        System.out.println(h.n);
    }
}