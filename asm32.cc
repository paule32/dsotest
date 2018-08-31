#include "binheader.h"

using namespace std;
using namespace asmjit;
using namespace asmtk;

void my_func(int p) { std::printf("----> %d\n",p); }
void my_func2(void) {
    std::printf("Hello World !!!\n");
    std::printf("numser: %d\n", 1212); exit(2150);
}
static void emitCaller2(X86Emitter * e)
{
    const auto addr = reinterpret_cast<size_t>  (&my_func2);
    
    e->nop();
    e->push(x86::ebp);
    e->mov (x86::ebp, x86::esp);
    
    //e->mov (x86::rbx, 814);
    //e->mov (x86::rax, addr);
    
    //e->push(x86::rbx);
    //e->call(x86::rax);
    
    e->pop (x86::ebx);
    e->mov (x86::esp, x86::ebp);
    
    //e->leave();
    e->ret();
    e->nop();
}
static void emitCaller(X86Emitter * e)
{
    const auto addr = reinterpret_cast<uint64_t>(&my_func2);
   
    e->mov (x86::rax, addr);
    e->call(x86::rax);

    e->leave();
    e->ret();
}

extern "C" int coder_test(void (*funcer)())
{
    asmjit::JitRuntime rt;
    asmjit::CodeHolder code;
    
    // write code ...
    {
        code.init(rt.getCodeInfo());
        X86Compiler cc(&code);
        
        cc.nop();
        emitCaller2(cc.asEmitter());
        
        cc.nop();
        cc.finalize();
    }

    // save code ...
    {
        CodeBuffer & buffer = code.getSectionEntry(0)->getBuffer();
        uint64_t code_len   = buffer.getLength();
        
        std::cout << "bufferLen: 0x" << std::hex << code_len << std::endl;

        MyHeaderWriter hw("runtime.ovl",code_len,
        buffer.getData());
        hw.write_data();
        
        return 0;
    }
    
    {
        const std::string roverlay = "./runtime.ovl";
        MyHeaderReader imgrd(
        roverlay,
        code.getCodeSize());

        std::cout << "len:  " << imgrd.codelen << std::endl;
        std::cout << "next step..." << std::endl;
        
        //imgrd.call("main");
        
        
        //typedef void (*testers)();
        //auto v = reinterpret_cast<testers>(0x400916);
        //testers v = funcer;
        //std::printf("%p\n", v);
        //v();
    }
    return 0;
}
