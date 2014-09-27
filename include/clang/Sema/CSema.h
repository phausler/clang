//===----- CSema.h - C Semantic Analysis & AST Building --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the C Sema class, which performs semantic analysis and
// builds ASTs.ma.h
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_SEMA_CSEMA_H
#define LLVM_CLANG_SEMA_CSEMA_H

#include "clang/Sema/Sema.h"

namespace clang {
class CSema : public Sema {
public:
  CSema(Preprocessor &pp, ASTContext &ctxt, ASTConsumer &consumer,
           TranslationUnitKind TUKind = TU_Complete,
           CodeCompleteConsumer *CompletionConsumer = nullptr) :
    Sema(pp, ctxt, consumer, TUKind, CompletionConsumer) {

  }

  virtual Parser* createParser(bool SkipFunctionBodies);
};
}

#endif
