struct s
{
    int a;
    int b;
};

auto dos()
{
    return s{1, 2};
}

#include <iostream>
int main()
{
    int a = 2, b;
    auto s = dos();
    std::cout << a << " " << s.b << std::endl;
}