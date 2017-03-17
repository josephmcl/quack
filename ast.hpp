#ifndef _AST_HPP
#define _AST_HPP

#include <vector>
#include <memory>

class Node
{
public:
 	Node();
 	~Node();
 	std::string type;
 	std::string id;
 	std::string t1;
 	std::string t2;
 	std::string t3;
 	std::string srepr;
 	std::vector<std::shared_ptr<Node>> v;
 	std::vector<std::string> sym_table;
 	int LineNumber;
 	int ln;
 	std::vector<std::string> sym_init;
  	virtual void printAttr();
  	virtual void repr();
  	virtual void travelTree(std::vector<Node*> &b, int d, std::vector<std::string> &sym_n);
  	
};

class Met
{
public:
	Met();
	~Met();
	std::string name;
	int n;
	int ln;
	std::string ClassType;
	std::string rt;
	std::vector <std::string> ParameterSymbols;
	std::vector <std::string> ParameterTypes;
	std::shared_ptr<Node> ptr;
	std::vector<std::string> Symbols;
	std::vector<std::string> Types;
	std::vector<std::string> Decs;
};

class Statements : public Node
{
public:
   Statements();
   ~Statements();
   void printAttr();
   std::vector<std::shared_ptr<Node>> v;
   std::string type;
};
class Statement
{
public:
   Statement();
   Statement(std::string name);
   std::string name;
};

class Class : public Node
{
public:
	Class();
	~Class();
	std::string type;
	std::string id;
	std::string s_id;
	int ln;
	std::shared_ptr<Class> s_ptr;
	std::shared_ptr<Statements> statements;
	std::vector<std::shared_ptr<Node>> v;
	std::vector<std::string> sym_n;
	std::vector<std::string> sym_table;
	std::vector<std::shared_ptr<Met>> methods;
	std::vector<std::string> ParameterSymbols;
	std::vector<std::string> ParameterTypes;
	std::vector<std::string> Fields;
	std::vector<std::string> Symbols;
	std::vector<std::string> Types;
	std::vector<std::string> Decs;
	int obj_dist;
	void printAttr();
	int printAttri();
	void travelTree(std::vector<Node*> &b, int d, std::vector<std::string> &sym_n);
	void assembleMethods();
};

class Classes : public Node
{
public:
	Classes();
	~Classes();
	std::vector<std::shared_ptr<Class>> v;
	std::vector<std::string> keys;
	virtual void printAttr();
	int buildTree();
	std::string
	GetCommonSuper(std::shared_ptr<Class> FirstClass, std::shared_ptr<Class> SecondClass);
	std::string
	GetCommonSuperFromString(std::string FirstClass,std::string SecondClass);
};
class Program : public Node
{
public:
   Program();
   Program(Node _c);
   virtual void printAttr();
   ~Program();
   Node c;
   std::vector<std::string> sym_table;
   std::shared_ptr<Classes> classes;
   std::shared_ptr<Node> statements;
   std::vector<std::string> initialized;
   std::vector<std::string> StaticSymbols;
   std::vector<std::string> StaticTypes;
   std::vector<std::string> StaticDecs;
   void printSymbolTable();
};
class Cons : public Node
{
public:
	Cons();
	~Cons();
	std::string id;
	std::string type;
	std::shared_ptr<Class> s_ptr;
	std::vector<std::shared_ptr<Node>> v;
	int findClass(std::vector<std::string> &v, std::vector<std::shared_ptr<Class>> &vp);
	int printAttri();
};


int rNode(std::vector<std::string> *symbol_table, std::shared_ptr<Node> node, int clean);
int rNodeC(std::vector<std::string> *symbol_table, std::shared_ptr<Class> node, int clean);
std::vector<std::string> 
remove_dups(std::vector<std::string> v);
std::vector<std::string> 
s_trav(std::vector<std::string> init, std::shared_ptr<Node> n);
std::vector<std::string>
class_s_trav(std::vector<std::string> init, std::shared_ptr<Node> n);
std::vector<std::string>
GetMethodFormalArguments_Init(std::vector<std::string> InitalizedValues, std::shared_ptr<Node> Method);
std::vector<std::string>
GetClassFormalArguments_Init(std::vector<std::string> InitalizedValues, std::shared_ptr<Class> c);
void 
MethodTypeBubblerInit(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Class> CurrentClass, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared);
std::string
MethodTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared);
std::string
StatementsTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types, 
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared);
int ClassWellformednessBubbler(std::shared_ptr<Class> Class, std::shared_ptr<Program> pgm);
std::string
ClassTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared);

std::shared_ptr<Met>
MethodisValid(std::shared_ptr<Class> CurrentClass, std::string MethodName);

#endif