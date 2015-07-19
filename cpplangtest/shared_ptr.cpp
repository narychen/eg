#include <iostream>
#include <memory>

using namespace std;
class Test;
typedef shared_ptr<Test> sp_Test;
void shit(Test* p);

class Test
{
public:
 //析构函数
 ~Test() { std::cout << "Test Destructor." << std::endl; }
 //获取指向当前对象的指针

 void run() {
  cout << "in run()" << endl;
  shit(this);
  cout << "out run()" << endl;
  // shit(shared_from_this());
 }
 
 string show() {
   return "test show";
 }
};

void shit(Test* p) {
 cout << p->show() << endl;
 // delete p;
}

int main(int argc, char *argv[])
{
  sp_Test p(new Test());
  sp_Test q = p;
  q->run();
  cout << "finish run()" << endl;
  return 0;
}