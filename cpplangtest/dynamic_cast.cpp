#include <iostream>

using namespace std;

struct A {
    
};

class B {
    
};

void testab(A& a, B& b)
{
    A* aa = dynamic_cast<A*>(&b);
    if (aa) {
        cout << "bingo" << endl;
    } else {
        cout << "shit" << endl;
    }
}

int main()
{
    A a;
    B b;
    testab(a, b);
}