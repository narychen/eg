#include <iostream>
#include <functional>
using namespace std;

function<int(int)> wrapper(function<void(int)> f)
{
    auto ff = [f](int a) { //Cannot use & or &f cuz it will be destroyed at the end of the function resulting dangling
        cout << "in wrapper " << a << endl;
        f(a);
        return 1;
    };
    return ff;
}

int main()
{
    auto fa = wrapper([](int a){
        cout << "in fa " << a << endl;
    });
    fa(999);
    wrapper([&fa](int b){
        cout << "in anon " << b << endl;
        fa(998);
    })(997);
}