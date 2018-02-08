# Bird
Bird is a very basic programming language I devloped, in order to learn how interpreters/compilers work.
Wings is the interpreter for "bird code".

Currently we have only "input", "output", "comments", "assignments", "mathematical operators", "if/else" and "while" statements in Bird's grammer.
For better understanding, code snippets are provided in /Examples/

I learned a lot from this blog series https://ruslanspivak.com/lsbasi-part1/ and a good amount of my code/logic comes from there.

Steps to use wings.cpp
1. Create a file containing Bird code, (eg. bird in this case)
2. Compile wings.cpp file with g++ -std=c++14
3. While executing from command line, give name of file containing Bird code as command line argument argv[1].
4. To see AST and internal details, provide command line argument "details" as argv[2].

Commands:

g++ -std=c++14 wings.cpp

./a.out bird

./a.out bird details
