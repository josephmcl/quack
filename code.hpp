#include <vector>
#include <memory>
#include <fstream>
#include "ast.hpp"

int GenerateCode(std::shared_ptr<Program> pgm);
int r_stmt(std::shared_ptr<Node> Node, std::ofstream f);
int GenerateMain(std::shared_ptr<Program> pgm, std::ofstream f);
