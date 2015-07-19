
#include <iostream>
using namespace std;

void f(int i)
{
    cout << "in f() i = " << i << endl;
    i = 0;
}

int main()
{
    int i = 5;
    f(i);
    cout << "after call i = " << i << endl;
}