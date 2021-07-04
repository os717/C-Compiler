#include <unordered_map>
#include <fstream>
#include "../include/ast_leaf.hpp"
#include "../include/ast_arith.hpp"
#include "../include/ast_dec.hpp"
#include "../include/ast_logic.hpp"
#include "../include/ast_prim.hpp"
#include "../include/ast_state.hpp"
#include "../include/cont.hpp"

std::vector<std::string> variables_global;

void print_python_main(std::ostream &stream){
  stream << "\n\nif __name__ == \"__main__\":\n";
  stream << "\timport sys\n";
  stream << "\tret=main()\n";
  stream << "\tsys.exit(ret)";
}

int main(int argc, char *argv[]){
  int SCP = 0;
  if(std::string(argv[1]) == "--translate"){
    const Pointer_LEAF ast = DO_PARSE(argv[2]);
    std::ofstream OUT(argv[4]);
    ast->PYTHON_t(OUT,SCP);
    OUT << "\n\nif __name__ == \"__main__\":\n";
    OUT << "\timport sys\n";
    OUT << "\tret=main()\n";
    OUT << "\tsys.exit(ret)";
  }else if(std::string(argv[1]) == "-S"){
    const Pointer_LEAF ast = DO_PARSE(argv[2]);
    std::ofstream OUT(argv[4]);
    std::unordered_map<int, std::unordered_map<std::string, int>> GELS;
    CONT PROG(GELS);
    ast->C90_COMP(OUT, PROG);
  }
  return 0;
}
