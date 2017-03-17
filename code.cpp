#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "ast.hpp"
#include <string>


int r_stmt(std::shared_ptr<Node> Node, std::ofstream &f)
{

	for (unsigned int i=0;i<Node->v.size();++i)
	{
        if (Node->v[i]->type == "STATEMENTS")
		{
			r_stmt(Node->v[i], f);
		}
	}
	for (unsigned int i=0;i<Node->v.size();++i)
	{
		if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
        {
        	f << Node->v[i]->type << "\n";
        }
    }

}

void
GenerateInstanceParameters(std::shared_ptr<Program> pgm, std::ofstream &f, std::string ClassName)
{
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id == "Int")
		{
			f << "int_literal(0)";
			return;
		} else if (pgm->classes->v[i]->id == "String")
		{
			f << "str_literal(\"placeholder\")";
			return;
		} else
		{
			if (pgm->classes->v[i]->id == ClassName)
			{
				f << "the_class_" << pgm->classes->v[i]->id << "->constructor(";
				if (pgm->classes->v[i]->id != "Obj" 
			    	&& pgm->classes->v[i]->id != "Boolean"
			    	&& pgm->classes->v[i]->id != "Nothing"
			    	&& pgm->classes->v[i]->id != "String" 
			    	&& pgm->classes->v[i]->id != "Int")
			    {
					for (unsigned int j=0;j<pgm->classes->v[i]->ParameterTypes.size();++j)
					{
						GenerateInstanceParameters(pgm, f, pgm->classes->v[i]->ParameterTypes[j]);
						if (j < pgm->classes->v[i]->ParameterTypes.size()-1)
						{
							f << ", ";
						}
					}
				}
				f << ")";
			}
		}
	}
}
void 
GenerateInstancesForConstructor(std::shared_ptr<Program> pgm, std::ofstream &f, std::shared_ptr<Class> CurrentClass)
{
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id == "Int")
		{
			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = int_literal(0);\n";
		} else if (pgm->classes->v[i]->id == "String")
		{
			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = str_literal(\"placeholder\");\n";
		} else if (pgm->classes->v[i]->id == CurrentClass->id)
		{
			/* do nothing */
		} else if (pgm->classes->v[i]->id == pgm->classes->GetCommonSuperFromString(pgm->classes->v[i]->id, CurrentClass->id))
		{
			/* do nothing */
		} else
		{

			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = the_class_" << pgm->classes->v[i]->id << "->constructor(";
			if (pgm->classes->v[i]->id != "Obj" 
			    && pgm->classes->v[i]->id != "Boolean"
			    && pgm->classes->v[i]->id != "Nothing"
			    && pgm->classes->v[i]->id != "String" 
			    && pgm->classes->v[i]->id != "Int")
			{
				for (unsigned int j=0;j<pgm->classes->v[i]->ParameterTypes.size();++j)
				{
					GenerateInstanceParameters(pgm, f, pgm->classes->v[i]->ParameterTypes[j]);
					if (j < pgm->classes->v[i]->ParameterTypes.size()-1)
					{
						f << ", ";
					}
				}
			}
			f << ");\n";
		}
	}

}
void 
GenerateInstances(std::shared_ptr<Program> pgm, std::ofstream &f)
{
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id == "Int")
		{
			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = int_literal(0);\n";
		} else if (pgm->classes->v[i]->id == "String")
		{
			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = str_literal(\"placeholder\");\n";
		} else
		{

			f << "obj_" << pgm->classes->v[i]->id << " " << pgm->classes->v[i]->id 
			<< "Instance = the_class_" << pgm->classes->v[i]->id << "->constructor(";
			if (pgm->classes->v[i]->id != "Obj" 
			    && pgm->classes->v[i]->id != "Boolean"
			    && pgm->classes->v[i]->id != "Nothing"
			    && pgm->classes->v[i]->id != "String" 
			    && pgm->classes->v[i]->id != "Int")
			{
				for (unsigned int j=0;j<pgm->classes->v[i]->ParameterTypes.size();++j)
				{
					GenerateInstanceParameters(pgm, f, pgm->classes->v[i]->ParameterTypes[j]);
					if (j < pgm->classes->v[i]->ParameterTypes.size()-1)
					{
						f << ", ";
					}
				}
			}
			f << ");\n";
		}
	}

}

int GenerateStmt(std::shared_ptr<Node> Node, std::ofstream &f, std::shared_ptr<Program> pgm)
{

	if (Node->type == "STATEMENTS")
	{
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENTS")
			{
				GenerateStmt(Node->v[i], f, pgm);
			}
		}
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
	        {
				GenerateStmt(Node->v[i], f, pgm);
			}
		}
	} else if (Node->type == "STMT_T_RETURN")
	{
		f << "return ";
		GenerateStmt(Node->v[0], f, pgm);
		f << ";\n";
	} else if (Node->type == "STMT_ASSIGN")
	{
		std::string Type;
		std::string Symbol;
		std::string Value;
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "L_EXPR")
			{
				Symbol = Node->v[i]->t1;
			}
		}
		f << Symbol << "_=";
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
			{
				GenerateStmt(Node->v[i], f, pgm);
			}
		}
		f << ";\n";
	} else if (Node->type == "STATEMENT")
	{
		//TODO Add return bit
		std::string Type;
		std::string Symbol;
		std::string Value;
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
			{
				GenerateStmt(Node->v[i], f, pgm);
			}
		}
		f << ";\n";
	} else if (Node->type == "STMT_IF")
	{
		f << "if (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateStmt(Node->v[i], f, pgm);
	        	d = i;
	         }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << ")\n{";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "STATEMENT_BLOCK")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateStmt(Node->v[i]->v[0], f, pgm);
	         }
		}
		f << "}\n";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "ELIFS")
	         {  /* Node->v[i]->v[0]->v[0] is a statement */
	        	GenerateStmt(Node->v[i], f, pgm);
	         }
		}
		f << "else {";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "ELSE" && Node->v[i]->t1 == "UNSET")
	         {  /* do nothing */
	         } else if (Node->v[i]->type == "ELSE" && Node->v[i]->t1 == "SET")
	         {  /* Node->v[i]->v[0]->v[0] is a statement */
	        	GenerateStmt(Node->v[i]->v[0]->v[0], f, pgm);
	         }
		}
		f << "}\n";

	} else if (Node->type == "ELIFS")
	{
		if (Node->t1 == "SET")
		{
			for (unsigned int i=0;i<Node->v.size();++i)
			{
				if (Node->v[i]->type == "ELIF")
				{
					GenerateStmt(Node->v[i], f, pgm);
				}	
				else if (Node->v[i]->type == "ELIFS")
				{
					GenerateStmt(Node->v[i], f, pgm);
				}	
			}
		}
		else
		{
			return 1;
		}
	} else if (Node->type == "ELIF")
	{
		f << "else if (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "R_EXPR")
		    {  
		    	GenerateStmt(Node->v[i], f, pgm);
		    	d = i;
		    }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << "){\n";
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENT_BLOCK")
		    {  /* Node->v[i]->v[0] is a statement */
		    	GenerateStmt(Node->v[i]->v[0], f, pgm);
		    }
		}
		f << "\n}\n";
	} else if (Node->type == "STMT_WHILE")
	{
		f << "while (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "R_EXPR")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateStmt(Node->v[i], f, pgm);
	        d = i;
	         }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << ")\n{";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "STATEMENT_BLOCK")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateStmt(Node->v[i]->v[0], f, pgm);
	         }
		}
		f << "}\n";
      	return 1;
	} else if (Node->type == "L_EXPR")
	{
      	f << Node->t1<< "_";
      	return 1;
    } else if (Node->type == "MEM_L_EXPR")
	{
		f << "this_->"<< Node->t1<< "_";
		return 1;
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Left Expression")
	{
      	GenerateStmt(Node->v[0], f, pgm);
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Parens")
	{
		f << "(";
      	GenerateStmt(Node->v[0], f, pgm);
      	f << ")";
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t2 == "INT_LIT")
	{
      	f << "int_literal(" << Node->t1 << ")";
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t2 == "STRING_LIT")
   	{
      	f << "str_literal(" << Node->t1 << ")";
      	return 1;
    } else if (Node->type == "R_EXPR" && Node->t1 == "+")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "ADD")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->ADD(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t1 == "-")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "SUB")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->SUB(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t1 == "*")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "MUL")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->MUL(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "/")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "DIV")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->DIV(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "<")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN")
   					{
   					
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->LESS(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == ">")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << ")->clazz->LESS(((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "<=")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN_EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << ")->clazz->LESS_EQUALS(((obj_" << Type << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "))";
   	}  else if (Node->type == "R_EXPR" && Node->t1 == ">=")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN_EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << ")->clazz->LESS_EQUALS(((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "))";
   	}  else if (Node->type == "R_EXPR" && Node->t1 == "==")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = StatementsTypeBubbler(&s, &t, Node->v[1], pgm, &d);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << ")->clazz->EQUALS(((obj_" << Type << ")";
   		GenerateStmt(Node->v[0], f, pgm);
   		f << "), ((obj_" << "Obj" << ")";
   		//f << "), ((obj_" << ParameterType << ")";
   		GenerateStmt(Node->v[1], f, pgm);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t2 == "METHOD_CALL")
   	{
   		std::string MethodName = Node->t1;
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type;
   		std::string Static;
   		std::vector<std::string> ParameterTypes;
   		std::shared_ptr<Met> ReturnMethod;
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				Type = StatementsTypeBubbler(&s, &t, Node->v[i], pgm, &d); /* may need to change this to determine dynamic type*/

   			}
   		}
   		/* Fetch the location of the method -- it may be inherited.*/
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{
   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				ReturnMethod = MethodisValid(CurrentClass, MethodName);
   			}
   		}
   		/* Process the method */
   		ParameterTypes = ReturnMethod->ParameterTypes;
   		f <<"((obj_" << Type << ")";   		
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				GenerateStmt(Node->v[i], f, pgm);
   			}
   		}
		f << ")->clazz->" << MethodName << "(((obj_" << ReturnMethod->ClassType << ")";
 
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				GenerateStmt(Node->v[i], f, pgm);
   			}
   		}
   		f << ")";
   		int ArgCounter = 0;
   		for (int i=Node->v.size()-1;i>=0;--i)
   		{
   			if (Node->v[i]->type == "ACTUAL_ARG")
	   		{

		   		f << ", ((obj_" << ParameterTypes[ArgCounter] << ")";
		   		GenerateStmt(Node->v[i]->v[0], f, pgm);
		   		f << ")";
		   		++ArgCounter;
		   	}
	   	}

   		f << ")";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "And")
   	{
   		f << "((";
   		if (Node->v[1]->t1 != "Not"
		 && Node->v[1]->t1 != "Or"
		 && Node->v[1]->t1 != "And")
   		{
   			GenerateStmt(Node->v[1], f, pgm);
   			f << "==lit_true";
   		} else
   		{
   			GenerateStmt(Node->v[1], f, pgm);
   		}
   		f << ") && (";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   			f << "==lit_true";
   		} else
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   		}
   		f << "))";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Or")
   	{
   		f << "((";
   		if (Node->v[1]->t1 != "Not"
		 && Node->v[1]->t1 != "Or"
		 && Node->v[1]->t1 != "And")
   		{
   			GenerateStmt(Node->v[1], f, pgm);
   			f << "==lit_true";
   		} else
   		{
   			GenerateStmt(Node->v[1], f, pgm);
   		}
   		f << ") || (";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   			f << "==lit_true";
   		} else
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   		}
   		f << "))";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Not")
   	{
   		f << "(!(";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   			f << "==lit_true";
   		} else
   		{
   			GenerateStmt(Node->v[0], f, pgm);
   		}
   		f << "))";
   	} else if (Node->type == "CONS")
   	{
   		std::string ClassName = Node->t1;
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type;
   		std::string Static;
   		std::vector<std::string> ParameterTypes;
   		std::shared_ptr<Met> ReturnMethod;
   		std::shared_ptr<Class> CurrentClass;
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{

   		}
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == ClassName)
   			{
   				CurrentClass = pgm->classes->v[i];
   			}
   		}
   		ParameterTypes = CurrentClass->ParameterTypes;
   		f << "the_class_" << ClassName << "->constructor(";
   		for (int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "ACTUAL_ARG")
	   		{
		   		f << "((obj_" << ParameterTypes[i] << ")";
		   		GenerateStmt(Node->v[i]->v[0], f, pgm);
		   		f << ")";
		   	}
		   	if ( i < Node->v.size()-1)
		   	{
		   		f << ", ";
		   	}
	   	}

   		f << ")";
   	} else
	{
	
				/*
		if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
        {
        	f << Node->v[i]->type << "\n";
        }
        */
        return 1;
    }
}
int 
GenerateMethodStmt(std::shared_ptr<Node> Node, 
			 	   std::ofstream &f, 
			 	   std::shared_ptr<Program> pgm,
			 	   std::vector<std::string> *Symbols,
             	   std::vector<std::string> *Types,
             	   std::vector<std::string> *Declared)
{

	if (Node->type == "STATEMENTS")
	{
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENTS")
			{
				GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
			}
		}
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
	        {
				GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
			}
		}
	} else if (Node->type == "STMT_T_RETURN")
	{
		f << "return ";
		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
		f << ";\n";
	} else if (Node->type == "STMT_ASSIGN")
	{
		std::string Type;
		std::string Symbol;
		std::string Value;
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "L_EXPR")
			{
				Symbol = Node->v[i]->t1;
			} else if (Node->v[i]->type == "MEM_L_EXPR")
			{
				Symbol = "this_->" + Node->v[i]->t1 ;
			}
		}
		f << Symbol << "_=";
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
			{
				GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
			}
		}
		f << ";\n";
	} else if (Node->type == "STATEMENT")
	{
		//TODO Add return bit
		if (Node->t2 == "Nothing")
		{
			//f << "return the_class_Nothing->constructor();";
		}
		else
		{
			std::string Type;
			std::string Symbol;
			std::string Value;
			for (unsigned int i=0;i<Node->v.size();++i)
			{
				if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
				{
					GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
				}
			}
			f << ";\n";
		}
	} else if (Node->type == "STMT_IF")
	{
		f << "if (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
	        	d = i;
	         }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << ")\n{";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "STATEMENT_BLOCK")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i]->v[0], f, pgm, Symbols, Types, Declared);
	         }
		}
		f << "}\n";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "ELIFS")
	         {  /* Node->v[i]->v[0]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
	         }
		}
		f << "else {";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "ELSE" && Node->v[i]->t1 == "UNSET")
	         {  /* do nothing */
	         } else if (Node->v[i]->type == "ELSE" && Node->v[i]->t1 == "SET")
	         {  /* Node->v[i]->v[0]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i]->v[0]->v[0], f, pgm, Symbols, Types, Declared);
	         }
		}
		f << "}\n";

	} else if (Node->type == "ELIFS")
	{
		if (Node->t1 == "SET")
		{
			for (unsigned int i=0;i<Node->v.size();++i)
			{
				if (Node->v[i]->type == "ELIF")
				{
					GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
				}	
				else if (Node->v[i]->type == "ELIFS")
				{
					GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
				}	
			}
		}
		else
		{
			return 1;
		}
	} else if (Node->type == "ELIF")
	{
		f << "else if (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "R_EXPR")
		    {  
		    	GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
		    	d = i;
		    }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << "){\n";
		for (unsigned int i=0;i<Node->v.size();++i)
		{
			if (Node->v[i]->type == "STATEMENT_BLOCK")
		    {  /* Node->v[i]->v[0] is a statement */
		    	GenerateMethodStmt(Node->v[i]->v[0], f, pgm, Symbols, Types, Declared);
		    }
		}
		f << "\n}\n";
	} else if (Node->type == "STMT_WHILE")
	{
		f << "while (";
		int d = 0;
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "R_EXPR")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
	        d = i;
	         }
		}
		if (Node->v[d]->t1 == "Parens")
		{
			if (Node->v[d]->v[0]->t1 != "Not"
			 && Node->v[d]->v[0]->t1 != "Or"
			 && Node->v[d]->v[0]->t1 != "And")
			{
				f << "==lit_true";
			}
		} else if (Node->v[d]->t1 != "Not"
		 && Node->v[d]->t1 != "Or"
		 && Node->v[d]->t1 != "And")
		{
			f << "==lit_true";
		}
		f << ")\n{";
		for (unsigned int i=0;i<Node->v.size();++i)
	    {
	         if (Node->v[i]->type == "STATEMENT_BLOCK")
	         {  /* Node->v[i]->v[0] is a statement */
	        	GenerateMethodStmt(Node->v[i]->v[0], f, pgm, Symbols, Types, Declared);
	         }
		}
		f << "}\n";
      	return 1;
	} else if (Node->type == "L_EXPR")
	{
      	f << Node->t1<< "_";
      	return 1;
    } else if (Node->type == "MEM_L_EXPR")
	{
		f << "this_->"<< Node->t1<< "_";
		return 1;
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Left Expression")
	{
      	GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Parens")
	{
		f << "(";
      	GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
      	f << ")";
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t2 == "INT_LIT")
	{
      	f << "int_literal(" << Node->t1 << ")";
      	return 1;
   	} else if (Node->type == "R_EXPR" && Node->t2 == "STRING_LIT")
   	{
      	f << "str_literal(" << Node->t1 << ")";
      	return 1;
    } else if (Node->type == "R_EXPR" && Node->t1 == "+")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "ADD")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->ADD(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t1 == "-")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "SUB")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->SUB(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t1 == "*")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "MUL")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->MUL(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "/")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "DIV")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->DIV(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "<")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->LESS(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == ">")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->LESS(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	}  else if (Node->type == "R_EXPR" && Node->t1 == "<=")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN_EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->LESS_EQUALS(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "))";
   	}  else if (Node->type == "R_EXPR" && Node->t1 == ">=")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "LESS_THAN_EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->LESS_EQUALS(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "))";
   	}  else if (Node->type == "R_EXPR" && Node->t1 == "==")
   	{	//type
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
   		std::string ParameterType;
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{

   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   				{
   					if (CurrentClass->methods[j]->name == "EQUALS")
   					{
   						ParameterType = CurrentClass->methods[j]->ParameterTypes[0];
   					}
   				}
   			}
   		}
   		f << "((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << ")->clazz->EQUALS(((obj_" << Type << ")";
   		GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		f << "), ((obj_" << ParameterType << ")";
   		GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		f << "))";
  	} else if (Node->type == "R_EXPR" && Node->t2 == "METHOD_CALL")
   	{
   		std::string MethodName = Node->t1;
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type;
   		std::string Static;
   		std::vector<std::string> ParameterTypes;
   		std::shared_ptr<Met> ReturnMethod;
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared); /* may need to change this to determine dynamic type*/

   			}
   		}
   		/* Fetch the location of the method -- it may be inherited.*/
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == Type)
   			{
   				std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
   				ReturnMethod = MethodisValid(CurrentClass, MethodName);
   			}
   		}
   		/* Process the method */
   		ParameterTypes = ReturnMethod->ParameterTypes;
   		f <<"((obj_" << Type << ")";   		
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
   			}
   		}
		f << ")->clazz->" << MethodName << "(((obj_" << ReturnMethod->ClassType << ")";
 
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{
   			if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
   			{
   				GenerateMethodStmt(Node->v[i], f, pgm, Symbols, Types, Declared);
   			}
   		}
   		f << ")";
   		int ArgCounter = 0;
   		for (int i=Node->v.size()-1;i>=0;--i)
   		{
   			if (Node->v[i]->type == "ACTUAL_ARG")
	   		{

		   		f << ", ((obj_" << ParameterTypes[ArgCounter] << ")";
		   		GenerateMethodStmt(Node->v[i]->v[0], f, pgm, Symbols, Types, Declared);
		   		f << ")";
		   		++ArgCounter;
		   	}
	   	}

   		f << ")";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "And")
   	{
   		f << "((";
   		if (Node->v[1]->t1 != "Not"
		 && Node->v[1]->t1 != "Or"
		 && Node->v[1]->t1 != "And")
   		{
   			GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   			f << "==lit_true";
   		} else
   		{
   			GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		}
   		f << ") && (";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   			f << "==lit_true";
   		} else
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		}
   		f << "))";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Or")
   	{
   		f << "((";
   		if (Node->v[1]->t1 != "Not"
		 && Node->v[1]->t1 != "Or"
		 && Node->v[1]->t1 != "And")
   		{
   			GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   			f << "==lit_true";
   		} else
   		{
   			GenerateMethodStmt(Node->v[1], f, pgm, Symbols, Types, Declared);
   		}
   		f << ") || (";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   			f << "==lit_true";
   		} else
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		}
   		f << "))";
   	} else if (Node->type == "R_EXPR" && Node->t1 == "Not")
   	{
   		f << "(!(";
   		if (Node->v[0]->t1 != "Not"
		 && Node->v[0]->t1 != "Or"
		 && Node->v[0]->t1 != "And")
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   			f << "==lit_true";
   		} else
   		{
   			GenerateMethodStmt(Node->v[0], f, pgm, Symbols, Types, Declared);
   		}
   		f << "))";
   	} else if (Node->type == "CONS")
   	{
   		std::string ClassName = Node->t1;
   		std::vector<std::string> s = pgm->StaticSymbols;
   		std::vector<std::string> t = pgm->StaticTypes;
   		std::vector<std::string> d = pgm->StaticDecs;
   		std::string Type;
   		std::string Static;
   		std::vector<std::string> ParameterTypes;
   		std::shared_ptr<Met> ReturnMethod;
   		std::shared_ptr<Class> CurrentClass;
   		for (unsigned int i=0;i<Node->v.size();++i)
   		{

   		}
   		for (unsigned int i=0;i<pgm->classes->v.size();++i)
   		{	
   			if (pgm->classes->v[i]->id == ClassName)
   			{
   				CurrentClass = pgm->classes->v[i];
   			}
   		}
   		ParameterTypes = CurrentClass->ParameterTypes;
   		f << "the_class_" << ClassName << "->constructor(";
   		int ArgCounter = 0;
   		for (int i=Node->v.size()-1;i>=0;--i)
   		{
   			if (Node->v[i]->type == "ACTUAL_ARG")
	   		{
		   		f << "((obj_" << ParameterTypes[ArgCounter] << ")";
		   		GenerateMethodStmt(Node->v[i]->v[0], f, pgm, Symbols, Types, Declared);
		   		f << ")";
		   		++ArgCounter;
		   	}
		   	if ( i > 0)
		   	{
		   		f << ", ";
		   	}
	   	}

   		f << ")";
   	} else
	{
	
				/*
		if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
        {
        	f << Node->v[i]->type << "\n";
        }
        */
        return 1;
    }
}

int GenerateMain(std::shared_ptr<Program> pgm, std::ofstream &f)
{
	f << "/*\n";
	r_stmt (pgm->statements, f);
	f << "*/\n";
	//GenerateInstances(pgm, f);
	for (unsigned int i=0;i<pgm->initialized.size();++i)
	{
		f << "void *"
		  << pgm->initialized[i]
		  << "_";
		if (pgm->initialized[i]=="true")
		{
			f << "=lit_true";
		} else
		if (pgm->initialized[i]=="false")
		{
			f << "=lit_false";
		}
		f << ";\n";
	}
	GenerateStmt(pgm->statements, f, pgm);
}
void 
GenerateClassStructMethods(std::shared_ptr<Class> CurrentClass, std::vector<std::string> *Methods, std::ofstream &f)
{
	for (unsigned int i=0;i<CurrentClass->methods.size();++i)
	{
		int Found = 0;
		for (unsigned int j=0;j<Methods->size();++j)
		{
			if (CurrentClass->methods[i]->name == Methods->at(j))
			{
				Found = 1;
			}
		}
		if (Found == 1) 
		{
			/* ddo nothing */
		}
		else
		{
			std::string Name;
			if ( CurrentClass->methods[i]->name == "LESS_THAN")
			{
				Name = "LESS";
			} else 
			{
				Name = CurrentClass->methods[i]->name;
			}
			f << "obj_" << CurrentClass->methods[i]->rt << " (*"
			<< Name << ") (";
			f << "obj_" << CurrentClass->id;
			for (unsigned int j=0;j<CurrentClass->methods[i]->ParameterTypes.size();++j)
			{

				f << ", obj_" << CurrentClass->methods[i]->ParameterTypes[j];
				
			}
			f << ");\n";
			Methods->push_back(Name);
		}

	}
	if (CurrentClass->id != "Obj")
	{
		GenerateClassStructMethods(CurrentClass->s_ptr, Methods, f);
	}
}
void

GenerateTheClass(std::shared_ptr<Program> pgm, 
				 std::shared_ptr<Class> CurrentClass, 
				 std::vector<std::string> *Methods, 
				 std::ofstream &f)
{
/*
struct  class_Pt_struct  the_class_Pt_struct = {
new_Pt,
Pt_method_ADD,
Pt_method__y,
Pt_method__x,
Pt_method_PRINT,
Obj_method_STRING,
Obj_method_EQUALS

};
class_Pt the_class_Pt = &the_class_Pt_struct; 
*/
	for (unsigned int i=0;i<CurrentClass->methods.size();++i)
	{
		int Found = 0;
		for (unsigned int j=0;j<Methods->size();++j)
		{
			if (CurrentClass->methods[i]->name == Methods->at(j))
			{
				Found = 1;
			}
		}
		if (Found == 1) 
		{
			/* ddo nothing */
		}
		else
		{
			std::string Name;
			if(CurrentClass->methods[i]->name == "LESS_THAN")
			{
				Name = "LESS";
			} else
			{
				Name = CurrentClass->methods[i]->name;
			}
			Methods->push_back(CurrentClass->methods[i]->name);
			f << ", " << CurrentClass->id << "_method_" << Name << "\n";

		}
	}
	if (CurrentClass->id != "Obj")
	{
		GenerateTheClass(pgm, CurrentClass->s_ptr, Methods, f);
	}
}
void 
GenerateClassFunctionMethods(std::shared_ptr<Program> pgm, 
							 std::shared_ptr<Class> CurrentClass, 
							 std::vector<std::string> *Methods, 
							 std::ofstream &f)
{
	for (unsigned int i=0;i<CurrentClass->methods.size();++i)
	{
		int Found = 0;
		for (unsigned int j=0;j<Methods->size();++j)
		{
			if (CurrentClass->methods[i]->name == Methods->at(j))
			{
				Found = 1;
			}
		}
		if (Found == 1) 
		{
			/* ddo nothing */
		}
		else
		{		
			std::string Name;
			if ( CurrentClass->methods[i]->name == "LESS_THAN")
			{
				Name = "LESS";
			} else 
			{
				Name = CurrentClass->methods[i]->name;
			}
			Methods->push_back(Name);
			/* where things happen */
			f << "obj_" << CurrentClass->methods[i]->rt << " " << CurrentClass->id << "_method_" 
			<< Name << "(obj_" << CurrentClass->id << " this_";
			for (unsigned int j=0;j<CurrentClass->methods[i]->ParameterTypes.size();++j)
			{
				f << ", obj_" << CurrentClass->methods[i]->ParameterTypes[j] 
				<< " " << CurrentClass->methods[i]->ParameterSymbols[j] << "_";				
			}
			f << ") {\n";
			std::shared_ptr<Node> CurrentMethod = CurrentClass->methods[i]->ptr;
			//GenerateInstances(pgm, f);


			f << "void *false_ = lit_false;\n";
			f << "void *true_ = lit_false;\n";
			for (unsigned int j=0;j<CurrentClass->methods[i]->Symbols.size();++j)
			{	
				int Caught = 0;
				for (unsigned int k=0;k<CurrentClass->methods[i]->ParameterSymbols.size();++k)
				{
					if (CurrentClass->methods[i]->Symbols[j] == CurrentClass->methods[i]->ParameterSymbols[k])
					{
						Caught = 1;
					}
				}
				if (Caught == 0)
				{
					if (CurrentClass->methods[i]->Symbols[j] == "true"
						|| CurrentClass->methods[i]->Symbols[j] == "false")
					{
						/* do nothing */
					} else if (CurrentClass->methods[i]->Symbols[j].size() > 5)
					{
						if (CurrentClass->methods[i]->Symbols[j].substr(0,5) == "this.")
						{
						} else
						{
							f << "void *" << CurrentClass->methods[i]->Symbols[j] << "_;\n";
						}
					} else if (CurrentClass->methods[i]->Symbols[j] != "this")
					{
						f << "void *" << CurrentClass->methods[i]->Symbols[j] << "_;\n";
					} else
					{

					}
				}
			}
			for (unsigned int j=0;j<CurrentMethod->v.size();++j)
			{
				if (CurrentMethod->v[j]->type == "STATEMENT_BLOCK")
				{
					/* CurrentMethod->v[j]->v[0] is a STATEMENTS */
					std::vector<std::string> s_ = CurrentClass->methods[i]->Symbols;
					std::vector<std::string> t_ = CurrentClass->methods[i]->Types;
					std::vector<std::string> d_ = CurrentClass->methods[i]->Decs;
					GenerateMethodStmt(CurrentMethod->v[j]->v[0], f, pgm, &s_, &t_, &d_);
				}
			}

			f << "}\n";
			
		}

	}
}

void 
GenerateFd(std::shared_ptr<Program> pgm, std::ofstream &f)
{
	std::string Name;
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" 
		 && pgm->classes->v[i]->id != "Boolean"
		 && pgm->classes->v[i]->id != "Nothing"
		 && pgm->classes->v[i]->id != "String" 
		 && pgm->classes->v[i]->id != "Int")
		{
			Name = pgm->classes->v[i]->id;
			f << "struct class_" << Name << "_struct;\n"
			<< "typedef struct class_" << Name << "_struct* class_" << Name << ";\n"
			<< "struct class_" << Name << "_struct the_class_" << Name << "_struct;\n"
			<< "struct obj_" << Name << "_struct;\n"
			<< "typedef struct obj_" << Name << "_struct* obj_" << Name << ";\n";
		}
	}
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" 
		 && pgm->classes->v[i]->id != "Boolean"
		 && pgm->classes->v[i]->id != "Nothing"
		 && pgm->classes->v[i]->id != "String" 
		 && pgm->classes->v[i]->id != "Int")
		{
			Name = pgm->classes->v[i]->id;
			f << "struct obj_" << Name << "_struct {\n"
			<< "  class_" << Name << " clazz;\n";
			for (unsigned int j=0;j<pgm->classes->v[i]->Symbols.size();++j)
			{	
				int Size_ = pgm->classes->v[i]->Symbols[j].size();
				std::string JustThis;
				std::string WithoutThis;
				if (Size_ > 5)
				{
					JustThis = pgm->classes->v[i]->Symbols[j].substr(0,5);
					if (JustThis == "this.")
					{
						WithoutThis = pgm->classes->v[i]->Symbols[j].substr(5,Size_-1);
						f << "obj_" << pgm->classes->v[i]->Types[j] << " " << WithoutThis << "_;\n";
					}
				}
				
			}
			f << "};\n";
		}
	}
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" 
		 && pgm->classes->v[i]->id != "Boolean"
		 && pgm->classes->v[i]->id != "Nothing"
		 && pgm->classes->v[i]->id != "String" 
		 && pgm->classes->v[i]->id != "Int")
		{
			std::vector<std::string> Methods;
			f << "struct class_" << pgm->classes->v[i]->id << "_struct {\n"
			<< "obj_" << pgm->classes->v[i]->id << " (*constructor) (";  
			for (unsigned int j=0;j<pgm->classes->v[i]->ParameterTypes.size();++j)
			{
				f << "obj_" << pgm->classes->v[i]->ParameterTypes[j];
				if (j < pgm->classes->v[i]->ParameterTypes.size()-1)
				{
					f << ", ";
				}
			}
			f << ");\n";
			GenerateClassStructMethods(pgm->classes->v[i], &Methods, f);
			f << "};\n";

			f << "extern class_" << pgm->classes->v[i]->id << " the_class_" << pgm->classes->v[i]->id << ";\n";
		}
	}
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" 
		 && pgm->classes->v[i]->id != "Boolean"
		 && pgm->classes->v[i]->id != "Nothing"
		 && pgm->classes->v[i]->id != "String" 
		 && pgm->classes->v[i]->id != "Int")
		{
			f << "obj_" << pgm->classes->v[i]->id << " new_" << pgm->classes->v[i]->id << "(";
			for (unsigned int j=0;j<pgm->classes->v[i]->ParameterTypes.size();++j)
			{
				f << "obj_" << pgm->classes->v[i]->ParameterTypes[j] << " "
				<< pgm->classes->v[i]->ParameterSymbols[j] << "_";
				if (j < pgm->classes->v[i]->ParameterTypes.size()-1)
				{
					f << ", ";
				}
			}
			f << ") {\n";
				f << "obj_" << pgm->classes->v[i]->id << " this_ = (obj_" << pgm->classes->v[i]->id << ") "
				<< "GC_MALLOC(sizeof(struct obj_" << pgm->classes->v[i]->id << "_struct));\n"
				<< "this_->clazz = the_class_" << pgm->classes->v[i]->id << ";\n";
				//GenerateInstancesForConstructor(pgm, f, pgm->classes->v[i]);
				for (unsigned int a=0;a<pgm->classes->v[i]->Symbols.size();++a)
				{	
					// TODO REMOVE SYMBOLS THAT DONT BELONG
					// WE ONLY WANT CONSTRUCTOR SYMBOLS, NO 
					// SYMBOLS FROM METHODS OR PARAMETERS!!
					// ADEDNDUM: MAYBE USE rNodeC() then weed out the params
					//
					int Caught = 0;
					for (unsigned int k=0;k<pgm->classes->v[i]->ParameterSymbols.size();++k)
					{
					
						if (pgm->classes->v[i]->Symbols[a] == pgm->classes->v[i]->ParameterSymbols[k])
						{
							Caught = 1;
						}
					}
					if (Caught == 0)
					{
						if (pgm->classes->v[i]->Symbols[a] == "true")
						{
							f << "void *" << pgm->classes->v[i]->Symbols[a] << "_ = lit_true;\n";
						} else if (pgm->classes->v[i]->Symbols[a] == "false")
						{
							f << "void *" << pgm->classes->v[i]->Symbols[a] << "_ = lit_false;\n";
						} else if (pgm->classes->v[i]->Symbols[a].size() > 5)
						{
							if (pgm->classes->v[i]->Symbols[a].substr(0,5) == "this.")
							{
							} else
							{
								f << "void *" << pgm->classes->v[i]->Symbols[a] << "_;\n";
							}
						} else if (pgm->classes->v[i]->Symbols[a] != "this")
						{
							f << "void *" << pgm->classes->v[i]->Symbols[a] << "_;\n";
						} else
						{

						}
					}
				}
				/* constructor business */
				std::vector<std::string> s_ = pgm->classes->v[i]->Symbols;
				std::vector<std::string> t_ = pgm->classes->v[i]->Types;
				std::vector<std::string> d_ = pgm->classes->v[i]->Types;
				for (unsigned int j=0;j<pgm->classes->v[i]->v.size();++j)
				{
					if (pgm->classes->v[i]->v[j]->type == "CLASS_BODY")
					{
						for (unsigned int k=0;k<pgm->classes->v[i]->v[j]->v.size();++k)
						{
							if (pgm->classes->v[i]->v[j]->v[k]->type == "STATEMENTS")
							{
								GenerateMethodStmt(pgm->classes->v[i]->v[j]->v[k], f, pgm, &s_, &t_, &d_);
								break;
							}
						}
					}
				}
				f << "return this_;\n";
			f << "}\n";
			std::vector<std::string> Methods1;
			GenerateClassFunctionMethods(pgm, pgm->classes->v[i], &Methods1, f);
		}
	}
	for (unsigned int i=0;i<pgm->classes->v.size();++i)
	{
		if (pgm->classes->v[i]->id != "Obj" 
		 && pgm->classes->v[i]->id != "Boolean"
		 && pgm->classes->v[i]->id != "Nothing"
		 && pgm->classes->v[i]->id != "String" 
		 && pgm->classes->v[i]->id != "Int")
		{
			std::vector<std::string> Methods2;
			f << "struct class_" << pgm->classes->v[i]->id << "_struct the_class_" 
			<< pgm->classes->v[i]->id << "_struct ={\n new_" << pgm->classes->v[i]->id;
			GenerateTheClass(pgm, pgm->classes->v[i], &Methods2, f);
			f << "};\nclass_" << pgm->classes->v[i]->id << " the_class_" << pgm->classes->v[i]->id 
			<< "= &the_class_" << pgm->classes->v[i]->id << "_struct;\n";
		}
	}
}


int GenerateCode(std::shared_ptr<Program> pgm)
{	
	std::ofstream f;
  	f.open("pgm.c");
  	f << "#include <stdio.h>\n"
	<< "#include <stdlib.h>\n"
	<< "#include \"builtin/Builtins.h\"\n"
	<< "#include \"gc.h\"\n"
	<< "void quackmain(); \n"
	<< "int main(int argc, char** argv) {\n"
	<< "  quackmain();\n"
	<< "  printf(\"\\n--- Terminated successfully ---\\n\");\n"
	<< "  exit(0);\n"
	<< "}\n";
	GenerateFd(pgm, f);
	f << "void quackmain() {\n";
	GenerateMain(pgm, f);
	f << "}";
  	f.close();
  	system("gcc -I/home/gc/include pgm.c builtin/Builtins.c /home/gc/lib/libgc.a -o qk.out"); /* DEBUGGING: comment this out and use run.sh */
	return 0;
}

