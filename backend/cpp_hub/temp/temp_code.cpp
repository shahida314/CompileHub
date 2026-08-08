#include <iostream>
using namespace std;

bool isEven(int num)
{
    return num % 2 == 0;
}

int main()
{
    int n;

    cout << "Enter a number: ";
    cin >> n;

    for(int i = 1; i <= n; i++)
    {
        if(isEven(i))
            cout << i << " is Even" << endl;
        else
            cout << i << " is Odd" << endl;
    }

    return 0;
}