// #include <iostream>
// #include <memory>
// using namespace std;
// std::weak_ptr<int> gw;

// class Test;
// typedef shared_ptr<Test> sp_Test;
// typedef weak_ptr<Test> wp_Test;
// wp_Test tw;
// class Test
// {
// public:
//  //析构函数
//  ~Test() { std::cout << "Test Destructor." << std::endl; }
//  //获取指向当前对象的指针

//  void run() {
//   cout << "in run()" << endl;
//   show();
//   cout << "out run()" << endl;
//   // shit(shared_from_this());
//  }
 
//  string show() {
//   return "test show";
//  }
// };

// void f()
// {
//     cout << "enter f() " << endl;
//     if (auto spt = tw.lock()) { // Has to be copied into a shared_ptr before usage
// 	    spt->run();
//     }
//     else {
//         std::cout << "gw is expired\n";
//     }
//     cout << "leave f()" << endl;
// }
 
// int main()
// {
//     if (true) {
//         auto sp = make_shared<Test>();
// 	    tw = sp;
 
// 	f();
//     }
 
//     f();
// }


#include <iostream>
#include <memory>
using namespace std;
std::weak_ptr<int> gw;

class Test;
typedef shared_ptr<Test> sp_Test;
typedef weak_ptr<Test> wp_Test;
wp_Test tw;
class Test
{
public:
 //析构函数
 ~Test() { std::cout << "Test Destructor." << std::endl; }
 //获取指向当前对象的指针

 void run() {
  cout << "in run()" << endl;
  show();
  cout << "out run()" << endl;
  // shit(shared_from_this());
 }
 
 string show() {
   return "test show";
 }
};

void f()
{
    cout << "enter f() " << endl;
    sp_Test sww(tw);
    if (auto spt = sww) { // Has to be copied into a shared_ptr before usage
	    spt->run();
    }
    else {
        std::cout << "gw is expired\n";
    }
    cout << "leave f()" << endl;
}
 
int main()
{
    if (true) {
        auto sp = make_shared<Test>();
	    tw = sp;
 
	f();
    }
 
    f();
}