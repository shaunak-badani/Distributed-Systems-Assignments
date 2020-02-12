import java.util.*;

public class G_class implements G {
    
    public void example_function() {
        System.out.println("Called");
    }
    public void call_h() {
        H h = new H(100);
        System.out.println(h.n + " from call_h");
    }
}