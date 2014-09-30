//===-- JavaASTContext.h - Context to hold long-lived AST nodes -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the clang::JavaASTContext interface.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_JAVAASTCONTEXT_H
#define LLVM_CLANG_AST_JAVAASTCONTEXT_H

#include "clang/AST/ASTContext.h"

namespace clang {
class JavaASTContext : public ASTContext {
public:
  JavaASTContext(LangOptions &LOpts, SourceManager &SM, IdentifierTable &idents,
                 SelectorTable &sels, Builtin::Context &builtins) :
    ASTContext(LOpts, SM, idents, sels, builtins) {

  }

  Sema* createSema(Preprocessor &pp, ASTConsumer &consumer,
       TranslationUnitKind TUKind = TU_Complete,
       CodeCompleteConsumer *CompletionConsumer = nullptr);

  IdentifierInfo &FullyQualifiedIdentifier(ArrayRef<IdentifierInfo *> Path) {
    std::string Name = "";
    for (ArrayRef<IdentifierInfo *>::iterator it = Path.begin(); it != Path.end(); ++it) {
      Name += (*it)->getName();
      if (it + 1 != Path.end()) {
        Name += ".";
      }
    }
    return Idents.get(Name);
  }
};
}

#endif