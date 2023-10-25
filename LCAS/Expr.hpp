//
// Expr.h
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Expr_h
#define Expr_h


#include <vector>
#include <string>

class Expr {
public:
    std::vector<Expr*> subExpr;

    virtual Expr* simplify() = 0;
    virtual Expr* copy() = 0;
    virtual bool equalStruct(Expr* other) = 0;
    virtual std::string getLatex() = 0;
    virtual void print() = 0;

};


#endif /* Expr_h */
