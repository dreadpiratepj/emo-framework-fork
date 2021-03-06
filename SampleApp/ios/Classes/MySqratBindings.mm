//
//  MySqratBindings.mm
//  SampleApp
//
//  Created by PJ Cabrera on 6/26/2012.
//  Copyright 2012 PJ Cabrera. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the project nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "sqratVM.h"

#include "Constants.h"
#include "SampleAppAppDelegate.h"

#if __cplusplus
extern "C" {
#endif
	
Sqrat::SqratVM *sqratVM;

void showMyUIView(void)
{
    sqratVM->doString("print(\"In C showMyUIView\");");
    SampleAppAppDelegate *appDelegate = 
        (SampleAppAppDelegate *)[UIApplication sharedApplication].delegate;
    [appDelegate showMyUIView];
}
    
std::string foo(int i, double d, const std::string& str)
{
    std::stringstream ss;
    ss << i << " " << d << " " << str;
    return ss.str();
}

class MyClass {
public:
    MyClass() {bar=999;}
    void Foo() {}
    int bar;
};

HSQUIRRELVM createSqratVM(void) {
    sqratVM = new Sqrat::SqratVM(SQUIRREL_VM_INITIAL_STACK_SIZE);
    return sqratVM->getVM();
}
    
void initSqratBindings(void)
{

    sqratVM->getRootTable().Func("showMyUIView", &showMyUIView);
    
    sqratVM->getRootTable().Func("foo", &foo);

    sqratVM->getRootTable().Bind("MyClass", Sqrat::Class<MyClass>(sqratVM->getVM())
                               .Func("Foo", &MyClass::Foo)
                               .Var("bar", &MyClass::bar)
                               );
    
    sqratVM->doString("arr <- [0,1,2,300,4]");

}

#if __cplusplus
}
#endif