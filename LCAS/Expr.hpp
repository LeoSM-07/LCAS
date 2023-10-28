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
private:
    std::vector<Expr*> subExpr;

public:
    bool simple = false;

    virtual Expr* simplify() = 0;
    virtual Expr* copy() = 0;
    virtual bool equalStruct(Expr* other) = 0;
    virtual std::string getLatex() = 0;
    virtual void print() = 0;

    void simplifyChildren();

    void push_back(Expr* e);

    void erase(int index);

    void set(int index, Expr* e);

    Expr* at(int index);

    Expr* first();

    size_t size();

    void clear();

    void sort();
};


#endif /* Expr_h */
