#include <set>
#include <iostream>
int main()
{
    std::set<int> c = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    // erase all odd numbers from c
    for(auto it = c.begin(); it != c.end(); )
        if(*it % 2 == 1)
            it = c.erase(it);
        else
            ++it;
    for(int n : c)
        std::cout << n << ' ';
    std::cout << std::endl;
    auto it = c.find(4);
    if(it != c.end()) {
        std::cout << *it << std::endl;
    }
}