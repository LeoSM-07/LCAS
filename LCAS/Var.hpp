//
// Var.hpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Var_hpp
#define Var_hpp

#include "Expr.hpp"
#include <string>

class Var : public Expr {
public:
    Var(std::string identifier);

    Expr* simplify() override;

    Expr* copy() override;

    bool equalStruct(Expr* other) override;

    std::string getLatex() override;

    void print() override;

private:
    std::string name;
};

#endif /* Var_hpp */
