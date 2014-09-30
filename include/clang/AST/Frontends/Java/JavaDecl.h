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
#include "clang/Sema/Frontends/Java/JavaSema.h"
#include "clang/AST/Frontends/Java/JavaType.h"

namespace clang {

class JavaPackageDecl : public Decl, public DeclContext {

  friend class ASTReader;
  friend class ASTDeclReader;
  friend class ASTContext;

  JavaPackageDecl(DeclContext *DC, 
                  SourceLocation StartLoc,
                  JavaClassPath ClassPath);
public:
  static JavaPackageDecl *Create(ASTContext &C, DeclContext *DC, 
                                 SourceLocation StartLoc,
                                 JavaClassPath ClassPath);

  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

class JavaImportDecl : public Decl {

  friend class ASTReader;
  friend class ASTDeclReader;
  friend class ASTContext;

  JavaImportDecl(DeclContext *DC, 
                 SourceLocation StartLoc,
                 JavaClassPath ClassPath);
public:
  static JavaImportDecl *Create(ASTContext &C, DeclContext *DC, 
                                SourceLocation StartLoc,
                                JavaClassPath ClassPath);

  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

class JavaContainerDecl : public NamedDecl, public DeclContext {
protected:
  JavaContainerDecl(Decl::Kind DK, DeclContext *DC, SourceLocation L, const IdentifierInfo *ClassPath);
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

  JavaClassDecl(DeclContext *DC, 
                SourceLocation StartLoc,
                const IdentifierInfo *ClassPath);

public:
  static JavaClassDecl *Create(ASTContext &C, DeclContext *DC, 
                               SourceLocation Loc, JavaQualifiers modifiers, 
                               const IdentifierInfo *ClassPath, 
                               SourceLocation ExtendsLoc, const IdentifierInfo *Extends, 
                               SourceLocation ImplementsLoc, ArrayRef<const IdentifierInfo *> ImplementsList);

  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

class JavaInterfaceDecl : public JavaContainerDecl {

  JavaInterfaceDecl(DeclContext *DC, 
                    SourceLocation StartLoc,
                    const IdentifierInfo *ClassPath);

public:
  static JavaInterfaceDecl *Create(ASTContext &C, DeclContext *DC, 
                                   SourceLocation Loc, JavaQualifiers modifiers, 
                                   const IdentifierInfo *ClassPath, 
                                   SourceLocation ExtendsLoc, ArrayRef<const IdentifierInfo *> ExtendsList);

  static bool classof(const Decl *D) { return classofKind(D->getKind()); }
  static bool classofKind(Kind K) { return K == Import; }
};

}

#endif
