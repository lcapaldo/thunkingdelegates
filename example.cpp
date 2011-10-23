#include "AsmJit.h"
#include <iostream>

typedef void (*callback)(int);

struct A
{
   int m_accum;
   void doIt(int x)
   {
     m_accum += x;
     std::cout << "A: " << x << " " << m_accum << std::endl;
   }
};

void A_trampoline(A* a, int x)
{
  a->doIt(x);
}


void takes_callback(callback cb)
{
  cb(1);
  cb(2);
  cb(3);
}



int main()
{
  using namespace AsmJit; 
  A a = A();

  Assembler assemblr;
  

  assemblr.push(dword_ptr(esp, 4));
  assemblr.push(imm(reinterpret_cast<intptr_t>(&a)));
  assemblr.call(imm(reinterpret_cast<intptr_t>(&A_trampoline)));
  assemblr.add(esp, 8);
  assemblr.ret();
  
  callback cb = function_cast<callback>(assemblr.make());
  takes_callback(cb); 
   
}

