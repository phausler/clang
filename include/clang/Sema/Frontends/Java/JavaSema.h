//===--- JavaSema.h - Java Semantic Analysis & AST Building -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the Java Sema class, which performs semantic analysis and
// builds ASTs.ma.h
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_SEMA_JAVASEMA_H
#define LLVM_CLANG_SEMA_JAVASEMA_H

#include "clang/Sema/Sema.h"

namespace clang {
class JavaSema : public Sema {
	typedef ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> JavaClassPath;
public:
  JavaSema(Preprocessor &pp, ASTContext &ctxt, ASTConsumer &consumer,
           TranslationUnitKind TUKind = TU_Complete,
           CodeCompleteConsumer *CompletionConsumer = nullptr) :
    Sema(pp, ctxt, consumer, TUKind, CompletionConsumer) {

  }

  virtual Parser* createParser(bool SkipFunctionBodies);

  void CodeCompletePacakge(SourceLocation PacakgeLoc, JavaClassPath Path);
  void CodeCompleteImport(SourceLocation ImportLoc, JavaClassPath Path);
  void CodeCompleteClass(SourceLocation ClassLoc, JavaClassPath Path);

  DeclResult ActOnJavaPackage(SourceLocation PacakgeLoc, JavaClassPath Path);
  DeclResult ActOnJavaImport(SourceLocation PacakgeLoc, JavaClassPath Path);
};
}

#endif
