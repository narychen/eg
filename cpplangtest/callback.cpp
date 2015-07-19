#include <iostream>

using namespace std;

class A;
typedef void (*f)(A* a);

class A {
    int _a;
public:
    A(int a) : _a(a) {}
    template<class F>
    void call(F cb) {
        cb(this);
    }
    template<class F>
    void call2(F cb) {
        cb(_a);
    }
    int GetA() { return _a; }
};

void test1()
{
    A* a = new A(999);
    a->call([&a](A* p) {
        cout << p->GetA() << endl;
        cout << a->GetA() << endl;
    });
    
    a->call2([](int a) {
        cout << "_a = " << a << endl;    
    });
}


int main()
{
    test1();
}