# include <iostream>
using namespace std;

extern "C"  int coder_test(void (*)());

void testfunc(void)
{
    std::cout << "meine jütte\n";
}
int main(int argc, char **argv)
{
    std::cout << "coder test ..." << std::endl;
    coder_test(&testfunc);
    std::cout << "endler" << std::endl;
    std::exit(EXIT_SUCCESS);
}
