#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "binheader.h"

using namespace std;

MyHeaderReader::MyHeaderReader(std::string n, uint64_t length)
    : m_path(n)
{
    rd.open(n,ios_base::in | ios_base::binary);
    if (!rd.is_open()) {
        std::cerr
        << "Error: could no open file for reading."
        << std::endl;
        exit(1);
    }

    code_libs   .clear();
    code_exports.clear();
    
    codelen = length;
    
    read_header  ();
    read_library ();
    read_exports ();
    read_image   ();
}

// header
void MyHeaderReader::read_header()
{
    uint32_t magic   = 0;
    uint32_t version = 0;
    uint16_t nsize   = 0;
    uint16_t lib_id  = 0;
  
    rd >> magic  ;
    rd >> version;
    rd >> nsize  ;

    char*   name = new char[nsize];
    rd.read(name,nsize);
    name[nsize] =  '\0';  // string terminator
    
    std::cout << "magic:   " << std::hex << magic   << std::endl;
    std::cout << "version: " << std::hex << version << std::endl;
    
    std::cout << "nsize:   " << std::dec << nsize   << std::endl;
    std::cout << "name:    " << name                << std::endl;
    
    rd >> lib_id ;
    rd >> codebeg;
    rd >> codelen;
    
    std::cout << "libid:   0x" << std::hex << lib_id  << std::endl;
    std::cout << "codebeg: 0x" << std::hex << codebeg << std::endl;
    std::cout << "codelen: 0x" << std::hex << codelen << std::endl;
}

// libs
void MyHeaderReader::read_library()
{
    uint16_t length;
    uint16_t id;
    uint32_t libs;

    rd >> libs;
    std::cout << "libsr:  " << libs << std::endl;
    
    for (uint32_t i = 0; i < libs; i++) {
        rd >> length;
        rd >> id    ;
        
        char *name = new char[length];
        rd.read(name,length);
        name[length] =  '\0';  // string terminator
        
        code_libs.emplace_back(
            static_cast<uint16_t>(length),
            static_cast<uint16_t>(id),
            std::string(name));
        
        std::cout << "nameovl: " << name   << std::endl;
        std::cout << "length:  " << length << std::endl;
        std::cout << "ovlid:   " << id     << std::endl;
        
        delete name;
    }
}

// exports
void MyHeaderReader::read_exports()
{
    uint64_t funcs  = 0;
    
    uint16_t length = 0;
    uint64_t offset = 0;
    uint8_t  lib_id = 0;
    
    code_exports.clear();
    
    rd >> funcs;
    cout << "funclen: " << funcs << std::endl;

    for (int i = 0; i < funcs; i++) {
        rd >> length;
        rd >> offset;
        rd >> lib_id;

std::cout << "funzers: " << length << "\n";

        char *  name = new char[length];
        rd.read(name,length);
        name[length] =  '\0';  // string terminator
        
        code_exports.emplace_back(
        length,
        offset,
        lib_id, name);
        
        std::cout << "funname: " << name     <<           std::endl;
        std::cout << "offset:  " << std::hex << offset << std::endl;
        std::cout << "id:      " << std::dec << 0 + lib_id << std::endl;
        
        delete name;
    }
}

// image
void MyHeaderReader::read_image()
{
    image = new uint8_t[codelen];
    rd.seekg(codebeg,ios_base::beg);
    rd.read(reinterpret_cast<char*>(image),codelen);
    
    std::cout << "codeBEG: " << std::hex << codebeg << std::endl;
    std::cout << "codeLEN: " << std::hex << codelen << std::endl;
}

void MyHeaderReader::call(std::string wfun)
{
    uint64_t position = 0;
    bool     found    = false;
    
    for (int i = 0; i < code_exports.size(); i++) {
        if (code_exports.at(i).name == wfun) {
            found    = true;
            position = code_exports.at(i).offset;
            break;
        }
    }
    
    if (!found) {
        std::cout
        << "error: could not found entry point."
        << std::endl;
        exit(2);
    }

    std::cout << "start...\n";
    vmExec(image,codelen);
    
    std::cout << "ferdsch\n";    
    exit(EXIT_SUCCESS);
}
