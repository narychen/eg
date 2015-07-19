// Example program
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class T {
public:
    T(int a) {
        cout << a << endl;
        cout << this << endl;
    }
    
    T(const T& t) {
        cout << "copy = " << this << endl;
    }
    
    void f() {
        cout << "fffff" << endl;
    }
    
    ~T() {}
};

int main()
{
  vector<T> t(5, T(2));
  T t2(9);
  t2.f();

}
