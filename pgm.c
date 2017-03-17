#include <stdio.h>
#include <stdlib.h>
#include "builtin/Builtins.h"
#include "gc.h"
void quackmain(); 
int main(int argc, char** argv) {
  quackmain();
  printf("\n--- Terminated successfully ---\n");
  exit(0);
}
struct class_Square_struct;
typedef struct class_Square_struct* class_Square;
struct class_Square_struct the_class_Square_struct;
struct obj_Square_struct;
typedef struct obj_Square_struct* obj_Square;
struct class_Rect_struct;
typedef struct class_Rect_struct* class_Rect;
struct class_Rect_struct the_class_Rect_struct;
struct obj_Rect_struct;
typedef struct obj_Rect_struct* obj_Rect;
struct class_Pt_struct;
typedef struct class_Pt_struct* class_Pt;
struct class_Pt_struct the_class_Pt_struct;
struct obj_Pt_struct;
typedef struct obj_Pt_struct* obj_Pt;
struct obj_Square_struct {
  class_Square clazz;
obj_Pt ll_;
obj_Pt ur_;
};
struct obj_Rect_struct {
  class_Rect clazz;
obj_Pt ur_;
obj_Pt ll_;
};
struct obj_Pt_struct {
  class_Pt clazz;
obj_Int y_;
obj_Int x_;
};
struct class_Square_struct {
obj_Square (*constructor) (obj_Int, obj_Pt);
obj_Nothing (*PRINT) (obj_Rect);
obj_String (*STRING) (obj_Rect);
obj_Rect (*translate) (obj_Rect, obj_Pt);
obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);
};
extern class_Square the_class_Square;
struct class_Rect_struct {
obj_Rect (*constructor) (obj_Pt, obj_Pt);
obj_Nothing (*PRINT) (obj_Rect);
obj_String (*STRING) (obj_Rect);
obj_Rect (*translate) (obj_Rect, obj_Pt);
obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);
};
extern class_Rect the_class_Rect;
struct class_Pt_struct {
obj_Pt (*constructor) (obj_Int, obj_Int);
obj_Int (*_y) (obj_Pt);
obj_Int (*_x) (obj_Pt);
obj_Pt (*ADD) (obj_Pt, obj_Pt);
obj_Nothing (*PRINT) (obj_Pt);
obj_String (*STRING) (obj_Pt);
obj_Boolean (*EQUALS) (obj_Obj, obj_Obj);
};
extern class_Pt the_class_Pt;
obj_Square new_Square(obj_Int side_, obj_Pt ll_) {
obj_Square this_ = (obj_Square) GC_MALLOC(sizeof(struct obj_Square_struct));
this_->clazz = the_class_Square;
void *true_ = lit_true;
void *false_ = lit_false;
this_->ll_=ll_;
this_->ur_=the_class_Pt->constructor(((obj_Int)((obj_Int)((obj_Pt)this_->ll_)->clazz->_x(((obj_Pt)this_->ll_)))->clazz->ADD(((obj_Int)((obj_Pt)this_->ll_)->clazz->_x(((obj_Pt)this_->ll_))), ((obj_Int)side_))), ((obj_Int)((obj_Int)((obj_Pt)this_->ll_)->clazz->_y(((obj_Pt)this_->ll_)))->clazz->ADD(((obj_Int)((obj_Pt)this_->ll_)->clazz->_y(((obj_Pt)this_->ll_))), ((obj_Int)side_))));
return this_;
}
obj_Rect new_Rect(obj_Pt ur_, obj_Pt ll_) {
obj_Rect this_ = (obj_Rect) GC_MALLOC(sizeof(struct obj_Rect_struct));
this_->clazz = the_class_Rect;
void *delta_;
void *true_ = lit_true;
void *false_ = lit_false;
this_->ll_=ll_;
this_->ur_=ur_;
return this_;
}
obj_Nothing Rect_method_PRINT(obj_Rect this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
((obj_String)str_literal("("))->clazz->PRINT(((obj_String)str_literal("(")));
((obj_Pt)this_->ll_)->clazz->PRINT(((obj_Pt)this_->ll_));
((obj_String)str_literal(", "))->clazz->PRINT(((obj_String)str_literal(", ")));
((obj_Pt)this_->ur_)->clazz->PRINT(((obj_Pt)this_->ur_));
((obj_String)str_literal(")"))->clazz->PRINT(((obj_String)str_literal(")")));
}
obj_String Rect_method_STRING(obj_Rect this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return str_literal("A Rectangle");
}
obj_Rect Rect_method_translate(obj_Rect this_, obj_Pt delta_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return the_class_Rect->constructor(((obj_Pt)((obj_Pt)this_->ll_)->clazz->ADD(((obj_Pt)this_->ll_), ((obj_Pt)delta_))), ((obj_Pt)((obj_Pt)this_->ur_)->clazz->ADD(((obj_Pt)this_->ur_), ((obj_Pt)delta_))));
}
obj_Pt new_Pt(obj_Int y_, obj_Int x_) {
obj_Pt this_ = (obj_Pt) GC_MALLOC(sizeof(struct obj_Pt_struct));
this_->clazz = the_class_Pt;
void *other_;
void *true_ = lit_true;
void *false_ = lit_false;
this_->x_=x_;
this_->y_=y_;
return this_;
}
obj_Int Pt_method__y(obj_Pt this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return this_->y_;
}
obj_Int Pt_method__x(obj_Pt this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return this_->x_;
}
obj_Pt Pt_method_ADD(obj_Pt this_, obj_Pt other_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return the_class_Pt->constructor(((obj_Int)((obj_Int)this_->x_)->clazz->ADD(((obj_Int)this_->x_), ((obj_Int)((obj_Pt)other_)->clazz->_x(((obj_Pt)other_))))), ((obj_Int)((obj_Int)this_->y_)->clazz->ADD(((obj_Int)this_->y_), ((obj_Int)((obj_Pt)other_)->clazz->_y(((obj_Pt)other_))))));
}
obj_Nothing Pt_method_PRINT(obj_Pt this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
((obj_String)str_literal("("))->clazz->PRINT(((obj_String)str_literal("(")));
((obj_Int)this_->x_)->clazz->PRINT(((obj_Obj)this_->x_));
((obj_String)str_literal(", "))->clazz->PRINT(((obj_String)str_literal(", ")));
((obj_Int)this_->y_)->clazz->PRINT(((obj_Obj)this_->y_));
((obj_String)str_literal(")"))->clazz->PRINT(((obj_String)str_literal(")")));
}
obj_String Pt_method_STRING(obj_Pt this_) {
void *false_ = lit_false;
void *true_ = lit_false;
void *return_;
return str_literal("A Point");
}
struct class_Square_struct the_class_Square_struct ={
 new_Square, Rect_method_PRINT
, Rect_method_STRING
, Rect_method_translate
, Obj_method_EQUALS
};
class_Square the_class_Square= &the_class_Square_struct;
struct class_Rect_struct the_class_Rect_struct ={
 new_Rect, Rect_method_PRINT
, Rect_method_STRING
, Rect_method_translate
, Obj_method_EQUALS
};
class_Rect the_class_Rect= &the_class_Rect_struct;
struct class_Pt_struct the_class_Pt_struct ={
 new_Pt, Pt_method__y
, Pt_method__x
, Pt_method_ADD
, Pt_method_PRINT
, Pt_method_STRING
, Obj_method_EQUALS
};
class_Pt the_class_Pt= &the_class_Pt_struct;
void quackmain() {
/*
STMT_ASSIGN
STMT_ASSIGN
STATEMENT
STATEMENT
STATEMENT
STATEMENT
STATEMENT
STATEMENT
STMT_ASSIGN
STATEMENT
STATEMENT
STMT_ASSIGN
STATEMENT
STATEMENT
STMT_ASSIGN
STATEMENT
STATEMENT
*/
void *true_=lit_true;
void *false_=lit_false;
void *a_;
void *b_;
void *c_;
void *d_;
a_=the_class_Pt->constructor(((obj_Int)int_literal(0)), ((obj_Int)int_literal(1)));
b_=the_class_Pt->constructor(((obj_Int)int_literal(1)), ((obj_Int)int_literal(0)));
((obj_Pt)a_)->clazz->PRINT(((obj_Pt)a_));
((obj_String)str_literal(" + "))->clazz->PRINT(((obj_String)str_literal(" + ")));
((obj_Pt)b_)->clazz->PRINT(((obj_Pt)b_));
((obj_String)str_literal(" = "))->clazz->PRINT(((obj_String)str_literal(" = ")));
((obj_Pt)(((obj_Pt)a_)->clazz->ADD(((obj_Pt)a_), ((obj_Pt)b_))))->clazz->PRINT(((obj_Pt)(((obj_Pt)a_)->clazz->ADD(((obj_Pt)a_), ((obj_Pt)b_)))));
((obj_String)str_literal("\n"))->clazz->PRINT(((obj_String)str_literal("\n")));
c_=the_class_Rect->constructor(((obj_Pt)b_), ((obj_Pt)a_));
((obj_Rect)c_)->clazz->PRINT(((obj_Rect)c_));
((obj_String)str_literal("\n"))->clazz->PRINT(((obj_String)str_literal("\n")));
d_=the_class_Square->constructor(((obj_Int)int_literal(3)), ((obj_Pt)the_class_Pt->constructor(((obj_Int)int_literal(1)), ((obj_Int)int_literal(1)))));
((obj_Rect)d_)->clazz->PRINT(((obj_Rect)d_));
((obj_String)str_literal("\n"))->clazz->PRINT(((obj_String)str_literal("\n")));
d_=((obj_Rect)d_)->clazz->translate(((obj_Rect)d_), ((obj_Pt)the_class_Pt->constructor(((obj_Int)int_literal(2)), ((obj_Int)int_literal(2)))));
((obj_Rect)d_)->clazz->PRINT(((obj_Rect)d_));
((obj_String)str_literal("\n"))->clazz->PRINT(((obj_String)str_literal("\n")));
}