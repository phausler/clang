//===--- JavaParser.h - Hava Language Parser --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the JavaParser interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_PARSE_JAVAPARSER_H
#define LLVM_CLANG_PARSE_JAVAPARSER_H

#include "clang/Parse/Parser.h"
#include "clang/Sema/JavaSema.h"

namespace clang {
class JavaParser : public Parser {
private:
  JavaSema *JavaActions() {
  	return (JavaSema *)&Actions;
  }
public:
  JavaParser(Preprocessor &PP, Sema &Actions, bool SkipFunctionBodies) :
    Parser(PP, Actions, SkipFunctionBodies) {  	
  }

  virtual bool ParseTopLevelDecl(DeclGroupPtrTy &Result);
  DeclGroupPtrTy ParseExternalDeclaration(ParsedAttributesWithRange &attrs,
                                          ParsingDeclSpec *DS = nullptr);

  DeclGroupPtrTy ParseJavaPackageDefinition();
  Decl *ParseJavaImport();
};
}

#endif
