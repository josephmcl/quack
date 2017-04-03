README.txt

Quack Compiler 
JOSEPH McLAUGHLIN
================
Quack is a simple object oriented, type inferred language.
This is a very early implementation of a compiler for the Quack language.
================
Usage: 
$ ./quack [file]
Output:
qk.out 

qk.out is the compiled quack program. 
A quack program without type errors will generate and compile C code.
================
NOTES:

Notable changes from the initial language is that all class fields are private, not protected.
This can be changed in the future, in fact all the infrastructure is in place in code generation (mostly,) but it requires reworking type checking. 
The sugar for operators (+,-,*,/) are ADD, SUB, MUL, DIV. 

The typechecker is not the nicest. There are few cases where it can proceed after encountering a type error most errors result in the program exiting.
This is a potential improvement to consider in the future.

If I could do this again (which I might) I'm going to rework a lot, starting with taking advantage of bison. 

================
THE GOOD DIRECTORY:
good contains four files:

Flow.qk  
Demos control flow, nesting combos of if's and while's, and conditionals.

Rec.qk  
Demos recursion. 
NOTE: "this" is not a variable used in code generation. Recursion requires instancing a new object for now.

Sqr.qk  
Demos class inheritance and method overloading.

TypeWalk.qk
Demos Type-walking.

================
THE BAD DIRECTORY:
bad contains five files:

Cond.qk  
Demos conditional type checking -- expects a subtype of Boolean

Init.qk  
Demos initialization checking

Rec.qk  
Demos method argument checking.

Sqr.qk  
Demos contra/covarience rules.

TypeWalk.qk
Demos type walking and checking the existence of methods. 

================
COMPILATION:
The quack compiler produces the file pgm.c that is linked with Builtins.c and uses the boehm GC.


