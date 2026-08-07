import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Explicit Declarations
        float pi = 3.14159f;
        boolean isSuccess = true;

        System.out.println(pi);
        System.out.println(isSuccess);

        // User Input Test
        float myFloat;
        boolean myBool;

        myFloat = scanner.nextFloat();
        myBool = scanner.nextBoolean();

        System.out.println(myFloat);
        System.out.println(myBool);
    }
}