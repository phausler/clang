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
  typedef SmallVector<std::pair<IdentifierInfo *, SourceLocation>, 2> JavaPathIdentifier;

  JavaSema *JavaActions() {
  	return (JavaSema *)&Actions;
  }

  bool isTokenModifier() const {
    switch (Tok.getKind()) {
      case tok::java_public:
      case tok::java_private:
      case tok::java_protected:
      case tok::java_static:
      case tok::java_final:
      case tok::java_native:
      case tok::java_synchronized:
      case tok::java_abstract:
      case tok::java_threadsafe:
      case tok::java_transient:
        return true;
      default:
        return false;
    }
  }
  bool ParseJavaIdentifier(JavaPathIdentifier &Ident,
                           bool AcceptsWildcard, SourceLocation Loc,
                           void (*CodeCompletion)(JavaParser *Parser, SourceLocation Loc, JavaPathIdentifier Path));
public:
  JavaParser(Preprocessor &PP, Sema &Actions, bool SkipFunctionBodies) :
    Parser(PP, Actions, SkipFunctionBodies) {  	
  }

  virtual bool ParseTopLevelDecl(DeclGroupPtrTy &Result);
  DeclGroupPtrTy ParseExternalDeclaration(ParsedAttributesWithRange &attrs,
                                          ParsingDeclSpec *DS = nullptr);

  DeclGroupPtrTy ParseJavaPackageDefinition();
  DeclGroupPtrTy ParseJavaImport();
  Decl *ParseJavaTypeDeclaration();
  Decl *ParseJavaClass(SourceLocation Loc /*,modifiers*/);
  Decl *ParseJavaInterface(SourceLocation Loc /*,modifiers*/);
};
}

#endif
