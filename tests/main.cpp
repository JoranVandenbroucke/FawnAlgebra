//
// Created by Joran on 29/01/2024.
//
#include <iostream>

#include <easing.hpp>
#include <hashing.hpp>


int main() {
    std::cout << "FawnAlgebra::lerp(0.f, 1.f, 0.5f) == " << FawnAlgebra::lerp(0.f, 1.f, 0.5f) << std::endl;
    std::cout << "FawnAlgebra::(0.f, 1.f, 0.5f) == " << FawnAlgebra::hash_djb2("Hallo, world!") << std::endl;
    return 0;
}
