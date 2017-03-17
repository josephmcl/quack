%{
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> 
#include <string>
#include <vector>
#include <stack>
#include "ast.hpp"
#include "code.hpp"
#include <memory>
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int yylineno;

 
void yyerror(const char *s);

std::stack<std::shared_ptr<Node>> s;

std::shared_ptr<Program>pgm(new Program());

int h_count = 0;
int m_count = 0;
int a_count = 0;
std::stack<int> a_stack;
int e_count = 0;
int arg_last = 0;

std::vector<std::string> global_sym_names;


%}

%locations

%union {
	int ival;
	char cval;
	char *sval;
	std::shared_ptr<Node> nval (Node *n = new Node());
}

%token <sval> CLASS
%token <sval> DEF
%token <sval> EXTENDS
%token <sval> IF
%token <sval> ELIF
%token <sval> ELSE
%token <sval> WHILE
%token <sval> RETURN
%token <sval> ATLEAST
%token <sval> ATMOST
%token <sval> EQUALS
%token <sval> AND
%token <sval> OR
%token <sval> NOT
%token <sval> IDENT
%token <sval> INT_LIT
%token <sval> STRING_LIT
%token <cval> '+'
%token <cval> '-'
%token <cval> '*'
%token <cval> '/'
%token <cval> '<'
%token <cval> '>'
%token <cval> '{'
%token <cval> '}'
%token <cval> '('
%token <cval> ')'
%token <cval> ','
%token <cval> '.'
%token <cval> ';'
%token <cval> ':'

%type<nval> l_expr r_expr

%left AND OR NOT
%left '<' '>' ATLEAST ATMOST EQUALS 
%left '+' '-'
%left '*' '/'
%left '.'



%%

program			: classes statements 		{	
												if (!s.empty())
												{
													pgm->statements = s.top();
													s.pop();
												}
												while (!s.empty())
												{
													pgm->classes->v.push_back(std::dynamic_pointer_cast<Class>(s.top()));
													pgm->classes->keys.push_back(std::dynamic_pointer_cast<Class>(s.top())->id);
													s.pop();
												}


												std::shared_ptr<Class> Obj (new Class());
												Obj->id="Obj";
												Obj->s_id="$";
												std::shared_ptr<Class> String (new Class());
												String->id="String";
												String->s_id="Obj";
												std::shared_ptr<Class> Int (new Class());
												Int->id="Int";
												Int->s_id="Obj";
												std::shared_ptr<Class> Nothing (new Class());
												Nothing->id="Nothing";
												Nothing->s_id="Obj";
												std::shared_ptr<Class> Bool (new Class());
												Bool->id="Boolean";
												Bool->s_id="Obj";
												pgm->classes->v.push_back(Nothing);
												pgm->classes->keys.push_back(Nothing->id);
												pgm->classes->v.push_back(Int);
												pgm->classes->keys.push_back(Int->id);
												pgm->classes->v.push_back(String);
												pgm->classes->keys.push_back(String->id);
												pgm->classes->v.push_back(Bool);
												pgm->classes->keys.push_back(Bool->id);
												pgm->classes->v.push_back(Obj);
												pgm->classes->keys.push_back(Obj->id);

												
												
											}
				;

classes			: 							{}
				| classes class 			{}
				; 

class			: class_sig class_body 		{	std::shared_ptr<Node> n= (s.top());
												s.pop();
												std::shared_ptr<Class> m= dynamic_pointer_cast<Class>(s.top());
												s.pop();
												n->ln = yylineno;
												m->v.push_back(n);
												s.push(m);

											}
				;

class_sig 		: CLASS IDENT '(' u_formal_args ')' extends {	std::shared_ptr<Class> n= dynamic_pointer_cast<Class>(s.top());
																n->id = $2;
																n->ln = yylineno;
																s.pop();
																while(h_count > 0)
																{
																	n->v.push_back(s.top());
																	s.pop();
																	--h_count;
																}
																s.push(n);
															}
				;

u_formal_args	: 					{}
				| formal_args 		{}
				;

formal_args		: formal_arg						{}
				| formal_args ',' formal_arg 		{}
				;

formal_arg 		: IDENT ':' IDENT	{	
										std::shared_ptr<Node>n(new Node()); 
										n->type = "FORMAL_ARG";
										n->t1 = $1;
										n->t3 = $3;
										n->ln = yylineno;
										n->srepr = n->t1 + " : " + n->t3;
										s.push(n);
										++h_count;
									}
				;

extends 		:					{	std::shared_ptr<Class>n(new Class());
										n->type = "CLASS";
										n->s_id = "Obj";
										n->ln = yylineno;
										s.push(n);
									}
				| EXTENDS IDENT 	{	std::shared_ptr<Class>n(new Class());
										n->type = "CLASS";
										n->s_id = $2;
										n->ln = yylineno;
										s.push(n);
									}
				;

class_body 		: '{' statements methods '}' 	{ 	std::shared_ptr<Node>n(new Node()); 
													n->type = "CLASS_BODY";
													n->ln = yylineno;
													
													while(m_count > 0)
													{
														n->v.push_back(s.top());
														s.pop();
														--m_count;
													}
													n->v.push_back(s.top());
													s.pop();		

													s.push(n);
												}
				;

methods 		: 					{}
				| methods method 	{}
				;

method 			: DEF IDENT '(' u_formal_args ')' type_id statement_block 	
					{	
						std::shared_ptr<Node> n(new Node());
						n->type = "METHOD";
						n->t1 = $2;
						n->ln = yylineno;
						n->v.push_back(s.top());
						s.pop();
						n->v.push_back(s.top());
						s.pop();
						while(h_count > 0)
						{
							n->v.push_back(s.top());
							s.pop();
							--h_count;
						}
						
						++m_count;
						n->srepr = n->t1 + "(# args: " + std::to_string(n->v.size()-2) + ")";
						s.push(n);
					}
				;

statement_block	: '{' statements '}' 			{	std::shared_ptr<Node> n(new Node());
													n->type = "STATEMENT_BLOCK";
													n->ln = yylineno;
													
													n->v.push_back(s.top());
													s.pop();

													s.push(n);
												}
				;

statements		:								{
													std::shared_ptr<Node> n(new Node());
													n->type = "STATEMENTS";
													n->ln = yylineno;
													
													s.push(n);
												}

				| statements statement 			{	std::shared_ptr<Node> n(new Node());
													n->type = "STATEMENTS";
													n->ln = yylineno;

													n->v.push_back(s.top());
													s.pop();
													n->v.push_back(s.top());
													s.pop();

													s.push(n);}
				;

statement 		: IF r_expr statement_block elifs else 	{	std::shared_ptr<Node> n(new Node());
															n->type = "STMT_IF";
															n->t1 = "IF";
															n->srepr = n->t1;
															n->ln = yylineno;
															n->t3 = "STATEMENT";
															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();

															s.push(n);
														}
				| WHILE r_expr statement_block 			{	std::shared_ptr<Node> n(new Node());
															n->type = "STMT_WHILE";
															n->t1 = $1;
															n->t3 = "STATEMENT";
															n->ln = yylineno;
															n->srepr = n->t1;
															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();

															s.push(n);
														}
				| RETURN ';' 							{	std::shared_ptr<Node> n(new Node());
															n->type = "STATEMENT";
															n->t1 = $1;
															n->t3 = "STATEMENT";
															n->srepr = n->t1 + " Nothing";
															n->t2 = "Nothing";
															n->ln = yylineno;
															s.push(n);
														}
				| RETURN r_expr ';' 					{	std::shared_ptr<Node> n(new Node());
															n->type = "STMT_T_RETURN";
															n->t3 = "STATEMENT";
															n->t1 = $1;
															n->srepr = n->t1 + " Right Expression";
															n->ln = yylineno;
															n->t2 = "Set";
															
															n->v.push_back(s.top());
															s.pop();

															s.push(n);
														}
				| l_expr type_id '=' r_expr ';' 		{	std::shared_ptr<Node> n(new Node());
															n->type = "STMT_ASSIGN";
															n->srepr = "Assignment";
															n->ln = yylineno;
															n->t3 = "STATEMENT";

															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();
															n->v.push_back(s.top());
															s.pop();

															s.push(n);
														}
				| r_expr ';' 							{	std::shared_ptr<Node> n(new Node());
															n->type = "STATEMENT";
															n->ln = yylineno;
															n->t3 = "STATEMENT";
															n->t2 = "something";
															
															n->v.push_back(s.top());
															s.pop();

															s.push(n);
														}
				;

type_id 		: 			{	std::shared_ptr<Node> n(new Node());
								n->type = "TYPE_ID";
								n->t1 = "Nothing";
								n->t2 = "BARE";
								n->srepr = ":" + n->t1;
								n->ln = yylineno;
								s.push(n);
							}
				|':' IDENT 	{	std::shared_ptr<Node> n(new Node());
								n->type = "TYPE_ID";
								n->t1 = $2;
								n->t2 = "DEFINED";
								n->srepr = ":" + n->t1;
								n->ln = yylineno;
								s.push(n);
							}
				;

elifs 			: 				{	std::shared_ptr<Node> n(new Node());
									n->type = "ELIFS";
									n->t1 = "UNSET";
									n->ln = yylineno;
									
									s.push(n);
								}
				| elifs elif 	{	std::shared_ptr<Node> n(new Node());
									n->type = "ELIFS";
									n->t1 = "SET";
									n->ln = yylineno;
									
									n->v.push_back(s.top());
									s.pop();
									n->v.push_back(s.top());
									s.pop();
									s.push(n);
								}
				;

elif 			: ELIF r_expr statement_block 	{	std::shared_ptr<Node> n(new Node());
													n->type = "ELIF";
													n->ln = yylineno;
													
													n->v.push_back(s.top());
													s.pop();
													n->v.push_back(s.top());
													s.pop();
													s.push(n);
												}
				;

else 			: 							{	std::shared_ptr<Node> n(new Node());
												n->type = "ELSE";
												n->t1 = "UNSET";
												n->ln = yylineno;
												
												s.push(n);
											}
				| ELSE statement_block		{	std::shared_ptr<Node> n(new Node());
												n->type = "ELSE";
												n->t1 = "SET";
												n->ln = yylineno;

												n->v.push_back(s.top());
												s.pop();
												s.push(n);
											}
				;

l_expr			: IDENT				{	std::shared_ptr<Node> n(new Node());
										n->type = "L_EXPR";
										n->t1 = $1;
										n->srepr = n->t1;
										n->ln = yylineno;
										
										s.push(n);
									}
				| r_expr '.' IDENT	{	std::shared_ptr<Node> n(new Node());
										n->type = "MEM_L_EXPR";
										n->t1 = $3;
										n->t3 = $3;
										n->srepr = "." + n->t3;
										n->ln = yylineno;
										
										n->v.push_back(s.top());
										s.pop();
										s.push(n);
									}
				;

r_expr 			
				: l_expr				{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "Left Expression";
											n->srepr = n->t1;
											n->ln = yylineno;
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| '(' r_expr ')'		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "Parens";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '*' r_expr 	{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "*";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '/' r_expr 	{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "/";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '+' r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "+";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '-' r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "-";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr EQUALS r_expr	{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "==";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr ATMOST r_expr	{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "<=";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '<' r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "<";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr ATLEAST r_expr	{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = ">=";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '>' r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = ">";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr AND r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "And";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr OR r_expr		{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "Or";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| NOT r_expr			{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = "Not";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											n->v.push_back(s.top());
											s.pop();
											s.push(n);
										}
				| r_expr '.' IDENT '(' u_actual_args ')'	{	std::shared_ptr<Node> n(new Node());
																n->type = "R_EXPR";
																n->t1 = $3;
																n->t2 = "METHOD_CALL";
																n->srepr = "Method Call " + n->t1;
																n->ln = yylineno;
																int args = a_stack.top();
																while(args > 0)
																{
																	n->v.push_back(s.top());
																	s.pop();
																	--args;
																}
																++a_count;
																n->v.push_back(s.top());
																s.pop();
																a_stack.pop();
																s.push(n);
															}
				| IDENT '(' u_actual_args ')'				{ 	std::shared_ptr<Node>n(new Node());
																n->id = $1;
																n->type = "CONS";
																n->t1 = $1;
																n->srepr = "Constructor Call " + n->t1;
																n->ln = yylineno;
																n->LineNumber = yylineno;
																int args = a_stack.top();
																while(args > 0)
																{
																	n->v.push_back(s.top());
																	s.pop();
																	--args;
																}
																++a_count;
																n->srepr = n->t1 + "(# args: " + std::to_string(n->v.size()) + ")";
																a_stack.pop();
																s.push(n);
															}
				| STRING_LIT 			{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = $1;
											n->t2 = "STRING_LIT";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											s.push(n);
										}
				| INT_LIT 				{	std::shared_ptr<Node> n(new Node());
											n->type = "R_EXPR";
											n->t1 = $1;
											n->t2 = "INT_LIT";
											n->srepr = n->t1;
											n->ln = yylineno;
											
											s.push(n);
										}
				;

u_actual_args	: 				{a_stack.push(0);}
				| actual_args 	{}
				;

actual_args 	: actual_arg 					{a_stack.push(1);}
				| actual_args ',' actual_arg 	{++a_stack.top();}
				;

actual_arg 	 	: r_expr 	{	

								
								std::shared_ptr<Node> n(new Node());
								n->type = "ACTUAL_ARG";
								n->ln = yylineno;
								n->v.push_back(s.top());
								s.pop();
								s.push(n);

							}
				;


%%	

char *filename = (char *) malloc(sizeof(char)*255); 


int main(int argc, char** argv) {

	a_stack.push(0);

	FILE *f;
	f = stdin;
	if (argc > 1) {
    	if( !(f = fopen(argv[1], "r"))) {
			perror(argv[1]);
			exit(1);
		}
	}

	if (!f) {
		cout << "File could not be opened!" << endl;
		return -1;
	}
	// set flex to read from it instead of defaulting to STDIN:
	yyin = f;
	int rv;

	int fd; 
  	char fd_path[255]; 

  	fd = fileno(f); 
  	sprintf(fd_path, "/proc/self/fd/%d", fd); 
  	readlink(fd_path, filename, 255); 

	Program p = Program();

	do {
		rv = yyparse();
	} while (!feof(yyin));



	int er = pgm->classes->buildTree();

	std::vector<Node*> cons;

	
	/* form class hierarchy */
	pgm->statements->travelTree(cons, 0, global_sym_names);
	if (er==0)
	{
		std::cout<<"Class hierarchy not well-formed."<<std::endl;
	}
	/* check constructors */
	for(unsigned int i=0;i<cons.size();++i)
	{
		std::unique_ptr<Cons> n (new Cons());
		n->id = cons[i]->id;
		er = n->findClass(pgm->classes->keys, pgm->classes->v);
		if (er==0)
		{
			std::cout << "Constructor error: class \'" << n->id << "\' not defined." << std::endl;
			exit(-1);
		}
	}

	free(filename);

	/* find symbols */
	int rb;
	for(unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		std::shared_ptr<Class> curr_class = pgm->classes->v[i];
		rNodeC(&curr_class->sym_table, curr_class, 1);
		curr_class->sym_table = remove_dups(curr_class->sym_table);
		curr_class->travelTree(cons, 0, global_sym_names);
		curr_class->assembleMethods();
		
		for(unsigned int j=0;j<curr_class->methods.size();++j)
		{
			std::shared_ptr<Node> curr_method = curr_class->methods[j]->ptr;
			/* Get variable names from method body */
			rNode(&curr_method->sym_table, curr_method, 1);
			curr_method->sym_table = remove_dups(curr_method->sym_table);
		}
	}

	rNode(&pgm->sym_table, pgm->statements, 1);
	pgm->sym_table = remove_dups(pgm->sym_table); 

	//pgm->printSymbolTable();



	/* type checking */


	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		/* Check initalized values in class definition */
		std::vector<std::string> initi;
		initi.push_back("true");
		initi.push_back("false");
		initi.push_back("this");
		std::vector<std::string> method;
		initi = GetClassFormalArguments_Init(initi, pgm->classes->v[i]);
	}
  
	/* traverse for initalized values */
	std::vector<std::string> init;
	init.push_back("true");
	init.push_back("false");
	init = s_trav(init, pgm->statements); 
	pgm->initialized = init;

	for(unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		ClassWellformednessBubbler(pgm->classes->v[i], pgm);
	} 

	for(unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id == "Obj")
		{
			std::shared_ptr<Met> Print_ (new Met());
			Print_->name = "PRINT";
			Print_->rt = "Obj";
			Print_->ClassType = "Obj";
			std::shared_ptr<Met> Str_ (new Met());
			Str_->name = "STRING";
			Str_->rt = "String";
			Str_->ClassType = "Obj";
			std::shared_ptr<Met> Equals_ (new Met());
			Equals_->name = "EQUALS";
			Equals_->rt = "Boolean";
			Equals_->ClassType = "Obj";
			Equals_->ParameterSymbols.push_back("other");
			Equals_->ParameterTypes.push_back("Obj");


			pgm->classes->v[i]->methods.push_back(Print_);
			pgm->classes->v[i]->methods.push_back(Str_);
			pgm->classes->v[i]->methods.push_back(Equals_);
		}
		if (pgm->classes->v[i]->id == "Int")
		{
			std::shared_ptr<Met> Int_Add_ (new Met());
			Int_Add_->name = "ADD";
			Int_Add_->rt = "Int";
			Int_Add_->ClassType = "Int";
			Int_Add_->ParameterSymbols.push_back("other");
			Int_Add_->ParameterTypes.push_back("Int");

			std::shared_ptr<Met> Mul_ (new Met());
			Mul_->name = "MUL";
			Mul_->rt = "Int";
			Mul_->ClassType = "Int";
			Mul_->ParameterSymbols.push_back("other");
			Mul_->ParameterTypes.push_back("Int");

			std::shared_ptr<Met> Sub_ (new Met());
			Sub_->name = "SUB";
			Sub_->rt = "Int";
			Sub_->ClassType = "Int";
			Sub_->ParameterSymbols.push_back("other");
			Sub_->ParameterTypes.push_back("Int");

			std::shared_ptr<Met> Div_ (new Met());
			Div_->name = "DIV";
			Div_->rt = "Int";
			Div_->ClassType = "Int";
			Div_->ParameterSymbols.push_back("other");
			Div_->ParameterTypes.push_back("Int");

			//std::shared_ptr<Met> Int_Print_ (new Met());
			//Int_Print_->name = "PRINT";
			//Int_Print_->rt = "Nothing";

			std::shared_ptr<Met> Int_String_ (new Met());
			Int_String_->name = "STRING";
			Int_String_->rt = "String";
			Int_String_->ClassType = "Int";

			std::shared_ptr<Met> Int_Equals_ (new Met());
			Int_Equals_->name = "EQUALS";
			Int_Equals_->rt = "Boolean";
			Int_Equals_->ClassType = "Int";
			Int_Equals_->ParameterSymbols.push_back("other");
			Int_Equals_->ParameterTypes.push_back("Int");

			std::shared_ptr<Met> Int_LTE_ (new Met());
			Int_LTE_->name = "LESS_THAN_EQUALS";
			Int_LTE_->rt = "Boolean";
			Int_LTE_->ClassType = "Int";
			Int_LTE_->ParameterSymbols.push_back("other");
			Int_LTE_->ParameterTypes.push_back("Int");

			std::shared_ptr<Met> Int_LessThan_ (new Met());
			Int_LessThan_->name = "LESS_THAN";
			Int_LessThan_->rt = "Boolean";
			Int_LessThan_->ClassType = "Int";
			Int_LessThan_->ParameterSymbols.push_back("other");
			Int_LessThan_->ParameterTypes.push_back("Int");

			pgm->classes->v[i]->methods.push_back(Int_Add_);
			pgm->classes->v[i]->methods.push_back(Mul_);
			pgm->classes->v[i]->methods.push_back(Sub_);
			pgm->classes->v[i]->methods.push_back(Div_);

			//pgm->classes->v[i]->methods.push_back(Int_Print_);
			pgm->classes->v[i]->methods.push_back(Int_String_);
			pgm->classes->v[i]->methods.push_back(Int_Equals_);
			pgm->classes->v[i]->methods.push_back(Int_LessThan_);
			pgm->classes->v[i]->methods.push_back(Int_LTE_);

		}
		if (pgm->classes->v[i]->id == "String")
		{
			std::shared_ptr<Met> String_Add_ (new Met());
			String_Add_->name = "ADD";
			String_Add_->rt = "String";
			String_Add_->ClassType = "String";
			String_Add_->ParameterSymbols.push_back("other");
			String_Add_->ParameterTypes.push_back("String");

			std::shared_ptr<Met> Str_Print_ (new Met());
			Str_Print_->name = "PRINT";
			Str_Print_->rt = "Nothing";
			Str_Print_->ClassType = "String";

			std::shared_ptr<Met> Str_String_ (new Met());
			Str_String_->name = "STR";
			Str_String_->rt = "String";
			Str_String_->ClassType = "String";


			pgm->classes->v[i]->methods.push_back(String_Add_);
			pgm->classes->v[i]->methods.push_back(Str_Print_);
			pgm->classes->v[i]->methods.push_back(Str_String_);

		}
		if (pgm->classes->v[i]->id == "Boolean")
		{
			std::shared_ptr<Met> Bool_Equals_ (new Met());
			Bool_Equals_->name = "EQUALS";
			Bool_Equals_->rt = "Boolean";
			Bool_Equals_->ClassType = "Nothing";			
			Bool_Equals_->ParameterSymbols.push_back("other");
			Bool_Equals_->ParameterTypes.push_back("Boolean");

			pgm->classes->v[i]->methods.push_back(Bool_Equals_);

		}
	}

	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		for (unsigned int j=0;j<pgm->classes->v.size();++j)
		{
			if ((pgm->classes->v[i]->id ==  pgm->classes->v[j]->id)
			    && (i != j))
			{
				std::cerr << pgm->classes->v[i]->ln <<  
				" : Type Error : Multiple definitions of class " << 
				pgm->classes->v[i]->id << std::endl;
				exit(-1);
			}
		}
	}
	/* Class Bubbler Loop */
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" && 
			pgm->classes->v[i]->id != "Int" && 
			pgm->classes->v[i]->id != "String" && 
			pgm->classes->v[i]->id != "Boolean" && 
			pgm->classes->v[i]->id != "Nothing")
		{
			int ClassMatching = 0;
			int ClassCycles = 0;
			std::vector<std::string> ClassTypes1;
			std::vector<std::string> ClassSymbols = pgm->classes->v[i]->sym_table;
			std::vector<std::string> ClassTypes;
			std::vector<std::string> ClassDecs;
			std::shared_ptr<Node> ClassStatements;
			ClassSymbols.push_back("this");
			ClassSymbols.push_back("true");
			ClassSymbols.push_back("false");
			for (unsigned int j=0;j<pgm->classes->v[i]->ParameterSymbols.size();++j)
			{
				ClassSymbols.push_back(pgm->classes->v[i]->ParameterSymbols[j]);
			}
			ClassSymbols = remove_dups(ClassSymbols);
			for (unsigned int j=0;j<ClassSymbols.size();++j)
			{
				if (ClassSymbols[j] == "true" ||
					ClassSymbols[j] == "false")
				{
					ClassTypes.push_back("Boolean");
					ClassDecs.push_back("Boolean");
				}
				else
				if (ClassSymbols[j] == "this")
				{
					ClassTypes.push_back(pgm->classes->v[i]->id);
					ClassDecs.push_back(pgm->classes->v[i]->id);
				}
				else
				{
					int Set = 0;
					for (unsigned int k=0;k<pgm->classes->v[i]->ParameterSymbols.size();++k)
					{
						if (ClassSymbols[j] == pgm->classes->v[i]->ParameterSymbols[k])
						{	
							Set = 1;
							ClassTypes.push_back(pgm->classes->v[i]->ParameterTypes[k]);
							ClassDecs.push_back(pgm->classes->v[i]->ParameterTypes[k]);	
						}
					}
					if (Set == 0)
					{
						ClassTypes.push_back("Bottom");
						ClassDecs.push_back("Obj");
					}
				}
				
			}
			for (unsigned int j=0;j<pgm->classes->v[i]->v.size();++j)
			{
				if (pgm->classes->v[i]->v[j]->type == "CLASS_BODY")
				{

					ClassStatements = pgm->classes->v[i]->v[j];
				}
			}
			while (ClassMatching == 0)
			{
				ClassTypeBubbler(&ClassSymbols, &ClassTypes, 
						 ClassStatements, pgm, &ClassDecs);
				if (ClassTypes1 == ClassTypes)
				{
					ClassMatching = 1;
				}
				ClassTypes1 = ClassTypes;
				++ClassCycles;
				/*
				for (unsigned int i=0;i<GlobalTypes.size();++i)
				{
					std::cout << GlobalSyms[i] << " : " << GlobalTypes[i] << std::endl;
				}
				*/
			}

			pgm->classes->v[i]->Symbols = ClassSymbols;
			pgm->classes->v[i]->Types = ClassTypes;
			pgm->classes->v[i]->Decs = ClassDecs;
			
			MethodTypeBubblerInit(&ClassSymbols, &ClassTypes, 
						 pgm->classes->v[i], pgm, &ClassDecs);
		}
	}


	/* Statements Bubbler Loop */
	int Matching=0;
	int Cycles=0;
	std::vector<std::string> GlobalTypes1 = pgm->sym_table;
	std::vector<std::string> GlobalSyms = pgm->sym_table;
	std::vector<std::string> GlobalTypes = pgm->sym_table;
	std::vector<std::string> GlobalDecs;
	for (unsigned int i=0;i<GlobalTypes.size();++i)
	{
		GlobalTypes[i] = "Bottom";
		GlobalDecs.push_back("Obj");
	}

	while (Matching == 0)
	{
		StatementsTypeBubbler(&GlobalSyms, &GlobalTypes, pgm->statements, pgm, &GlobalDecs);
		if (GlobalTypes1 == GlobalTypes)
		{
			Matching = 1;
		}
		GlobalTypes1 = GlobalTypes;
		++Cycles;
	}
	pgm->StaticSymbols = GlobalSyms;
  	pgm->StaticTypes = GlobalTypes;
   	pgm->StaticDecs = GlobalDecs;

	GenerateCode(pgm);
}

void yyerror(const char *s) {

	cout << filename << ":" << yylineno << ": " << s <<" (at \'"<< yylval.sval <<"\')"<< endl;
	free(filename);
	exit(-1);
}