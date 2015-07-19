#include <iostream>
#include <sstream>
int main()
{
    // default constructor (input/output stream)
    std::stringstream buf1;
    buf1 << 7 << "xxdd";
    int n = 0;
    buf1 >> n;
    std::string n2;
    buf1 >> n2;
    std::cout << "buf1 = " << buf1.str() << " n = " << n << " n2 = " << n2 << std::endl;
 
    // input stream
    std::istringstream inbuf("-10");
    inbuf >> n;
    std::cout << "n = " << n << '\n';
 
    // output stream in append mode (C++11)
    std::ostringstream buf2("test", std::ios_base::ate);
    buf2 << '1';
    std::cout << buf2.str() << '\n';
}