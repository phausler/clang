#include "clang/Parse/JavaParser.h"
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

bool JavaParser::ParseJavaIdentifier(JavaPathIdentifier &Ident,
                                     bool AcceptsWildcard, SourceLocation Loc,
                                     void (*CodeCompletion)(JavaParser *Parser, SourceLocation Loc, JavaPathIdentifier Path)) {
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
  JavaPathIdentifier ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, PackageLoc, [] (JavaParser *Parser, SourceLocation Loc, JavaPathIdentifier Path) {
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

  JavaPathIdentifier ClassPath;
  if (!ParseJavaIdentifier(ClassPath, true, ImportLoc, [] (JavaParser *Parser, SourceLocation Loc, JavaPathIdentifier Path) {
    Parser->JavaActions()->CodeCompleteImport(Loc, Path);
  })) {
    return DeclGroupPtrTy();
  }

  DeclResult Import = JavaActions()->ActOnJavaImport(ImportLoc, ClassPath);
  ExpectAndConsumeSemi(diag::err_expected_semi_declaration);

  return Actions.ConvertDeclToDeclGroup(Import.get());;
}

Decl *JavaParser::ParseJavaTypeDeclaration() {
  SourceLocation Loc;
  while (isTokenModifier()) {
    ConsumeToken();
  }

  if (Tok.is(tok::java_class)) {
    return ParseJavaClass(Loc /*,modifiers*/);
  } else if (Tok.is(tok::java_interface)) {
    return ParseJavaInterface(Loc /*,modifiers*/);
  } else {
    // TODO: Emit diag here
    return nullptr;
  }
}

Decl *JavaParser::ParseJavaClass(SourceLocation Loc /*,modifiers*/) {
  assert(Tok.is(tok::java_class) && "expected class");

  ConsumeToken();
  if (!Tok.is(tok::identifier)) {
    // TODO: Emit diag here
    return nullptr;
  }
  
  JavaPathIdentifier ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, Loc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaPathIdentifier Path) {
    Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
  })) {
    return nullptr;
  }
  
  JavaPathIdentifier Extends;
  SourceLocation ExtendsLoc;

  if (Tok.is(tok::java_extends)) {
    ExtendsLoc = Tok.getLocation();
    ConsumeToken();
    
    if (!ParseJavaIdentifier(Extends, false, ExtendsLoc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaPathIdentifier Path) {
      Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
    })) {
      return nullptr;
    }
  }

  return nullptr;
}

Decl *JavaParser::ParseJavaInterface(SourceLocation Loc /*,modifiers*/) {
  assert(Tok.is(tok::java_interface) && "expected interface");

  ConsumeToken();
  if (!Tok.is(tok::identifier)) {
    // TODO: Emit diag here
    return nullptr;
  }
  
  JavaPathIdentifier ClassPath;
  if (!ParseJavaIdentifier(ClassPath, false, Loc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaPathIdentifier Path) {
    Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
  })) {
    return nullptr;
  }
  
  JavaPathIdentifier Extends;
  SourceLocation ExtendsLoc;

  if (Tok.is(tok::java_extends)) {
    ExtendsLoc = Tok.getLocation();
    ConsumeToken();
    
    if (!ParseJavaIdentifier(Extends, false, ExtendsLoc, [] (JavaParser *Parser, SourceLocation StartLoc, JavaPathIdentifier Path) {
      Parser->JavaActions()->CodeCompleteClass(StartLoc, Path);
    })) {
      return nullptr;
    }
  }

  return nullptr;
}

}