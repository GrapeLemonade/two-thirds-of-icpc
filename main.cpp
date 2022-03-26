#include <iostream>
#include <set>


int main()
{
    std::set<int> count({1, 2, 3, 4, 5, 7, 8, 9});
    std::cout << count.contains(1) << " " << count.contains(6);
    if (count.contains(1)) throw std::out_of_range("this is not supposed to be here");
}