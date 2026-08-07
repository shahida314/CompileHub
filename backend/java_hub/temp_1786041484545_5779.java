class Main {
    public static void main(String[] args) {
print ("enter value: "); 
        Scanner sc = new Scanner(System.in);
        float n;
        n = sc.nextInt();
        int sum = 0;
        int i;
    
        for (i = 1; i <= n; i++) {
            sum = sum + i;
        }
        print sum;
    }
}