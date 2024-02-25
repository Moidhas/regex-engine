#include <iostream>
#include <stdexcept>
#include "reg.h"


int main(int argc, char *argv[]) {
    try {
        Regex re{argv[1]};
        std::string b = re.match(argv[2]) ? "match" : "not matched";
        std::cout << b << std::endl;
    } catch (std::out_of_range r) {
        std::cerr << r.what() << std::endl; 
    }
}
