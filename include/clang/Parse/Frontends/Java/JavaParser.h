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
#include "clang/AST/Frontends/Java/JavaType.h"
#include "clang/Sema/Frontends/Java/JavaSema.h"

namespace clang {
class JavaParser : public Parser {
private:
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

  bool isTokenIntrinsicTypeSpecifier() const {
    switch (Tok.getKind()) {
      case tok::java_boolean:
      case tok::java_byte:
      case tok::java_char:
      case tok::java_short:
      case tok::java_int:
      case tok::java_float:
      case tok::java_long:
      case tok::java_double:
      case tok::java_void:
        return true;
      default:
        return false;
    }
  }

  const IdentifierInfo *ParseJavaIdentifierType(ArrayRef<IdentifierInfo *> Path);
  const IdentifierInfo *ParseJavaIdentifierType();

  bool ParseJavaIdentifier(JavaClassPath &Ident,
                           bool AcceptsWildcard, SourceLocation Loc,
                           void (*CodeCompletion)(JavaParser *Parser, SourceLocation Loc, JavaClassPath Path));
public:
  JavaParser(Preprocessor &PP, Sema &Actions, bool SkipFunctionBodies) :
    Parser(PP, Actions, SkipFunctionBodies) {  	
  }

  virtual bool ParseTopLevelDecl(DeclGroupPtrTy &Result);
  DeclGroupPtrTy ParseExternalDeclaration(ParsedAttributesWithRange &attrs,
                                          ParsingDeclSpec *DS = nullptr);

  DeclGroupPtrTy ParseJavaPackageDefinition();
  DeclGroupPtrTy ParseJavaImport();
  JavaQualifiers ParseModifiers();
  ParsedType ParseJavaType(bool isReturnType);
  Decl *ParseJavaTypeDeclaration();
  Decl *ParseJavaClass(SourceLocation Loc, JavaQualifiers modifiers);
  Decl *ParseJavaInterface(SourceLocation Loc, JavaQualifiers modifiers);
  void ParseJavaContainer(SourceLocation Loc, JavaQualifiers modifiers, Decl *ContainerType, bool CanContainImplementations);
  Decl *ParseJavaStaticInitializer(Decl *ContainerType);

  StmtResult ParseJavaStatement(SourceLocation *TrailingElseLoc = nullptr);
  StmtResult
  ParseJavaStatementOrDeclaration(StmtVector &Stmts, bool OnlyStatement,
                                  SourceLocation *TrailingElseLoc = nullptr);

  StmtResult ParseJavaIfStatement(SourceLocation *TrailingElseLoc);
  StmtResult ParseJavaSwitchStatement(SourceLocation *TrailingElseLoc);
  StmtResult ParseJavaWhileStatement(SourceLocation *TrailingElseLoc);
  StmtResult ParseJavaForStatement(SourceLocation *TrailingElseLoc);
  StmtResult ParseJavaExprStatement();
  StmtResult ParseJavaCaseStatement();
  StmtResult ParseJavaDefaultStatement();
  StmtResult ParseCompoundStatement();
  StmtResult ParseJavaDoStatement();
  StmtResult ParseJavaGotoStatement();
  StmtResult ParseJavaContinueStatement();
  StmtResult ParseJavaBreakStatement();
  StmtResult ParseJavaReturnStatement();

  Decl *ParseJavaParameter();
  void ParseJavaParameterList(SmallVector<Decl *, 8> Params);
  Decl *ParseJavaConstructor(SourceLocation Loc, JavaQualifiers modifiers);
  Decl *ParseJavaMethod(SourceLocation Loc, JavaQualifiers modifiers);
  Expr *ParseJavaExpression(SourceLocation Loc);
  Decl *ParseJavaVariable(SourceLocation Loc, JavaQualifiers modifiers, ParsedType Ty);
  SmallVector<Decl *, 2> ParseJavaVariable(SourceLocation Loc, JavaQualifiers modifiers);
};

}

#endif
