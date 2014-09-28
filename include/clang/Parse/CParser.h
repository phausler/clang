//===--- CParser.h - C Language Parser -----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the CParser interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_PARSE_CPARSER_H
#define LLVM_CLANG_PARSE_CPARSER_H

#include "clang/Parse/Parser.h"

namespace clang {
class CParser : public Parser {
public:
  CParser(Preprocessor &PP, Sema &Actions, bool SkipFunctionBodies) :
    Parser(PP, Actions, SkipFunctionBodies) {  	
  }

  virtual bool ParseTopLevelDecl(DeclGroupPtrTy &Result);
};
}

#endif
