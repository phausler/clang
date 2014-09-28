//===--- JavaDecl.h - Classes for representing declarations -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the JavaDecl subclasses.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_JAVADECL_H
#define LLVM_CLANG_AST_JAVADECL_H

#include "clang/AST/Decl.h"

namespace clang {

class JavaPackageDecl : public Decl {

  friend class ASTReader;
  friend class ASTDeclReader;
  friend class ASTContext;

  JavaPackageDecl(DeclContext *DC, 
                  SourceLocation StartLoc,
                  ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath);
public:
  static JavaPackageDecl *Create(ASTContext &C, DeclContext *DC, 
                                 SourceLocation StartLoc,
                                 ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath);
  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

}

#endif
