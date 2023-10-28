//
// Power.hpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Power_hpp
#define Power_hpp

#include <stdio.h>
#include "Expr.hpp"

class Power : public Expr {
public:
    Power(Expr* base, Expr* expo);

    void setBase(Expr* base);
    void setExpo(Expr* expo);

    Expr* getBase();
    Expr* getExpo();

    Expr* simplify() override;

    Expr* copy() override;

    long generateHash() override;
    bool equalStruct(Expr* other) override;

    std::string getLatex() override;

    void print() override;
};


#endif /* Power_hpp */
