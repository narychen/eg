#include <list>
#include <iostream>

using namespace std;
 
class LambdaTest {
public:
    void lambda_call(int a, int b, T c) {
        c(a, b);
    }
}
 
void test1()
{
    std::list<int> l = { 1,100,2,3,10,1,11,-1,12 };
 
    l.remove(1); // remove both elements equal to 1
    l.remove_if([](int n){ 
        if (n > 10) {
            return false;
        } else {
            return true;
        }
    }); // remove all elements greater than 10
 
    for (int n : l) {
        std::cout << n << ' '; 
    }
    std::cout << '\n';   
}

template< class T >
void lambda_call(int a, int b, T c)
{
    c(a, b);
}

void test2()
{
    lambda_call(1, 2, [](int a, int b, int k=5) {
        int c = a + b;
        cout << c << endl;
        cout << k << endl;
    });
}

void test3()
{
    
    
}

int main()
{
    test1();
    test2();
    test3();
}