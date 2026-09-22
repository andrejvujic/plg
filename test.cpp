#include <memory>
#include <iostream>

std::unique_ptr<int> foo()
{
    std::unique_ptr<int> bar = std::make_unique<int>(69);
    return bar;
}

int main()
{
    std::unique_ptr<int> bar = foo();
    std::cout << *bar << std::endl;
    return 0;
}