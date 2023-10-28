//
// main.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include <vector>
#include <string>

#include "Expr.hpp"
#include "Num.hpp"
#include "Var.hpp"
#include "Power.hpp"
#include "Sum.hpp"
#include "Prod.hpp"

void evaluate(Expr* equation);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

//  Check Sums
    evaluate(new Sum(new Num(2), new Num(5))); // Num + Num
    evaluate(new Sum(new Num(2), new Var("x"))); // Number + Var
    evaluate(new Sum(new Prod(new Num(2), new Var("x")), new Prod(new Num(4), new Var("x")))); // Num*Var + Num*Var
    evaluate(new Sum(new Prod(new Num(2), new Var("y")), new Prod(new Num(4), new Var("x")))); // Num*Var + Num*Var

//  Check Products
    evaluate(new Prod(new Num(2), new Num(7)));
    evaluate(new Prod(new Num(2), new Var("x")));
    evaluate(new Prod(new Var("x"), new Power(new Var("x"), new Num(3))));
    evaluate(new Prod(new Power(new Var("x"), new Num(3)), new Power(new Var("x"), new Num(3))));
    evaluate(new Prod(new Power(new Var("x"), new Prod(new Num(3), new Var("x"))), new Power(new Var("x"), new Prod(new Num(3), new Var("x")))));

    std::cout << std::endl;

//  Children Simplification
    evaluate(new Sum(new Sum(new Num(3), new Num (7)), new Sum(new Num(3), new Num (7))));
    evaluate(new Prod(new Prod(new Num(6), new Num(3)), new Sum(new Num(3), new Num (7))));

    return 0;
}

void evaluate(Expr* equation) {
    Expr* result = equation->simplify();

    std::cout << "$" << equation->getLatex() << "=" << result->getLatex() << "$" << std::endl;

}
