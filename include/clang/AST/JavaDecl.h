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

class JavaImportDecl : public Decl {

  friend class ASTReader;
  friend class ASTDeclReader;
  friend class ASTContext;

  JavaImportDecl(DeclContext *DC, 
                 SourceLocation StartLoc,
                 ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath);
public:
  static JavaImportDecl *Create(ASTContext &C, DeclContext *DC, 
                                SourceLocation StartLoc,
                                ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath);
  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

class JavaContainerDecl : public NamedDecl, public DeclContext {
public:
  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) {
    return K >= firstObjCContainer &&
           K <= lastObjCContainer;
  }

  static DeclContext *castToDeclContext(const JavaContainerDecl *D) {
    return static_cast<DeclContext *>(const_cast<JavaContainerDecl*>(D));
  }
  static JavaContainerDecl *castFromDeclContext(const DeclContext *DC) {
    return static_cast<JavaContainerDecl *>(const_cast<DeclContext*>(DC));
  }
};

class JavaClassDecl : public JavaContainerDecl {
public:
  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

class JavaInterfaceDecl : public JavaContainerDecl {
public:
  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

}

#endif
