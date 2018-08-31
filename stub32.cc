# include <iostream>
using namespace std;

extern "C"  int coder_test(void (*)());

void testfunc(void)
{
    std::cout << "meine jütte\n";
}
int main(int argc, char **argv)
{
    coder_test(&testfunc);
    return 0;
}
