flex pq.lex 
bison -d -t -v quack.y
g++ -std=c++11 quack.tab.c lex.yy.c ast.cpp code.cpp -lfl -o quack
