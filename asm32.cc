#include "binheader.h"

using namespace std;
using namespace asmjit;
using namespace asmtk;

void my_func(int p) { std::printf("----> %d\n",p); }
void my_func2(void) {
    std::printf("Hello World !!!\n");
    std::printf("numser: %d\n", 1212); exit(2150);
}

void MyCodeEmitter::emitTest()
{
    const auto addr = reinterpret_cast<size_t>  (&my_func2);
    
    x86_emitter->nop();
    x86_emitter->push(x86::rbp);
    x86_emitter->mov (x86::rbp, x86::rsp);
    
    x86_emitter->mov (x86::rbx, 814);
    x86_emitter->mov (x86::rax, addr);
    
    x86_emitter->push(x86::rbx);
    x86_emitter->call(x86::rax);
    x86_emitter->pop (x86::rbx);
    
    x86_emitter->mov (x86::rsp, x86::rbp);
    x86_emitter->ret();
    x86_emitter->nop();
}

MyCodeEmitter::MyCodeEmitter(std::string _name)
{
    lib_name = _name;
    
    // write code ...
    x86_codeholder.init(x86_runtime.getCodeInfo());
    
    x86_codeholder.attach(&x86_compiler);
    x86_emitter = x86_compiler.asEmitter();
    
    emitTest();
}

void MyCodeEmitter::write()
{
    x86_compiler.finalize();

    // save code ...
    CodeBuffer & buffer = x86_codeholder.getSectionEntry(0)->getBuffer();
    uint64_t code_len   = buffer.getLength();

    MyHeaderWriter hw(lib_name,code_len,
    buffer.getData());
    hw.write_data();
    
    x86_codeholder.detach(&x86_compiler);
}
