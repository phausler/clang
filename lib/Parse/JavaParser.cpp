#include "clang/Parse/Frontends/Java/JavaParser.h"
#include "RAIIObjectsForParser.h"
#include "clang/Parse/ParseDiagnostic.h"

// http://cui.unige.ch/isi/bnf/JAVA/BNFindex.html

namespace clang {

bool JavaParser::ParseTopLevelDecl(DeclGroupPtrTy &Result) {
  Result = DeclGroupPtrTy();
  switch (Tok.getKind()) {
    case tok::eof:
      if (!PP.isIncrementalProcessingEnabled())
        Actions.ActOnEndOfTranslationUnit();
      return true;
    default:
      break;
  }
  ParsedAttributesWithRange attrs(AttrFactory);
  Result = ParseExternalDeclaration(attrs);
  return false;
}

JavaParser::DeclGroupPtrTy JavaParser::ParseExternalDeclaration(ParsedAttributesWithRange &attrs,
                                                                ParsingDeclSpec *DS) {
  ParenBraceBracketBalancer BalancerRAIIObj(*this);
  Decl *SingleDecl = nullptr;

  if (PP.isCodeCompletionReached()) {
    cutOffParsing();
    return DeclGroupPtrTy();
  }

  switch (Tok.getKind()) {
    case tok::java_package:
      return ParseJavaPackageDefinition();
      break;
    case tok::java_import:
      return ParseJavaImport();
    default:
      SingleDecl = ParseJavaTypeDeclaration();
      break;
  }

  return Actions.ConvertDeclToDeclGroup(SingleDecl);
}

bool JavaParser::ParseJavaIdentifier(JavaSema::JavaClassPath &Ident,
                                     bool AcceptsWildcard, SourceLocation Loc,
                                     void (*CodeCompletion)(JavaParser *Parser, SourceLocation Loc, JavaSema::JavaClassPath Path)) {
  do {
    if (!Tok.is(tok::identifier) && 
        ((AcceptsWildcard && !Tok.is(tok::star)) || !AcceptsWildcard)) {
      if (Tok.is(tok::code_completion)) {
        CodeCompletion(this, Loc, Ident);
        cutOffParsing();
        return false;
      }
      
      SkipUntil(tok::semi);
      return false;
    }

    Ident.push_back(std::make_pair(Tok.getIdentifierInfo(), Tok.getLocation()));
    ConsumeToken();
    
    if (Tok.is(tok::period)) {
      ConsumeToken();
      continue;
    }
    
    break;
  } while (true);

  return true;
}

JavaParser::DeclGroupPtrTy JavaParser::ParseJavaPackageDefinition() {
  assert(Tok.is(tok::java_package) && "expected package");

  SourceLocation PackageLoc = ConsumeToken();
  JavaSema::JavaClassPath ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, PackageLoc, [] (JavaParser *Parser, SourceLocation Loc, JavaSema::JavaClassPath Path) {
    Parser->JavaActions()->CodeCompletePacakge(Loc, Path);
  })) {
    return DeclGroupPtrTy();
  }

  DeclResult Package = JavaActions()->ActOnJavaPackage(PackageLoc, ClassPath);
  ExpectAndConsumeSemi(diag::err_pacakge_expected_semi);
  // ExpectAndConsumeSemi(diag::err_expected_semi_declaration);

  return Actions.ConvertDeclToDeclGroup(Package.get());
}

JavaParser::DeclGroupPtrTy JavaParser::ParseJavaImport() {
  assert(Tok.is(tok::java_import) && "expected import");

  SourceLocation ImportLoc = ConsumeToken();

  JavaSema::JavaClassPath ClassPath;
  if (!ParseJavaIdentifier(ClassPath, true, ImportLoc, [] (JavaParser *Parser, SourceLocation Loc, JavaSema::JavaClassPath Path) {
    Parser->JavaActions()->CodeCompleteImport(Loc, Path);
  })) {
    return DeclGroupPtrTy();
  }

  DeclResult Import = JavaActions()->ActOnJavaImport(ImportLoc, ClassPath);
  ExpectAndConsumeSemi(diag::err_expected_semi_declaration);

  return Actions.ConvertDeclToDeclGroup(Import.get());;
}

JavaQualifiers JavaParser::ParseModifiers() {
  JavaQualifiers Quals;
  while (isTokenModifier()) {
    switch (Tok.getKind()) {
      case tok::java_public:
        Quals.addPublic();
        break;
      case tok::java_private:
        Quals.addPrivate();
        break;
      case tok::java_protected:
        Quals.addProtected();
        break;
      case tok::java_static:
        Quals.addStatic();
        break;
      case tok::java_final:
        Quals.addFinal();
        break;
      case tok::java_native:
        Quals.addNative();
        break;
      case tok::java_synchronized:
        Quals.addSynchronized();
        break;
      case tok::java_abstract:
        Quals.addAbstract();
        break;
      case tok::java_threadsafe:
        Quals.addThreadsafe();
        break;
      case tok::java_transient:
        Quals.addTransient();
        break;
    }
    ConsumeToken();
  }
  return Quals;
}

Decl *JavaParser::ParseJavaTypeDeclaration() {
  SourceLocation Loc;
  JavaQualifiers modifiers = ParseModifiers();
  
  if (Tok.is(tok::java_class)) {
    return ParseJavaClass(Loc, modifiers);
  } else if (Tok.is(tok::java_interface)) {
    return ParseJavaInterface(Loc, modifiers);
  } else {
    // TODO: Emit diag here
    return nullptr;
  }
}

Decl *JavaParser::ParseJavaClass(SourceLocation Loc ,JavaQualifiers modifiers) {
  assert(Tok.is(tok::java_class) && "expected class");

  ConsumeToken();
  if (!Tok.is(tok::identifier)) {
    // TODO: Emit diag here
    return nullptr;
  }
  
  JavaSema::JavaClassPath ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, Loc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaSema::JavaClassPath Path) {
    Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
  })) {
    return nullptr;
  }
  
  JavaSema::JavaClassPath Extends;
  SourceLocation ExtendsLoc;

  if (Tok.is(tok::java_extends)) {
    ExtendsLoc = Tok.getLocation();
    ConsumeToken();
    
    if (!ParseJavaIdentifier(Extends, false, ExtendsLoc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaSema::JavaClassPath Path) {
      Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
    })) {
      return nullptr;
    }
  }

  JavaSema::JavaClassPathList ImplementsList;
  SourceLocation ImplementsLoc;

  if (Tok.is(tok::java_implements)) {
    ImplementsLoc = Tok.getLocation();
    ConsumeToken();

    do {
      JavaSema::JavaClassPath Implements;
      
      if (!ParseJavaIdentifier(Implements, false, ImplementsLoc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaSema::JavaClassPath Path) {
        Parser->JavaActions()->CodeCompleteInterface(StartLoc, Path);
      })) {
        return nullptr;
      }
      
      ImplementsList.push_back(Implements);

      if (!Tok.is(tok::comma)) {
        break;
      }

      ConsumeToken();

    } while (true);
  }

  Decl *ClsType = JavaActions()->ActOnJavaClass(Loc, modifiers, ClassPath, ExtendsLoc, Extends, ImplementsLoc, ImplementsList);

  ParseJavaContainer(Loc, modifiers, ClsType, true);

  return ClsType;
}

Decl *JavaParser::ParseJavaInterface(SourceLocation Loc, JavaQualifiers modifiers) {
  assert(Tok.is(tok::java_interface) && "expected interface");

  ConsumeToken();
  if (!Tok.is(tok::identifier)) {
    // TODO: Emit diag here
    return nullptr;
  }
  
  JavaSema::JavaClassPath ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, Loc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaSema::JavaClassPath Path) {
    Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
  })) {
    return nullptr;
  }
  
  JavaSema::JavaClassPathList ExtendsList;
  SourceLocation ExtendsLoc;

  if (Tok.is(tok::java_extends)) {
    ExtendsLoc = Tok.getLocation();
    ConsumeToken();
    
    do {
      JavaSema::JavaClassPath Extends;
      
      if (!ParseJavaIdentifier(Extends, false, ExtendsLoc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaSema::JavaClassPath Path) {
        Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
      })) {
        return nullptr;
      }
      
      ExtendsList.push_back(Extends);

      if (!Tok.is(tok::comma)) {
        break;
      }

      ConsumeToken();

    } while (true);
  }

  Decl *InterfaceType = JavaActions()->ActOnJavaInterface(Loc, modifiers, ClassPath, ExtendsLoc, ExtendsList);

  ParseJavaContainer(Loc, modifiers, InterfaceType, false);

  return InterfaceType;
}

void JavaParser::ParseJavaContainer(SourceLocation Loc, JavaQualifiers modifiers, Decl *ContainerType, bool CanContainImplementations) {
  do {
    if (Tok.is(tok::java_static) && NextToken().is(tok::l_brace)) {
      if (CanContainImplementations) {
        Decl *SI = ParseJavaStaticInitializer(ContainerType);
        // TODO: Add SI to container
      } else {
        // TOOD: Emit diag
        SkipUntil(tok::r_brace);
        return;
      }
    }

    SourceLocation ElementLoc = Tok.getLocation();
    JavaQualifiers modifiers = ParseModifiers();

    if (Tok.is(tok::java_class)) {
      Decl *Inner = ParseJavaClass(ElementLoc, modifiers);
      // TODO: Add Inner to container
    } else if (Tok.is(tok::java_interface)) {
      Decl *Inner = ParseJavaInterface(ElementLoc, modifiers);
      // TODO: Add Inner to container
    } else if (Tok.is(tok::identifier) && NextToken().is(tok::l_paren)) {
      Decl *Constructor = ParseJavaConstructor(ElementLoc, modifiers);
      // TODO: Add Constructor to container
    } else {
      if (GetLookAheadToken(2).is(tok::l_paren)) {
        Decl *Method = ParseJavaMethod(ElementLoc, modifiers);
        // TODO: Add Constructor to container
      } else {
        SmallVector<Decl *, 2> Variables = ParseJavaVariable(ElementLoc, modifiers);
        // TODO: Add Variable to container
      }
    }

    if (Tok.is(tok::r_brace)) {
      break;
    }
  } while (true);
}

Decl *JavaParser::ParseJavaStaticInitializer(Decl *ContainerType) {
  assert(Tok.is(tok::java_static));
  SourceLocation StaticInitializerLoc = Tok.getLocation();
  ConsumeToken();
  StmtResult Body(ParseJavaStatement());
  return JavaActions()->ActOnJavaStaticInitializer(StaticInitializerLoc, Body);
}

StmtResult JavaParser::ParseJavaStatement(SourceLocation *TrailingElseLoc) {
  StmtResult Res;

  do {
    StmtVector Stmts;
    Res = ParseJavaStatementOrDeclaration(Stmts, true, TrailingElseLoc);
  } while (!Res.isInvalid() && !Res.get());

  return Res;
}

StmtResult
JavaParser::ParseJavaStatementOrDeclaration(StmtVector &Stmts, bool OnlyStatement,
                                            SourceLocation *TrailingElseLoc) {
  ParenBraceBracketBalancer BalancerRAIIObj(*this);
  const char *SemiError = nullptr;
  StmtResult Res;
  
  tok::TokenKind Kind  = Tok.getKind();
  SourceLocation AtLoc;
  switch (Kind) {
    case tok::code_completion:
      Actions.CodeCompleteOrdinaryName(getCurScope(), Sema::PCC_Statement);
      cutOffParsing();
      return StmtError();
    case tok::identifier:
    default:
      if (Tok.is(tok::r_brace)) {
        Diag(Tok, diag::err_expected_statement);
        Res = StmtError();
      } else {
        Res = ParseJavaExprStatement();
      }
      break;
    case tok::java_case:
      Res = ParseJavaCaseStatement();
      break;
    case tok::java_default:
      Res = ParseJavaDefaultStatement();
      break;
    case tok::l_brace:
      Res = ParseCompoundStatement();
      break;
    case tok::semi:
      Res = Actions.ActOnNullStmt(ConsumeToken(), false);
      break;

    case tok::java_if:
      Res = ParseJavaIfStatement(TrailingElseLoc);
      break;
    case tok::java_switch:
      Res = ParseJavaSwitchStatement(TrailingElseLoc);
      break;
    case tok::java_while:
      Res = ParseJavaWhileStatement(TrailingElseLoc);
      break;
    case tok::java_do:
      Res = ParseJavaDoStatement();
      SemiError = "do/while";
      break;
    case tok::java_for:
      Res = ParseJavaForStatement(TrailingElseLoc);
      break;
    case tok::java_goto:
      Res = ParseJavaGotoStatement();
      SemiError = "goto";
      break;
    case tok::java_continue:
      Res = ParseJavaContinueStatement();
      SemiError = "continue";
      break;
    case tok::java_break:
      Res = ParseJavaBreakStatement();
      SemiError = "break";
      break;
    case tok::java_return:
      Res = ParseJavaReturnStatement();
      SemiError = "return";
      break;
  }

  assert((Res.isInvalid() || Res.isUsable()) &&
         "attributes on empty statement");

  if (Res.isInvalid())
    return Res;

  return Actions.ProcessStmtAttributes(Res.get(), nullptr, SourceRange());
}

StmtResult JavaParser::ParseJavaIfStatement(SourceLocation *TrailingElseLoc) {
  return StmtError();
}

StmtResult JavaParser::ParseJavaSwitchStatement(SourceLocation *TrailingElseLoc) {
  return StmtError();
}

StmtResult JavaParser::ParseJavaWhileStatement(SourceLocation *TrailingElseLoc) {
  return StmtError();
}

StmtResult JavaParser::ParseJavaForStatement(SourceLocation *TrailingElseLoc) {
  return StmtError();
}

StmtResult JavaParser::ParseJavaExprStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaCaseStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaDefaultStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseCompoundStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaDoStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaGotoStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaContinueStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaBreakStatement() {
  return StmtError();
}

StmtResult JavaParser::ParseJavaReturnStatement() {
  return StmtError();
}

ParsedType JavaParser::ParseJavaType(bool isReturnType) {
  ParsedType Ty;
  
  if (isTokenIntrinsicTypeSpecifier()) {
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
        if (isReturnType) {

        } else {
          // TODO: Emit diag?
        }

        break;
      default:
        break;
    }
    ConsumeToken();
  } else {

  }

  return Ty;
}

Decl *JavaParser::ParseJavaParameter() {
  SourceLocation Loc = Tok.getLocation();
  ParsedType Ty = ParseJavaType(false);
  
  if (!Tok.is(tok::identifier)) {
    // TODO: emit diag
    return nullptr;
  }

  IdentifierInfo *II = Tok.getIdentifierInfo();
  ConsumeToken();
  return JavaActions()->ActOnJavaParameter(Loc, Ty, II);
}

void JavaParser::ParseJavaParameterList(SmallVector<Decl *, 8> Params) {
  while (Tok.isNot(tok::r_paren)) {
    Decl *param = ParseJavaParameter();
    
    if (param == nullptr) {
      // TODO: emit diag
      SkipUntil(tok::r_paren);
      break;
    } else {
      Params.push_back(param);
    }

    if (!Tok.is(tok::comma)) {
      break;
    }
  }

  ExpectAndConsume(tok::r_paren);
}

Decl *JavaParser::ParseJavaConstructor(SourceLocation Loc , JavaQualifiers modifiers) {
  assert(Tok.is(tok::identifier) && "expected identifier");
  IdentifierInfo *II = Tok.getIdentifierInfo();
  ConsumeToken();
  ExpectAndConsume(tok::l_paren);
  SmallVector<Decl *, 8> Args;
  ParseJavaParameterList(Args);
  ExpectAndConsume(tok::l_brace);
  StmtResult Body(ParseJavaStatement());
  return JavaActions()->ActOnJavaConstructor(Loc, modifiers, Args, Body);
}

Decl *JavaParser::ParseJavaMethod(SourceLocation Loc, JavaQualifiers modifiers) {
  ParsedType ReturnTy = ParseJavaType(true);

  if (!Tok.is(tok::identifier)) {
    // TODO: emit diag and skip to ; or }
    return nullptr;
  }

  IdentifierInfo *II = Tok.getIdentifierInfo();
  ConsumeToken();

  ExpectAndConsume(tok::l_paren);
  SmallVector<Decl *, 8> Args;
  ParseJavaParameterList(Args);

  Decl *Method = nullptr;

  if (Tok.is(tok::semi)) {
    Method = JavaActions()->ActOnJavaAbstractMethod(Loc, modifiers, ReturnTy, II, Args);
  } else {
    ExpectAndConsume(tok::l_brace);
    StmtResult Body(ParseJavaStatement());
    Method = JavaActions()->ActOnJavaMethod(Loc, modifiers, ReturnTy, II, Args, Body);
  }

  return Method;
}

Expr *JavaParser::ParseJavaExpression(SourceLocation Loc) {
  return nullptr;
}

Decl *JavaParser::ParseJavaVariable(SourceLocation Loc, JavaQualifiers modifiers, ParsedType Ty) {
  if (!Tok.is(tok::identifier)) {
    // TODO: Emit diag
    return nullptr;
  }

  IdentifierInfo *II = Tok.getIdentifierInfo();
  ConsumeToken();

  if (Tok.is(tok::l_square)) {
    ConsumeToken();
    // TODO: transform Ty into an array type
    ExpectAndConsume(tok::r_square);
  }

  SourceLocation InitializerLoc;
  Expr *Initializer = nullptr;
  
  if (Tok.is(tok::equal)) {
    InitializerLoc = Tok.getLocation();
    Initializer = ParseJavaExpression(InitializerLoc);
  }

  return JavaActions()->ActOnJavaVariable(Loc, modifiers, Ty, II, InitializerLoc, Initializer);
}

SmallVector<Decl *, 2> JavaParser::ParseJavaVariable(SourceLocation Loc, JavaQualifiers modifiers) {
  SmallVector<Decl *, 2> Variables;
  ParsedType Ty = ParseJavaType(true);
  do {
    Decl *Var = ParseJavaVariable(Loc, modifiers, Ty);
    
    if (Var) {
      Variables.push_back(Var);
    } else {
      SkipUntil(tok::semi);
      break;
    }
    
    if (Tok.is(tok::comma)) {
      continue;
    }

    break;
  } while (true);
  ExpectAndConsume(tok::semi);
  return Variables;
}

}
