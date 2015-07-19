#include <iostream>
#include <list>
#include <functional>
using namespace std;

template <class T>
struct A {
    int a;
    T t;
};

typedef void (*void_f_int)(int);

template<class Bt>
class B {
    A<Bt> _a;
public:
    B(A<Bt> a) : _a(a) {}
    void run() {
        _a.t(_a.a);
    }
};

void test1()
{
    A<void_f_int> a;
    a.a = 5;
    a.t = [](int a) { cout << a << endl; };
    B<void_f_int> b(a);
    b.run();
}

template<class T>
class C {
    list<T> _list;
public:
    void add(T t) {
        _list.push_back(t);
    }
    void run() {
        for (auto e : _list) {
            e();
        }
    }
};


void test2()
{
    int kk = 8;
    string tt = "shit";

    C<function<void()>> c;
    c.add([kk, tt](){
        cout << 1 << "->" << kk << "->" << tt << endl;
    });
    c.add([kk](){
        cout << 1 << "->" << kk << endl;
    });
    c.add([tt](){
        cout << 1 << "->" << tt << endl;
    });
    c.run();
    
}

function<int(int)> wrapper(function<void(int)> inf)
{
    auto ff = [&](int a) {
        cout << "wrapper " << a << endl;
        inf(a);
        return 1;
    };
    return ff;
}

void wrapped(int a)
{
    cout << "in wrapped " << a << endl;
}

void test3()
{
    auto aaf = wrapper([](int a){
        cout << "in wrapped" << a << endl;
    });
    aaf(999);
    auto bbf = wrapper([&](int b){
        cout << "in test3 lambda " << b << endl;
        aaf(1099);
    })(888);
}

int main()
{
    // test1();
    // test2();
    test3();
    
}