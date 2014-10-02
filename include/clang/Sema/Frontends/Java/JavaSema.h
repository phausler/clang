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
#include "clang/AST/Frontends/Java/JavaType.h"

namespace clang {

class JavaASTContext;

// Do these belong here? if not where should they go?
typedef SmallVector<std::pair<IdentifierInfo *, SourceLocation>, 2> JavaClassPath;
typedef SmallVector<JavaClassPath, 2> JavaClassPathList;

class JavaSema : public Sema {
public:
  JavaSema(Preprocessor &pp, ASTContext &ctxt, ASTConsumer &consumer,
           TranslationUnitKind TUKind = TU_Complete,
           CodeCompleteConsumer *CompletionConsumer = nullptr) :
    Sema(pp, ctxt, consumer, TUKind, CompletionConsumer) {

  }

  JavaASTContext *JavaContext() const {
    return (JavaASTContext *)&Context;
  }

  virtual Parser* createParser(bool SkipFunctionBodies);

  void CodeCompletePacakge(SourceLocation PacakgeLoc, JavaClassPath Path);
  void CodeCompleteImport(SourceLocation ImportLoc, JavaClassPath Path);
  void CodeCompleteClass(SourceLocation ClassLoc, JavaClassPath Path);
  void CodeCompleteInterface(SourceLocation ImplementsLoc, JavaClassPath Path);

  DeclResult ActOnJavaPackage(SourceLocation PacakgeLoc, JavaClassPath Path);
  DeclResult ActOnJavaImport(SourceLocation PacakgeLoc, JavaClassPath Path);
  Decl *ActOnBeginJavaClass(SourceLocation Loc, JavaQualifiers modifiers, 
                            const IdentifierInfo *ClassPath, 
                            SourceLocation ExtendsLoc, const IdentifierInfo *Extends, 
                            SourceLocation ImplementsLoc, ArrayRef<const IdentifierInfo *> ImplementsList);
  Decl *ActOnBeginJavaInterface(SourceLocation Loc, JavaQualifiers modifiers, 
                                const IdentifierInfo *ClassPath, 
                                SourceLocation ExtendsLoc, ArrayRef<const IdentifierInfo *> ExtendsList);

  Decl *ActOnJavaStaticInitializer(SourceLocation Loc, StmtResult Body);
  Decl *ActOnJavaParameter(SourceLocation Loc, ParsedType Ty, IdentifierInfo *II);
  Decl *ActOnJavaConstructor(SourceLocation Loc, JavaQualifiers modifiers, SmallVector<Decl *, 8>Args, StmtResult Body);

  Decl *ActOnJavaAbstractMethod(SourceLocation Loc, JavaQualifiers modifiers, ParsedType ReturnTy, IdentifierInfo *II, SmallVector<Decl *, 8>Args);
  Decl *ActOnJavaMethod(SourceLocation Loc, JavaQualifiers modifiers, ParsedType ReturnTy, IdentifierInfo *II, SmallVector<Decl *, 8>Args, StmtResult Body);
  Decl *ActOnJavaVariable(SourceLocation Loc, JavaQualifiers modifiers, ParsedType Ty, IdentifierInfo *II, SourceLocation InitializerLoc, Expr *Initializer);
};
}

#endif
