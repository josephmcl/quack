#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "ast.hpp"
#include <algorithm>

int NumberOfCaughtErrors = 0;

Met::Met()
{

}
Met::~Met()
{
}
Node::Node()
{

}
Node::~Node()
{

}
void Node::printAttr()
{
   std::cout << this->type << "\n{" << std::endl;
   if (this->v.size() > 0)
   {
      for (unsigned i=0;i<0;i++)
      {
         this->v[i]->printAttr();
      }
   }
   std::cout << "}" << std::endl;
}
void Node::travelTree(std::vector<Node*> &b, int d, std::vector<std::string> &sym_n)
{
   /*
   for(int i=0;i<d;i++)
   {
      std::cout << "  ";
   }
   std::cout << this->type;
   
   this->repr();
   std::cout << std::endl;
   */
   if (this->type == "STMT_ASSIGN")
   {
      for(unsigned int i=0;i<v.size();++i)
      {
         if(v[i]->type == "L_EXPR")
         {
            sym_n.push_back(v[i]->t1);
         }
      }
   }
   if (this->type =="CONS")
   {
      b.push_back(this);
   }
   if (this->v.size() > 0)
   {
      for(unsigned int i=0;i<v.size();++i)
      {
         v[i]->travelTree(b, d+1, sym_n);
      }
   }
   return;
}
void Node::repr()
{
   std::cout << " " << this->srepr;
}
/*
void Node::assembleST()
{
   for(int i=0;i<v->size();++i)
   {

   }
}
*/
/*
*/
Statements::Statements()
{
}
Statements::~Statements()
{
}
void Statements::printAttr()
{
   std::cout << "statement node" << std::endl;
}
/*
*/
Statement::Statement(std::string name)
{
   this->name = name;
}
Cons::Cons()
{

}
Cons::~Cons()
{
}
int Cons::findClass(std::vector<std::string> &v, std::vector<std::shared_ptr<Class>> &vp)
{
   for(unsigned i=0;i<v.size();++i)
   {  
      if(this->id==v[i])
      {
         this->s_ptr = vp[i];
         return 1;
      }
      
   }
   return 0;
   
}
int Cons::printAttri()
{
   return 1;
}
Class::Class()
{

}
Class::~Class()
{

}
void Class::printAttr()
{
   std::cout << "Class node" << std::endl;
}
int Class::printAttri()
{
   if(this->id=="Obj")
   {  
      //std::cout << "    <-- [" << this->id << "]" << std::endl; 
      return 1;
   }
   std::vector<std::string> used_v;
   std::vector<std::string> v;
   std::shared_ptr<Class> rv = this->s_ptr;
   int ind = 1;
   while(rv->id!="Obj")
   {
      ind+=1; 
      used_v.insert(used_v.begin(), rv->id);
      v.insert(v.begin(), rv->id);
      rv = rv->s_ptr;
      for(unsigned i=0;i<v.size();i++)
      {
         if(this->id==v[i])
         {
            std::cerr << "Circular dependence detected on "<<this->s_id<<" <-- [" << this->id << "]" << std::endl;
            return 0;
         }
         for(unsigned j=1;j<used_v.size();j++)
         {
            if(used_v[j]==rv->id)
            {
               std::cerr << "Circular dependence detected on "<<used_v[j]<<" <-- [" << this->id << "]" << std::endl;
               return 0;
            }

         }
      }
   }
   v.insert(v.begin(), rv->id);
   rv = rv->s_ptr;
   //for(int i=0;i<ind;i++){std::cout<< v[i] <<" <-- ";}
   //std::cout << "[" << this->id << "]" << std::endl;
   return 1;
}
void Class::travelTree(std::vector<Node*> &b, int d, std::vector<std::string> &sym_n) 
{
   /*
   for(int i=0;i<d;i++)
   {
      std::cout << "  ";
   }
  std::cout << this->type << " " << this->id << std::endl;
  */
   
   if (this->type == "STMT_ASSIGN")
   {
      for(unsigned int i=0;i<v.size();++i)
      {
         if(v[i]->type == "L_EXPR")
         {
            sym_n.push_back(v[i]->t1);
         }
      }
   }

   if (this->type =="CONS")
   {
      b.push_back(this);
   }
   if (this->v.size() > 0)
   {
      for(unsigned int i=0;i<v.size();++i)
      {
         v[i]->travelTree(b, d+1, this->sym_n);
      }
   }
   return;
}

void Class::assembleMethods()
{

   for (unsigned int i=0;i<this->v.size();++i)
   {
      if (this->v[i]->type == "FORMAL_ARG")
      {
      this->ParameterSymbols.push_back(this->v[i]->t1); /* variable */
      this->ParameterTypes.push_back(this->v[i]->t3); /* type */
      }

      if(this->v[i]->type == "CLASS_BODY")
      {
         std::shared_ptr<Node> t = this->v[i]; 
         for(unsigned int j=0;j<t->v.size();++j)
         {
            if( this->v[i]->v[j]->type == "METHOD")
            {
               std::shared_ptr<Met> NewMethod (new Met());
               NewMethod->name = t->v[j]->t1;
               NewMethod->n = t->v[j]->v.size();
               NewMethod->ptr = this->v[i]->v[j];
               NewMethod->ClassType = this->id;
               std::shared_ptr<Node> y = t->v[j];
               for(unsigned int k = 0;k < y->v.size();++k)
               {
                  if( y->v[k]->type == "FORMAL_ARG")
                  {
                     NewMethod->ParameterSymbols.push_back(y->v[k]->t1);
                     NewMethod->ParameterTypes.push_back(y->v[k]->t3);
                  }
                  if( y->v[k]->type == "TYPE_ID")
                  {
                     NewMethod->rt = y->v[k]->t1;
                  }
               }
               this->methods.push_back(NewMethod);
            }
         }
         break;
      }
   }

}
/*void Class::assembleST()
{
   for(int i=0;i<this->v.size();++i)
   {
      if(v[i]!="METHOD")
   }
*/
/*
 *
 */
Classes::Classes(){}
Classes::~Classes(){}
void Classes::printAttr()
{  
   int valid = 1;
   //std::cerr<<"Class Hierarchy:"<<std::endl;
   for(auto const& value: this->v) 
   {
      if ((valid = value->printAttri())==0)
      {  
         std::cerr<<"Class hierarchy not well-formed."<<std::endl;
         return;
      }
   }
   //std::cerr<<"Class hierarchy is well-formed."<<std::endl;
   return;
}

int Classes::buildTree()
{
   int valid = 0;
   for(unsigned i=0; i < this->keys.size()-1; i++) 
   {  
      for(unsigned j=0; j < this->keys.size(); j++) 
      {
         if(this->v[i]->s_id == this->v[j]->id)
         {  
            this->v[i]->s_ptr = this->v[j];
            valid = 1;
            break;
         }
       }
      if(valid==0)
      {
         std::cerr << "Undefined class \'" << this->v[i]->s_id << "\' extended by \'" << this->v[i]->id << "\'"<< std::endl;
         return 0;
      }
      valid = 0;
   }
   this->printAttr();
   return 1;

}

std::string
Classes::GetCommonSuper(std::shared_ptr<Class> FirstClass, 
                        std::shared_ptr<Class> SecondClass)
{  
   std::shared_ptr<Class> SecondClassIterator;
   int i = 0;
   while (i<this->v.size())
   {  
      SecondClassIterator = SecondClass;
      int j = 0;
      while (j<this->v.size())
      {
         if (FirstClass->id == SecondClassIterator->id)
         {
            return FirstClass->id;
         }
         if (SecondClassIterator->id == "Obj")
         {
            break;
         }
         SecondClassIterator = SecondClassIterator->s_ptr;
         ++j;
      }
      if (FirstClass->id == "Obj")
      {
         break;
      }
      ++i;
      FirstClass = FirstClass->s_ptr;
   }
   return "ERR";
}
std::string
Classes::GetCommonSuperFromString(
            std::string FirstClass, 
            std::string SecondClass)
{  std::shared_ptr<Class> FCPtr;
   std::shared_ptr<Class> SCPtr;

   if (FirstClass == "Bottom")
   {
      return SecondClass;
   }
   if (SecondClass == "Bottom")
   {
      return FirstClass;
   }

   for(unsigned int i=0;i<this->v.size();++i)
   {
      if (this->v[i]->id == FirstClass)
      {
         FCPtr = this->v[i];
      }
      if (this->v[i]->id == SecondClass)
      {
         SCPtr = this->v[i];
      }
   }
   return this->GetCommonSuper(FCPtr, SCPtr);
}


Program::Program()
{
   this->classes = std::shared_ptr<Classes>(new Classes());
   this->statements = std::shared_ptr<Statements>(new Statements());

}
Program::Program(Node c)
{
   this->c = c;
}
Program::~Program()
{

}
void Program::printAttr()
{
   std::cout << "Program <--" << std::endl;
}

void Program::printSymbolTable()
{
   std::cout << "Global Symbols: " << std::endl;

   for (unsigned int i=0;i<this->sym_table.size();++i)
   {
      std::cout << " " << this->sym_table[i] << std::endl;
   }
   for (unsigned int i=0;i<this->classes->v.size();++i)
   {
      std::cout << "Class: " << this->classes->v[i]->id << std::endl;
      std::cout << " Parameters:" << std::endl;
      for (unsigned int j=0;j<this->classes->v[i]->ParameterTypes.size();++j)
      {
         std::cout << "  " << this->classes->v[i]->ParameterSymbols[j] << " : " <<
         this->classes->v[i]->ParameterTypes[j] << std::endl;
      }
      std::cout << " Symbols:" << std::endl;
      for (unsigned int j=0;j<this->classes->v[i]->sym_table.size() ;++j)
      {
         std::cout << "  " << this->classes->v[i]->sym_table[j] << std::endl;
      }
      for (unsigned int j=0;j<this->classes->v[i]->methods.size();++j)
      {
         std::cout << "   Method: " << this->classes->v[i]->methods[j]->name << std::endl;
         std::cout << "    Parameters:" << std::endl;
         for (unsigned int k=0;k<this->classes->v[i]->methods[j]->ParameterTypes.size() ;++k)
         {
            std::cout << "     " << this->classes->v[i]->methods[j]->ParameterSymbols[k]
            << " : " << this->classes->v[i]->methods[j]->ParameterTypes[k] << std::endl;
         }
         std::cout << "    Symbols: " << std::endl;
         for (unsigned int k=0;k<this->classes->v[i]->methods[j]->ptr->sym_table.size() ;++k)
         {
            std::cout << "     " << this->classes->v[i]->methods[j]->ptr->sym_table[k] << std::endl;
         }
         
      }
   }
}

int rNode(std::vector<std::string> *symbol_table, std::shared_ptr<Node> node, int clean)
{
   if (node->type == "L_EXPR")
   {
      if(clean == 0)
      {
         std::string nv = node->t1 + "." + symbol_table->back();
         symbol_table->pop_back();
         symbol_table->push_back(nv);
         clean = 1;
      }
      else if(clean == 1)
      {
         symbol_table->push_back(node->t1);
      }
   }
   else if (node->type == "MEM_L_EXPR")
   {
      symbol_table->push_back(node->t3);
      
      clean = 0;
   }
   else {}
   for(int i=0;i<node->v.size();++i)
   {
      
      clean = rNode(symbol_table, node->v[i], clean);
   }
   return clean; 
}

int rNodeC(std::vector<std::string> *symbol_table, std::shared_ptr<Class> node, int clean)
{
   for(int i=0;i<node->v.size();++i)
   {
      if(node->v[i]->type != "METHOD")
      {
         clean = rNode(symbol_table, node->v[i], clean);
      }
   }
   return clean; 
}

std::vector<std::string>
remove_dups(std::vector<std::string> v)
{

   for(unsigned int k=0;k<v.size();++k)
   {
      for(unsigned int l=0;l<k;++l)
      {
         if (v[k] == v[l])
         {  
            v.erase(v.begin()+l);
         }
      }
      for(unsigned int l=k+1;l<v.size();++l)
      {  
         if (v[k] == v[l])
         {
            v.erase(v.begin()+l);
         }
      }
   }
   return v;
}

std::vector<std::string> 
Intersection(std::vector<std::string> &v1, std::vector<std::string> &v2)
{
    std::vector<std::string> rv;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),std::back_inserter(rv));

    return rv;
}

std::vector<std::string>
s_trav(std::vector<std::string> init, std::shared_ptr<Node> n)
{

   //std::cout << n->type << std::endl;
   for(unsigned int i=0;i<init.size();++i)
   {
      //std::cout << init[i] << std::endl;
   }
   if (n->type == "STATEMENTS")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENTS")
         {
            init = s_trav(init, n->v[i]);
            break;
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT" || n->v[i]->type == "STMT_ASSIGN" 
             || n->v[i]->type == "STMT_IF"|| n->v[i]->type == "STMT_WHILE")
         {
            std::vector<std::string> temp = s_trav(init, n->v[i]);
            init.insert(init.end(), temp.begin(), temp.end());

            init = remove_dups(init);
         }
      }
   }
   if (n->type == "STATEMENT")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ACTUAL_ARG")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_ASSIGN")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }

      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "L_EXPR")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "MEM_L_EXPR")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_IF")
   {
      std::vector<std::string> PriorToIf = init;
      std::vector<std::string> PriorToElif = init;
      std::vector<std::string> PriorToElse = init;
      int NoElse = 0;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            PriorToIf = s_trav(PriorToIf, n->v[i]);
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            PriorToIf = s_trav(PriorToIf, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "SET")
         {
            PriorToElif = s_trav(PriorToElif, n->v[i]); 
            PriorToElif = remove_dups(PriorToElif);
         }
         else if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "UNSET")
         {
            PriorToElif = PriorToIf;
         }
      }
      for (unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELSE" && n->v[i]->t1 == "SET")
         {
            PriorToElse = s_trav(PriorToElse, n->v[i]); 
            PriorToElse = remove_dups(PriorToElse);
         }
      }

      std::vector<std::string> rv1 = Intersection(PriorToIf, PriorToElif);
      std::vector<std::string> rv2 = Intersection(rv1, PriorToElse);

      for (unsigned int i=0;i<rv2.size();++i)
      {
         init.push_back(rv2[i]);
      }
      init = remove_dups(init);
   }
   else if (n->type == "ELIFS")
   {
      std::vector<std::string> PriorToElifs = init;
      std::vector<std::string> PriorToElif = init;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIF")
         {
            PriorToElif = s_trav(PriorToElif, n->v[i]); 
            PriorToElif = remove_dups(PriorToElif);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "SET")
         {
            PriorToElifs = s_trav(PriorToElifs, n->v[i]); 
            PriorToElifs = remove_dups(PriorToElifs);
         }
         else if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "UNSET")
         {
            PriorToElifs = PriorToElif;
         }
      }
      std::vector<std::string> rv = Intersection(PriorToElifs, PriorToElif);

      for (unsigned int i=0;i<rv.size();++i)
      {
         init.push_back(rv[i]);
      }
      init = remove_dups(init);
   }
   else if (n->type == "ELIF")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            init = s_trav(init, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "ELSE")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            init = s_trav(init, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_WHILE")
   {
      std::vector<std::string> PriorToIf = init;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            PriorToIf = s_trav(PriorToIf, n->v[i]);
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            PriorToIf = s_trav(PriorToIf, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      if (init.size() < PriorToIf.size())
      {
         std::cerr << n->ln << " : ";
         std::cerr << "Initialization Error: \'" 
         << PriorToIf[PriorToIf.size()-1] 
         << "\' initialized in \'while\' block." 
         << std::endl;
         exit(-1);
      }
   }
   else if (n->type == "L_EXPR")
   {

      init.push_back(n->t1);
   }
   else if (n->type == "MEM_L_EXPR")
   {
      std::string pbv;
      int found = 0;
      for (unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {

            for (unsigned int j=0;j<n->v[i]->v.size();++j)
            {
               if (n->v[i]->v[j]->type == "L_EXPR")
               {  

                  pbv = n->v[i]->v[j]->t1 + "." + n->t1;
                  std::cerr << n->ln << " : ";
                  std::cerr << "Initialization Error: class field \'" <<
                  pbv << "\' referenced outside of class def." << std::endl;
                  exit(-1);
               }
            }
         }
      }
   }
   else if (n->type == "ACTUAL_ARG")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "R_EXPR" || n->type == "CONS")
   {

      std::string err;
      
      int found = 0;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ACTUAL_ARG")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
         if (n->v[i]->type == "MEM_L_EXPR")
         {
            init = s_trav(init, n->v[i]);
            init = remove_dups(init);
         }

         if (n->v[i]->type == "L_EXPR")
         {

            //std::cout << "checking " << n->v[i]->srepr << std::endl;
            err = n->v[i]->srepr;
            found = 1;
            for (unsigned int j=0;j<init.size();++j)
            {
               //std::cout << init[j] << std::endl;
            }
            for (unsigned int j=0;j<init.size();++j)
            {
               if (n->v[i]->srepr == init[j])
               {  
                  found = 2;
                  //std::cout << "found " << err <<"!!!" << std::endl; 
                  break;
               }

            }
            if(found == 1)
            {
               std::cerr << n->ln << " : ";
               std::cerr << "Type error: \'" << err << "\' not initalized." << std::endl;
               exit(-1);
            }
         }
      }
   }
   else
   {}
   return init;
}


std::vector<std::string>
class_s_trav(std::vector<std::string> init, std::shared_ptr<Node> n)
{
   if (n->type == "STATEMENTS")
   {
      for(int i=0;i<n->v.size();++i)
      {  
         if (n->v[i]->type == "STATEMENTS")
         {
            init = class_s_trav(init, n->v[i]);
            break;
         }
      }
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT" || n->v[i]->type == "STMT_ASSIGN"
             || n->v[i]->type == "STMT_IF"|| n->v[i]->type == "STMT_WHILE"
             || n->v[i]->type == "STMT_T_RETURN")
         {
            std::vector<std::string> temp = class_s_trav(init, n->v[i]);
            init.insert(init.end(), temp.begin(), temp.end());

            init = remove_dups(init);
            
         }
      }
   }
   if (n->type == "STATEMENT")
   {
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ACTUAL_ARG")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_ASSIGN")
   {
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "L_EXPR")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "MEM_L_EXPR")
         {

            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_IF")
   {
      std::vector<std::string> PriorToIf = init;
      std::vector<std::string> PriorToElif = init;
      std::vector<std::string> PriorToElse = init;
      int NoElse = 0;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            PriorToIf = class_s_trav(PriorToIf, n->v[i]);
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            PriorToIf = class_s_trav(PriorToIf, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "SET")
         {
            PriorToElif = class_s_trav(PriorToElif, n->v[i]); 
            PriorToElif = remove_dups(PriorToElif);
         }
         else if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "UNSET")
         {
            PriorToElif = PriorToIf;
         }
      }
      for (unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELSE" && n->v[i]->t1 == "SET")
         {
            PriorToElse = class_s_trav(PriorToElse, n->v[i]); 
            PriorToElse = remove_dups(PriorToElse);
         }
      }

      std::vector<std::string> rv1 = Intersection(PriorToIf, PriorToElif);
      std::vector<std::string> rv2 = Intersection(rv1, PriorToElse);

      for (unsigned int i=0;i<rv2.size();++i)
      {
         init.push_back(rv2[i]);
      }
      init = remove_dups(init);
   }
   else if (n->type == "ELIFS")
   {
      std::vector<std::string> PriorToElifs = init;
      std::vector<std::string> PriorToElif = init;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIF")
         {
            PriorToElif = class_s_trav(PriorToElif, n->v[i]); 
            PriorToElif = remove_dups(PriorToElif);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "SET")
         {
            PriorToElifs = class_s_trav(PriorToElifs, n->v[i]); 
            PriorToElifs = remove_dups(PriorToElifs);
         }
         else if (n->v[i]->type == "ELIFS" && n->v[i]->t1 == "UNSET")
         {
            PriorToElifs = PriorToElif;
         }
      }
      std::vector<std::string> rv = Intersection(PriorToElifs, PriorToElif);

      for (unsigned int i=0;i<rv.size();++i)
      {
         init.push_back(rv[i]);
      }
      init = remove_dups(init);
   }
   else if (n->type == "ELIF")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            init = class_s_trav(init, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "ELSE")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            init = class_s_trav(init, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "STMT_WHILE")
   {
      std::vector<std::string> PriorToIf = init;
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            PriorToIf = class_s_trav(PriorToIf, n->v[i]);
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "STATEMENT_BLOCK" && n->v[i]->v.size() > 0)
         {
            PriorToIf = class_s_trav(PriorToIf, n->v[i]->v[0]); /*v[0] is STATEMENTS*/
            PriorToIf = remove_dups(PriorToIf);
         }
      }
      if (init.size() < PriorToIf.size())
      {
         std::cerr << n->ln << " : ";
         std::cerr << "Initialization Error: \'" 
         << PriorToIf[PriorToIf.size()-1] 
         << "\' initialized in \'while\' block." 
         << std::endl;
         exit(-1);
      }
   }
   else if (n->type == "STMT_T_RETURN")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "L_EXPR")
   {

      init.push_back(n->t1);
   }
   else if (n->type == "MEM_L_EXPR")
   {
      //TODO make left IDENT exclusive to in init or is this
      std::string pbv;
      std::string fdv;
      int found = 0;
      for (unsigned int i=0;i<n->v.size();++i)
      {
         if (found == 1)
         {
            break;
         }
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            for (unsigned int j=0;j<n->v[i]->v.size();++j)
            {
               if (n->v[i]->v[j]->type == "L_EXPR")
               {  
                  fdv = n->v[i]->v[j]->t1;
                  pbv = n->v[i]->v[j]->t1 + "." + n->t1;
                  found = 1;
                  break;
               }
            }
         }
      }
      if (fdv != "this" )
      {
         std::cerr << n->ln << " : ";
         std::cerr << n->ln << " : Initialization Error:"
         << "class field " << pbv << " referenced outside of class def." << std::endl;
         exit(-1);
      }

      init.push_back(pbv);
   }
   else if (n->type == "ACTUAL_ARG")
   {
      for(unsigned int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
   }
   else if (n->type == "R_EXPR" || n->type == "CONS")
   {
      std::string err;
      
      int found = 0;
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "ACTUAL_ARG")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "R_EXPR" || n->v[i]->type == "CONS")
         {
            init = class_s_trav(init, n->v[i]);
            init = remove_dups(init);
         }
      }
      for(int i=0;i<n->v.size();++i)
      {
         if (n->v[i]->type == "L_EXPR")
         {

            //std::cout << "checking " << n->v[i]->srepr << std::endl;
            err = n->v[i]->srepr;
            found = 1;
            for (int j=0;j<init.size();++j)
            {
               //std::cout << init[j] << std::endl;
            }
            for (int j=0;j<init.size();++j)
            {
               
               if (n->v[i]->srepr == init[j])
               {  
                  found = 2;
                  //std::cout << "found " << err <<"!!!" << std::endl; 
                  break;
               }

            }
            if(found == 1)
            {
               std::cerr << n->ln << " : ";
               std::cerr << "Initialization Error: \'" << err << "\' not initalized." << std::endl;
               exit(-1);
            }
         }
         if (n->v[i]->type == "MEM_L_EXPR")
         {
            std::string pbv;
            std::string root;
            int found = 0;
            for (unsigned int j=0;j<n->v[i]->v.size();++j)
            {
               if (n->v[i]->v[j]->type == "R_EXPR" || n->v[i]->type == "CONS")
               {
                  for (unsigned int k=0;n->v[i]->v[j]->v.size();++j)
                  {
                     if (n->v[i]->v[j]->v[k]->type == "L_EXPR")
                     {  
                        pbv = n->v[i]->v[j]->v[k]->t1 + "." + n->v[i]->t1;
                        root = n->v[i]->v[j]->v[k]->t1;
                        if (root != "this" )
                        {
                           std::cerr << n->ln << " : ";
                           std::cerr << n->ln << " : Initialization Error:"
                           << "class field " << pbv << " referenced outside of class def." << std::endl;
                           exit(-1);
                        }
                        found = 1;
                        break;
                     }
                  }
               }
               if (found == 1)
               {
                  int is_in = 0;
                  for (unsigned int k=0;k<init.size();++k)
                  {
                     if (pbv == init[k] || root == init[k])
                     {  
                        is_in = 1;
                        break;
                     }
                  }
                  if(is_in == 0)
                  {
                     std::cerr << n->ln << " : ";
                     std::cerr << "Initialization Error: \'" << pbv 
                     << "\' not initalized in " << n->type << std::endl;
                     exit(-1);
                  }
                  break;
               }
            }
            
         }
      }
   }
   return init;
}

std::vector<std::string>
GetMethodFormalArguments_Init(std::vector<std::string> InitalizedValues, std::shared_ptr<Node> Method)
{
   for(unsigned int i=0;i<Method->v.size();++i)
   {
      if(Method->v[i]->type == "FORMAL_ARG")
      {
         InitalizedValues.push_back(Method->v[i]->t1);
      }
   }
   for(unsigned int i=0;i<Method->v.size();++i)
   {
      {
         for(unsigned int j=0;j<Method->v[i]->v.size();++j)
         {  
            if(Method->v[i]->v[j]->type == "STATEMENTS")
            {
               InitalizedValues = class_s_trav(InitalizedValues, Method->v[i]->v[j]);
               break;
            }
         }
      }
   }
   return InitalizedValues; 
}

std::vector<std::string>
GetClassFormalArguments_Init(std::vector<std::string> InitalizedValues, std::shared_ptr<Class> c)
{
   for(unsigned int i=0;i<c->v.size();++i)
   {
      if(c->v[i]->type == "FORMAL_ARG")
      {
         InitalizedValues.push_back(c->v[i]->t1);
      }
   }
   for(unsigned int i=0;i<c->v.size();++i)
   {
      if(c->v[i]->type == "CLASS_BODY")
      {
         for(unsigned int j=0;j<c->v[i]->v.size();++j)
         {  
            if(c->v[i]->v[j]->type == "STATEMENTS")
            {
               InitalizedValues = class_s_trav(InitalizedValues, c->v[i]->v[j]);
               break;
            }
         }
         std::vector<std::string> MethodAccessable = InitalizedValues;
 
         for(unsigned int i=0;i<MethodAccessable.size();++i)
         {
            if (MethodAccessable[i].length() < 5 
                && MethodAccessable[i]!="true"
                && MethodAccessable[i]!="false")
            {
               MethodAccessable.erase(MethodAccessable.begin()+i);
               --i;
            }
            else
            { 
               if (MethodAccessable[i].substr(0,5) != "this." 
                   && MethodAccessable[i]!="true"
                   && MethodAccessable[i]!="false")
               {
                  MethodAccessable.erase(MethodAccessable.begin()+i);
                  --i;
               }
            }
         }
         /* sneaky -- get fields here */
         c->Fields = MethodAccessable;
         for(unsigned int i=0;i<c->Fields.size();++i)
         {
            if (c->Fields[i]=="true"
                || c->Fields[i]=="false")
            {
               c->Fields.erase(c->Fields.begin()+i);
               --i;
            }
         }

         for(unsigned int j=0;j<c->v[i]->v.size();++j)
         {  
            if(c->v[i]->v[j]->type == "METHOD")
            {
               GetMethodFormalArguments_Init(MethodAccessable, c->v[i]->v[j]);
            }
         }
      }
   }
   return InitalizedValues;
}

std::shared_ptr<Met>
MethodisValid(std::shared_ptr<Class> CurrentClass, std::string MethodName)
{
   int ValidMethod=0;
   std::shared_ptr<Met> ReturnMethod (new Met()); 
   for (unsigned int j=0;j<CurrentClass->methods.size();++j)
   {
      if (CurrentClass->methods[j]->name == MethodName)
      {
         ReturnMethod = CurrentClass->methods[j];
         ValidMethod=1;
      }
   }
   if (ValidMethod == 1)
   {
      return ReturnMethod;
   }
   else
   {  
      if (CurrentClass->id == "Obj")
      {

         ReturnMethod->name = "BadMethod";
         return ReturnMethod;
      }
      else
      {
         return MethodisValid(CurrentClass->s_ptr, MethodName);
      }
   }  
}

int
MethodParamValidator( std::vector<std::string> ArgTypes, 
                      std::shared_ptr<Program> pgm,
                      std::shared_ptr<Met> _Method)
{
   if (ArgTypes.size() != _Method->ParameterTypes.size())
   {
      std::cerr << _Method->ptr->ln << " : ";
      std::cerr << "Method " << _Method->name << " expected " <<  
      _Method->ParameterTypes.size() << " arguments. Got " <<
      ArgTypes.size() << " instead." << std::endl;
      exit(-1);
   }
   for(unsigned int i=0;i<ArgTypes.size();++i)
   {
      std::string rv_ = pgm->classes->GetCommonSuperFromString(ArgTypes[i], _Method->ParameterTypes[i]);
      if (_Method->ParameterTypes[i] != rv_)
      {
         std::cerr << _Method->ptr->ln << " : ";
         std::cerr << "Method " << _Method->name << " parameter " << (ArgTypes.size() - i) 
         << " expected type " << _Method->ParameterTypes[i] << " got " 
         << ArgTypes[i] << " instead." << std::endl;
         exit(-1);
      }
   }
   return 1;
}

std::string
StatementsTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared)
{
   if (Node->type == "STATEMENTS")
   {
      for(int i=0;i<Node->v.size();++i)
      {  
         if (Node->v[i]->type == "STATEMENTS")
         {
            StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }

      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
         {
            StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STATEMENT")
   {
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STMT_WHILE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "While condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "STMT_ASSIGN")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      for(int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      /* set check declared type */
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "TYPE_ID" && Node->v[i]->t2 == "DEFINED")
         {
            //std::string LeftType = Node->v[i]->t1;
            //DeclaredType = pgm->classes->GetCommonSuperFromString(LeftType, Types->at(SymbolIndex));
            Declared->at(SymbolIndex) = Node->v[i]->t1;
         }
      }
      std::string temp = pgm->classes->GetCommonSuperFromString(Type, Declared->at(SymbolIndex));
      if (temp != Declared->at(SymbolIndex))
      {
         std::cerr << Node->ln << " : ";
         std::cerr << "Type Error: Type " << temp << " to be assigned to " << Symbol << 
         " lower than declared type " << Declared->at(SymbolIndex) << std::endl;
         exit(-1);
      }

      if (Type == "ERR" )
      {
         return "Complete";
      }
      if (Types->at(SymbolIndex) == "Bottom" )
      {
         Types->at(SymbolIndex) = Type;
      }
      else
      {
         Type = pgm->classes->GetCommonSuperFromString(Type, Types->at(SymbolIndex));
         Types->at(SymbolIndex) = Type;
      }

      //std::cout << "Symbol: " << Symbol << " got type: " << Types->at(SymbolIndex) << std::endl;

      std::string rv = "Complete";
      return rv;
   }
   else if (Node->type == "STMT_IF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "ELIFS")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELIF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELSE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "INT_LIT")
   {
      std::string rv = "Int";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "STRING_LIT")
   {
      std::string rv = "String";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Left Expression")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* Extract object symbol         */
      for(unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      if(Symbol == "true" || Symbol == "false" )
      {
         return "Boolean";
      }
      for(unsigned int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      return Types->at(SymbolIndex);
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "METHOD_CALL")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* type checking for method call */
      /* Extract        type           */
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {

            Type = StatementsTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      /* Check if method is "of" type  */
      int ValidMethod=0;
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == Type)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == Node->t1)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;
               for (unsigned int j=0;j<Node->v.size();++j)
               {
                  if (Node->v[j]->type == "ACTUAL_ARG")
                  {
                     ArgTypes.push_back(StatementsTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                  }
               }
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, Node->t1);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr <<  Node->t1 << " is not a method of class " << Type << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;
                  for (unsigned int j=0;j<Node->v.size();++j)
                  {
                     if (Node->v[j]->type == "ACTUAL_ARG")
                     {
                        ArgTypes.push_back(StatementsTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                     }
                  }
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }

      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "+")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "ADD";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "-")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "SUB";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "*")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "MUL";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "/")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "DIV";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "==")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {  
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<=")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">=")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Parens")
   {
      return StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "And")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "AND expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Or")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = StatementsTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "OR expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Not")
   {
      std::string LeftType = StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "NOT expression expected a Boolean, got " << 
      LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "CONS")
   {
      std::string ClassName = Node->t1;
      int Valid = 0;
      int ClassIndex;
      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if(pgm->classes->v[i]->id == ClassName)
         {
            ClassIndex = i;
            Valid = 1;
            break;
         }
      }
      if (Valid == 1)
      {
         //std::cout << "Valid Constructor Name: " 
         //<< pgm->classes->v[ClassIndex]->id << std::endl;
      }
      else /* This block should never be reached */
      {
         std::cerr << Node->ln << " : ";
         std::cerr << "Type Error: Invalid constructor name: " 
         << ClassName << std::endl;
         exit(-1);
      }
      Valid = 0;
      if (Node->v.size() != (pgm->classes->v[ClassIndex]->v.size())-1) /* -1 because every Class.v has a class body */
      {
         if (ClassName != "Obj" && ClassName != "Boolean" && ClassName != "Nothing")
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Class Constructor \'" << ClassName 
            << "\' expected " << (pgm->classes->v[ClassIndex]->v.size())-1 
            << " arguments -- Got "<< Node->v.size() << " instead." << std::endl;
            ++NumberOfCaughtErrors;
         } else if (Node->v.size() != 0)
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Class Constructor \'" << ClassName 
            << "\' expected " << (pgm->classes->v[ClassIndex]->v.size())-1 
            << " arguments -- Got "<< Node->v.size() << " instead." << std::endl;
            ++NumberOfCaughtErrors;
         }
      }
      /* //check that params match type 
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if( pgm->Classes == StatementsTypeBubbler(Node->v[i], pgm));
      }
      */
      return ClassName;
   }
   else if (Node->type == "L_EXPR")
   {
   }
   else if (Node->type == "ACTUAL_ARG")
   {
      return StatementsTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else
   {
      std::string rv = "ERR";
      return rv;
   }
}

std::string
MethodTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared)
{
   if (Node->type == "CLASS_BODY")
   {
      for(int i=0;i<Node->v.size();++i)
      {  
         if (Node->v[i]->type == "STATEMENTS")
         {
            MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }

      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STATEMENTS")
   {
      for(int i=0;i<Node->v.size();++i)
      {  
         if (Node->v[i]->type == "STATEMENTS")
         {
            MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }

      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
         {
            MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STMT_T_RETURN")
   {
      std::string TypeToReturn = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string Type;
      std::string Symbol = "return";
      std::string DeclaredType;
      int SymbolIndex;
      for(int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break; 
         }
      }
      DeclaredType = Declared->at(SymbolIndex);
      std::string CommonType = pgm->classes->GetCommonSuperFromString(TypeToReturn, DeclaredType);
      if (CommonType != DeclaredType)
      {
         std::cerr << Node->ln << " : return type expected "
         << DeclaredType << " got " << TypeToReturn << " instead." << std::endl;
         exit(-1);
      }
      else
      {
         /* do nothing we do not set return, just match */
      }
      return "UNUSED";
   }
   else if (Node->type == "STATEMENT" && Node->t2 == "Nothing")/*blank return */
   {

      std::string Type = "Nothing";
      std::string Symbol = "return";
      std::string DeclaredType;
      int SymbolIndex;
      for(int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break; 
         }
      }
      DeclaredType = Declared->at(SymbolIndex);
      std::string CommonType = pgm->classes->GetCommonSuperFromString(Type, DeclaredType);
      if (CommonType != DeclaredType)
      {
         std::cerr << Node->ln << " : return type expected "
         << DeclaredType << " got " << CommonType << " instead." << std::endl;
         exit(-1);
      }
      else
      {
         /* do nothing we do not set return, just match */
      }
      return "UNUSED";
   }
   else if (Node->type == "STATEMENT")
   {
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STMT_WHILE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << "While condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "STMT_ASSIGN")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "MEM_L_EXPR")
         {
            Symbol = Node->v[i]->t1;
            for (unsigned int j=0;j<Node->v[i]->v.size();++j)
            {
               if (Node->v[i]->v[j]->type == "R_EXPR")
               {
                  for (unsigned int k=0;k<Node->v[i]->v[j]->v.size();++k)
                  {
                     if (Node->v[i]->v[j]->v[k]->type == "L_EXPR")
                     {
                        if (Node->v[i]->v[j]->v[k]->t1 == "this")
                        {  

                           Symbol = Node->v[i]->v[j]->v[k]->t1 + "." + Node->v[i]->t1;
                           std::string er = Types->at(SymbolIndex);
                           std::cerr << Node->v[i]->v[j]->ln << " : ";
                           std::cerr << "Type Error: potential assignment to field " << 
                           Symbol << " in method def." << std::endl;
                           exit(-1);
                           ++NumberOfCaughtErrors;
                           return "Obj";
                        }  
                        else
                        {
                           for(unsigned int q=0;q<Symbols->size();++q)
                           {
                              if (Symbols->at(q) == Node->v[i]->v[j]->v[k]->t1)
                              {
                                 SymbolIndex = q;
                                 break;
                              }
                           }
                           std::string er = Types->at(SymbolIndex);
                           std::cerr << Node->v[i]->v[j]->ln << " : ";
                           std::cerr << "Type Error: symbol " << 
                           Node->v[i]->v[j]->v[k]->t1 << "." << Node->v[i]->t1
                           << " referenced outside of class def: " 
                           << er << std::endl;
                           exit(-1);
                           ++NumberOfCaughtErrors;
                           return "Obj";
                        }
                     }
                  }
               }
            }
         }
      }
      for(int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      /* set check declared type */
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "TYPE_ID" && Node->v[i]->t2 == "DEFINED")
         {
            //std::string LeftType = Node->v[i]->t1;
            //DeclaredType = pgm->classes->GetCommonSuperFromString(LeftType, Types->at(SymbolIndex));
            Declared->at(SymbolIndex) = Node->v[i]->t1;
         }
      }
      if(Declared->at(SymbolIndex) != "Bottom" && Type != "Bottom")
      {
         std::string temp = pgm->classes->GetCommonSuperFromString(Type, Declared->at(SymbolIndex));
         if (temp != Declared->at(SymbolIndex))
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Type " << temp << " to be assigned to " << Symbol << 
            " lower than declared type " << Declared->at(SymbolIndex) << std::endl;
            exit(-1);
         }
      }

      if (Type == "ERR" )
      {
         return "Complete";
      }
      if (Types->at(SymbolIndex) == "Bottom" )
      {
         Types->at(SymbolIndex) = Type;
      }
      else
      {
         Type = pgm->classes->GetCommonSuperFromString(Type, Types->at(SymbolIndex));
         Types->at(SymbolIndex) = Type;
      }

      //std::cout << "Symbol: " << Symbol << " got type: " << Types->at(SymbolIndex) << std::endl;

      std::string rv = "Complete";
      return rv;
   }
   else if (Node->type == "STMT_IF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "ELIFS")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELIF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELSE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = MethodTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "INT_LIT")
   {
      std::string rv = "Int";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "STRING_LIT")
   {
      std::string rv = "String";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Left Expression")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* Extract object symbol         */
      for(unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      if(Symbol == "true" || Symbol == "false" )
      {
         return "Boolean";
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "MEM_L_EXPR")
         {
            Symbol = Node->v[i]->t1;
            for (unsigned int j=0;j<Node->v[i]->v.size();++j)
            {
               if (Node->v[i]->v[j]->type == "R_EXPR")
               {
                  for (unsigned int k=0;k<Node->v[i]->v[j]->v.size();++k)
                  {
                     if (Node->v[i]->v[j]->v[k]->type == "L_EXPR")
                     {
                        if (Node->v[i]->v[j]->v[k]->t1 == "this")
                        {  

                           Symbol = Node->v[i]->v[j]->v[k]->t1 + "." + Node->v[i]->t1;
                        }  
                        else
                        {
                           for(unsigned int q=0;q<Symbols->size();++q)
                           {
                              if (Symbols->at(q) == Node->v[i]->v[j]->v[k]->t1)
                              {
                                 SymbolIndex = q;
                                 break;
                              }
                           }
                           std::string er = Types->at(SymbolIndex);
                           std::cerr << Node->v[i]->v[j]->ln << " : ";
                           std::cerr << "Type Error: symbol " << 
                           Node->v[i]->v[j]->v[k]->t1 << "." << Node->v[i]->t1
                           << " referenced outside of class def: " 
                           << er << std::endl;
                           exit(-1);
                        }
                     }
                  }
               }
            }
         }
      }
      for(unsigned int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      return Types->at(SymbolIndex);
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "METHOD_CALL")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* type checking for method call */
      /* Extract        type           */
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {

            Type = MethodTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      /* Check if method is "of" type  */
      int ValidMethod=0;
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == Type)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == Node->t1)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;
               for (unsigned int j=0;j<Node->v.size();++j)
               {
                  if (Node->v[j]->type == "ACTUAL_ARG")
                  {
                     ArgTypes.push_back(MethodTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                  }
               }
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, Node->t1);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr <<  Node->t1 << " is not a method of class " << Type << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;
                  for (unsigned int j=0;j<Node->v.size();++j)
                  {
                     if (Node->v[j]->type == "ACTUAL_ARG")
                     {
                        ArgTypes.push_back(MethodTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                     }
                  }
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }

      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "+")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "ADD";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "-")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "SUB";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "*")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "MUL";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "/")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "DIV";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "==")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {  
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<=")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">=")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Parens")
   {
      return MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "And")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "AND expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Or")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = MethodTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "OR expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Not")
   {
      std::string LeftType = MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "NOT expression expected a Boolean, got " << 
      LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "CONS")
   {
      std::string ClassName = Node->t1;
      int Valid = 0;
      int ClassIndex;
      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if(pgm->classes->v[i]->id == ClassName)
         {
            ClassIndex = i;
            Valid = 1;
            break;
         }
      }
      if (Valid == 1)
      {
         //std::cout << "Valid Constructor Name: " 
         //<< pgm->classes->v[ClassIndex]->id << std::endl;
      }
      else /* This block should never be reached */
      {
         std::cerr << Node->ln << " : ";
         std::cerr << "Type Error: Invalid constructor name: " 
         << ClassName << std::endl;
         exit(-1);
      }
      Valid = 0;
      if (Node->v.size() != (pgm->classes->v[ClassIndex]->v.size())-1) /* -1 because every Class.v has a class body */
      {
         if (ClassName != "Obj" && ClassName != "Boolean" && ClassName != "Nothing")
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Class Constructor \'" << ClassName 
            << "\' expected " << (pgm->classes->v[ClassIndex]->v.size())-1 
            << " arguments -- Got "<< Node->v.size() << " instead." << std::endl;
            ++NumberOfCaughtErrors;
         } else if (Node->v.size() != 0)
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Class Constructor \'" << ClassName 
            << "\' expected " << (pgm->classes->v[ClassIndex]->v.size())-1 
            << " arguments -- Got "<< Node->v.size() << " instead." << std::endl;
            ++NumberOfCaughtErrors;
         }
      }
      /* //check that params match type 
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if( pgm->Classes == StatementsTypeBubbler(Node->v[i], pgm));
      }
      */
      return ClassName;
   }
   else if (Node->type == "L_EXPR")
   {
   }
   else if (Node->type == "MEM_L_EXPR")
   { 
   }
   else if (Node->type == "ACTUAL_ARG")
   {
      return MethodTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else
   {
      std::string rv = "ERR";
      return rv;
   }
}

void 
MethodTypeBubblerInit(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Class> CurrentClass, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared)
{
   for (unsigned int i=0;i<CurrentClass->methods.size();++i)
   {
      std::shared_ptr<Met> CurrentMethod = CurrentClass->methods[i];
      std::vector<std::string> MethodSymbols = CurrentMethod->ptr->sym_table;
      std::vector<std::string> MethodTypes;
      std::vector<std::string> MethodTypes1;
      std::vector<std::string> MethodDecs;
      std::shared_ptr<Node> MethodStatements;
      /* this symbol set gets passed to MethodTypeBubbler */
      /* Assemble Method's symbols 
       * Union :
       *    CurrentMethod->ParameterSymbols
       *    CurrentMethod->ptr->sym_table
       *    "this", "true", "false", "return"
       * Assign:
       *    Parametertypes
       *    ReturnType 
       *    "this"=CurrentObj
       *    "true", "false" = Boolean
       */



      for (unsigned int j=0;j<CurrentMethod->ParameterSymbols.size();++j)
      {
         MethodSymbols.push_back(CurrentMethod->ParameterSymbols[j]);
      }

      MethodSymbols = remove_dups(MethodSymbols);
      MethodTypes = MethodSymbols;
      MethodDecs = MethodSymbols;

      for (unsigned int j=0;j<MethodSymbols.size();++j)
      {
         
         int Set = 0;
         for (unsigned int k=0;k<Symbols->size();++k)
         {
            if (MethodSymbols[j] == Symbols->at(k))
            {  
               Set = 1;
               MethodTypes[j]= Types->at(k);
               MethodDecs[j] = Types->at(k); 
            }
         }
         for (unsigned int k=0;k<CurrentMethod->ParameterSymbols.size();++k)
         {
            if (MethodSymbols[j] == CurrentMethod->ParameterSymbols[k])
            {  
               Set = 1;
               MethodTypes[j] = CurrentMethod->ParameterTypes[k];
               MethodDecs[j] = CurrentMethod->ParameterTypes[k]; 
            }
         }
         if (Set == 0 )
         {
            MethodTypes[j]  = "Bottom";
            MethodDecs[j] = "Obj";
         }
      }
      MethodSymbols.push_back("this");
      MethodSymbols.push_back("true");
      MethodSymbols.push_back("false");
      MethodSymbols.push_back("return");

      MethodTypes.push_back(CurrentClass->id);
      MethodTypes.push_back("Boolean");
      MethodTypes.push_back("Boolean");
      MethodTypes.push_back(CurrentMethod->rt);

      MethodDecs.push_back(CurrentClass->id);
      MethodDecs.push_back("Boolean");
      MethodDecs.push_back("Boolean");
      MethodDecs.push_back(CurrentMethod->rt);

      for (unsigned int j=0;j<CurrentMethod->ptr->v.size();++j)
      {
         if (CurrentMethod->ptr->v[j]->type == "STATEMENT_BLOCK")
         {
            MethodStatements = CurrentMethod->ptr->v[j]->v[0];
         }
      }
      
      int Matching   = 0;
      int Cycles     = 0;
      while (Matching == 0)
      {
         /*
         for (int q=0;q<MethodSymbols.size();++q)
         {
            std::cout <<MethodSymbols[q] << " : " 
            <<MethodTypes[q] << " : " 
            <<MethodDecs[q] << std::endl;
            
         }
         */
         
         MethodTypeBubbler(&MethodSymbols, &MethodTypes, 
                MethodStatements, pgm, &MethodDecs);
         if (MethodTypes1 == MethodTypes)
         {
            Matching = 1;
         }
         MethodTypes1 = MethodTypes;
         ++Cycles;

      

      }
      CurrentMethod->Symbols = MethodSymbols;
      CurrentMethod->Types = MethodTypes;
      CurrentMethod->Decs = MethodDecs;
   }
}

int ClassWellformednessBubbler(std::shared_ptr<Class> CurrentClass, std::shared_ptr<Program> pgm)
{

   for (unsigned int i=0;i<CurrentClass->methods.size();++i)
   {
      for (unsigned int j=0;j<CurrentClass->methods.size();++j)
      {
         if ((CurrentClass->methods[i]->name == CurrentClass->methods[j]->name) && (i!=j))
         {
            std::cerr << CurrentClass->ln << " : ";
            std::cerr << "Type Error: multiple instances of method " << 
            CurrentClass->methods[i]->name << " defined in class " <<
            CurrentClass->id << "." << std::endl;
            exit(-1);

         }
      }
   }

   if (CurrentClass->id != "Obj")
   {
      std::shared_ptr<Class> SuperClass = CurrentClass->s_ptr;
      /* check field redefinition */
      for (unsigned int i=0;i<SuperClass->Fields.size();++i)
      {
         int Exists = 0;
         for (unsigned int j=0;j<CurrentClass->Fields.size();++j)
         {
            if (SuperClass->Fields[i] == CurrentClass->Fields[j])
            {
               Exists = 1;
               break;
            }
         }  
         if (Exists == 0)
         {
            std::cerr << CurrentClass->methods[i]->ptr->ln << " : ";
            std::cerr << "Type Error: Class field \'" 
            << SuperClass->Fields[i] << "\' extended from class \'" 
            << SuperClass->id << "\' not defined in class \'" 
            << CurrentClass->id << "\'." << std::endl;
            exit(-1);
         }
      }
      for (unsigned int i=0;i<CurrentClass->methods.size();++i)
      {
         for (unsigned int j=0;j<SuperClass->methods.size();++j)
         {
            if (CurrentClass->methods[i]->name == SuperClass->methods[j]->name)
            {
               std::string ReturnT = pgm->classes->GetCommonSuperFromString(CurrentClass->methods[i]->rt,
                                                      SuperClass->methods[j]->rt);
               if (ReturnT != SuperClass->methods[j]->rt)
               {
                  std::cerr << CurrentClass->methods[i]->ptr->ln << " : ";
                  std::cerr << "Return type " << CurrentClass->methods[i]->rt
                  << " not a subtype of " << SuperClass->methods[j]->rt
                  << " in redefinition of method " << CurrentClass->methods[i]->name
                  << " in class " << CurrentClass->id << std::endl;
                  exit(-1);
               }
               for (unsigned int k=0;k<CurrentClass->methods[i]->ParameterTypes.size();++k)
               {
                  std::string MustBeSuper = pgm->classes->GetCommonSuperFromString(CurrentClass->methods[i]->ParameterTypes[k],
                                                                                    SuperClass->methods[j]->ParameterTypes[k]);
                  if (MustBeSuper != CurrentClass->methods[i]->ParameterTypes[k])
                  {
                     std::cerr << CurrentClass->methods[i]->ptr->ln << " : ";
                     std::cerr << MustBeSuper << " Parameter " << (CurrentClass->methods[i]->ParameterTypes.size() - k)
                     << " of type " << CurrentClass->methods[i]->ParameterTypes[k]
                     << " not a supertype of " << SuperClass->methods[j]->ParameterTypes[k]
                     << " in redefinition of method " << CurrentClass->methods[i]->name
                     << " in class " << CurrentClass->id << std::endl;
                     exit(-1);
                  }
               }
            }
         }
      }
   }
   for (unsigned int i=0;i<CurrentClass->methods.size();++i)
   {
      std::shared_ptr<Met> CurrentMethod = CurrentClass->methods[i];
   }
}

std::string
ClassTypeBubbler(std::vector<std::string> *Symbols,
                      std::vector<std::string> *Types,
                      std::shared_ptr<Node> Node, 
                      std::shared_ptr<Program> pgm,
                      std::vector<std::string> *Declared)
{
   if (Node->type == "CLASS_BODY")
   {
      for(int i=0;i<Node->v.size();++i)
      {  
         if (Node->v[i]->type == "STATEMENTS")
         {
            ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }

      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STATEMENTS")
   {
      for(int i=0;i<Node->v.size();++i)
      {  
         if (Node->v[i]->type == "STATEMENTS")
         {
            ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }

      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT" || Node->v[i]->type == "STMT_ASSIGN"
             || Node->v[i]->type == "STMT_IF"|| Node->v[i]->type == "STMT_WHILE"
             || Node->v[i]->type == "STMT_T_RETURN")
         {
            ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STATEMENT")
   {
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      std::string rv = "UNUSED";
      return rv;
   }
   else if (Node->type == "STMT_WHILE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "While condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "STMT_ASSIGN")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "MEM_L_EXPR")
         {
            Symbol = Node->v[i]->t1;
            for (unsigned int j=0;j<Node->v[i]->v.size();++j)
            {
               if (Node->v[i]->v[j]->type == "R_EXPR")
               {
                  for (unsigned int k=0;k<Node->v[i]->v[j]->v.size();++k)
                  {
                     if (Node->v[i]->v[j]->v[k]->type == "L_EXPR")
                     {
                        if (Node->v[i]->v[j]->v[k]->t1 == "this")
                        {  

                           Symbol = Node->v[i]->v[j]->v[k]->t1 + "." + Node->v[i]->t1;
                        }  
                        else
                        {
                           for(unsigned int q=0;q<Symbols->size();++q)
                           {
                              if (Symbols->at(q) == Node->v[i]->v[j]->v[k]->t1)
                              {
                                 SymbolIndex = q;
                                 break;
                              }
                           }
                           std::string er = Types->at(SymbolIndex);
                           std::cerr << Node->v[i]->v[j]->ln << " : ";
                           std::cerr << "Type Error: symbol " << 
                           Node->v[i]->v[j]->v[k]->t1 << "." << Node->v[i]->t1
                           << " referenced outside of class def: " 
                           << er << std::endl;
                           exit(-1);
                        }
                     }
                  }
               }
            }
         }
      }
      for(int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      /* set check declared type */
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "TYPE_ID" && Node->v[i]->t2 == "DEFINED")
         {
            //std::string LeftType = Node->v[i]->t1;
            //DeclaredType = pgm->classes->GetCommonSuperFromString(LeftType, Types->at(SymbolIndex));
            Declared->at(SymbolIndex) = Node->v[i]->t1;
         }
      }
      if(Declared->at(SymbolIndex) != "Bottom" && Type != "Bottom")
      {
         std::string temp = pgm->classes->GetCommonSuperFromString(Type, Declared->at(SymbolIndex));
         if (temp != Declared->at(SymbolIndex))
         {
            std::cerr << Node->ln << " : ";
            std::cerr << "Type Error: Type " << temp << " to be assigned to " << Symbol << 
            " lower than declared type " << Declared->at(SymbolIndex) << std::endl;
            exit(-1);
         }
      }

      if (Type == "ERR" )
      {
         return "Complete";
      }
      if (Types->at(SymbolIndex) == "Bottom" )
      {
         Types->at(SymbolIndex) = Type;
      }
      else
      {  

         Type = pgm->classes->GetCommonSuperFromString(Type, Types->at(SymbolIndex));
         Types->at(SymbolIndex) = Type;
      }

      //std::cout << "Symbol: " << Symbol << " got type: " << Types->at(SymbolIndex) << std::endl;

      std::string rv = "Complete";
      return rv;
   }
   else if (Node->type == "STMT_IF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete";
   }
   else if (Node->type == "ELIFS")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELIFS")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "ELSE")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELIF")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR")
         {
            std::string et = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
            if (et != "Boolean")
            {
               std::cerr << Node->ln << " : ";
               std::cerr << "Type Error: If condition expects resolution to subtype of Boolean. Got "
               << et << " instead." << std::endl;
               exit(-1);

            }
         }
      }
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "ELSE")
   {
      std::string Type;
      std::string Symbol;
      std::string DeclaredType;
      int SymbolIndex;
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "STATEMENT_BLOCK")
         {  /* Node->v[i]->v[0] is a statement */
            Type = ClassTypeBubbler(Symbols, Types, Node->v[i]->v[0], pgm, Declared);
         }
      }
      return "Complete"; //unused
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "INT_LIT")
   {
      std::string rv = "Int";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "STRING_LIT")
   {
      std::string rv = "String";
      return rv;
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Left Expression")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* Extract object symbol         */
      for(unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "L_EXPR")
         {
            Symbol = Node->v[i]->t1;
         }
      }
      if(Symbol == "true" || Symbol == "false" )
      {
         return "Boolean";
      }
      for(int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "MEM_L_EXPR")
         {
            Symbol = Node->v[i]->t1;
            for (unsigned int j=0;j<Node->v[i]->v.size();++j)
            {
               if (Node->v[i]->v[j]->type == "R_EXPR")
               {
                  for (unsigned int k=0;k<Node->v[i]->v[j]->v.size();++k)
                  {
                     if (Node->v[i]->v[j]->v[k]->type == "L_EXPR")
                     {
                        if (Node->v[i]->v[j]->v[k]->t1 == "this")
                        {  

                           Symbol = Node->v[i]->v[j]->v[k]->t1 + "." + Node->v[i]->t1;
                        }  
                        else
                        {
                           for(unsigned int q=0;q<Symbols->size();++q)
                           {
                              if (Symbols->at(q) == Node->v[i]->v[j]->v[k]->t1)
                              {
                                 SymbolIndex = q;
                                 break;
                              }
                           }
                           std::string er = Types->at(SymbolIndex);
                           std::cerr << Node->v[i]->v[j]->ln << " : ";
                           std::cerr << "Type Error: symbol " << 
                           Node->v[i]->v[j]->v[k]->t1 << "." << Node->v[i]->t1
                           << " referenced outside of class def: " 
                           << er << std::endl;
                           exit(-1);
                        }
                     }
                  }
               }
            }
         }
      }
      for(unsigned int i=0;i<Symbols->size();++i)
      {
         if (Symbols->at(i) == Symbol)
         {
            SymbolIndex = i;
            break;
         }
      }
      return Types->at(SymbolIndex);
   }
   else if (Node->type == "R_EXPR" && Node->t2 == "METHOD_CALL")
   {
      std::string Type;
      std::string Symbol;
      std::string ReturnType;
      int SymbolIndex;
      /* type checking for method call */
      /* Extract        type           */
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if (Node->v[i]->type == "R_EXPR" || Node->v[i]->type == "CONS")
         {

            Type = ClassTypeBubbler(Symbols, Types, Node->v[i], pgm, Declared);
         }
      }
      /* Check if method is "of" type  */
      int ValidMethod=0;
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == Type)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == Node->t1)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;
               for (unsigned int j=0;j<Node->v.size();++j)
               {
                  if (Node->v[j]->type == "ACTUAL_ARG")
                  {
                     ArgTypes.push_back(ClassTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                  }
               }
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, Node->t1);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr <<  Node->t1 << " is not a method of class " << Type << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;
                  for (unsigned int j=0;j<Node->v.size();++j)
                  {
                     if (Node->v[j]->type == "ACTUAL_ARG")
                     {
                        ArgTypes.push_back(ClassTypeBubbler(Symbols, Types, Node->v[j], pgm, Declared));
                     }
                  }
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }

      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "+")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "ADD";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "-")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "SUB";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "*")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "MUL";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "/")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "DIV";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "==")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {  
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "<=")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == ">=")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      //std::string ReturnType = pgm->classes->GetCommonSuperFromString(LeftType, RightType);
      int ValidMethod=0;
      std::string MethodName = "LESS_THAN_EQUALS";
      std::shared_ptr<Met> ReturnMethod;

      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if (pgm->classes->v[i]->id == RightType)
         {
            std::shared_ptr<Class> CurrentClass = pgm->classes->v[i];
            for (unsigned int j=0;j<CurrentClass->methods.size();++j)
            {
               if (CurrentClass->methods[j]->name == MethodName)
               {
                  ReturnMethod = CurrentClass->methods[j];
                  ValidMethod=1;
               }
            }
            if (ValidMethod == 1)
            {
               std::vector<std::string> ArgTypes;  
               ArgTypes.push_back(LeftType);
               MethodParamValidator(ArgTypes, pgm, ReturnMethod);
               /* match method params */
               return ReturnMethod->rt;
            }
            else
            {
               ReturnMethod = MethodisValid(CurrentClass, MethodName);
               if (ReturnMethod->name == "BadMethod")
               {
                  std::cerr << Node->ln << " : ";
                  std::cerr << MethodName << " is not a method of " << RightType << "." << std::endl;
                  exit(-1);
               }
               else
               {
                  std::vector<std::string> ArgTypes;  
                  ArgTypes.push_back(LeftType);
                  MethodParamValidator(ArgTypes, pgm, ReturnMethod);
                  /* match method params */
                  return ReturnMethod->rt;
               }
            }
         }
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "Method " << MethodName << " is not of type " << RightType << "." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Parens")
   {
      return ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "And")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "AND expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Or")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      std::string RightType = ClassTypeBubbler(Symbols, Types, Node->v[1], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean" && RightType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "OR expression expected two Booleans got " << 
      LeftType << ", " << LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "R_EXPR" && Node->t1 == "Not")
   {
      std::string LeftType = ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
      /* check if left type is has a method mul */
      
      if (LeftType == "Boolean")
      {
         return LeftType;
      }
      std::cerr << Node->ln << " : ";
      std::cerr << "NOT expression expected a Boolean, got " << 
      LeftType << " instead." << std::endl;
      exit(-1);
      return "Bottom";
   }
   else if (Node->type == "CONS")
   {
      std::string ClassName = Node->t1;
      int Valid = 0;
      int ClassIndex;
      for (unsigned int i=0;i<pgm->classes->v.size();++i)
      {
         if(pgm->classes->v[i]->id == ClassName)
         {
            ClassIndex = i;
            Valid = 1;
            break;
         }
      }
      if (Valid == 1)
      {
         //std::cout << "Valid Constructor Name: " 
         //<< pgm->classes->v[ClassIndex]->id << std::endl;
      }
      else /* This block should never be reached */
      {
         std::cerr << Node->ln << " : ";
         std::cerr << "Type Error: Invalid constructor name: " 
         << ClassName << std::endl;
         exit(-1);
      }
      Valid = 0;
      if (Node->v.size() != (pgm->classes->v[ClassIndex]->v.size())-1) /* -1 because every Class.v has a class body */
      {
         std::cerr << Node->ln << " : ";
         std::cerr << "Type Error: Class Constructor \'" << ClassName 
         << "\' expected " << (pgm->classes->v[ClassIndex]->v.size())-1 
         << " arguments -- Got "<< Node->v.size() << " instead." << std::endl;
         ++NumberOfCaughtErrors;
      }
      /* //check that params match type 
      for (unsigned int i=0;i<Node->v.size();++i)
      {
         if( pgm->Classes == StatementsTypeBubbler(Node->v[i], pgm));
      }
      */
      return ClassName;
   }
   else if (Node->type == "L_EXPR")
   {
   }
   else if (Node->type == "ACTUAL_ARG")
   {
      return ClassTypeBubbler(Symbols, Types, Node->v[0], pgm, Declared);
   }
   else
   {
      std::string rv = "ERR";
      return rv;
   }
}