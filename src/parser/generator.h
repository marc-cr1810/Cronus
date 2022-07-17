#pragma once

#include <port.h>

#include <parser/ast.h>
#include <parser/parser.h>

#include <objects/object.h>

mod_type CrGen_ASTFromFileObject(std::ifstream* fp, CrObject* filename, int mode, const char* ps1, const char* ps2, int* error_code);