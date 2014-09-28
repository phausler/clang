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
      SingleDecl = ParseJavaImport();
    default:
      break;
  }

  return Actions.ConvertDeclToDeclGroup(SingleDecl);
}

JavaParser::DeclGroupPtrTy JavaParser::ParseJavaPackageDefinition() {
   assert(Tok.is(tok::java_package) && "expected package");

   SourceLocation PackageLoc = ConsumeToken();
   SmallVector<std::pair<IdentifierInfo *, SourceLocation>, 2> ClassPath;
   do {
    if (!Tok.is(tok::identifier)) {
      if (Tok.is(tok::code_completion)) {
        JavaActions()->CodeCompletePacakge(PackageLoc, ClassPath);
        cutOffParsing();
        return DeclGroupPtrTy();
      }
      
      // Diag(Tok, diag::err_package_expected_ident);
      SkipUntil(tok::semi);
      return DeclGroupPtrTy();
    }

    ClassPath.push_back(std::make_pair(Tok.getIdentifierInfo(), Tok.getLocation()));
    ConsumeToken();
    
    if (Tok.is(tok::period)) {
      ConsumeToken();
      continue;
    }
    
    break;
  } while (true);

  DeclResult Package = JavaActions()->ActOnJavaPackage(PackageLoc, ClassPath);
  ExpectAndConsumeSemi(diag::err_pacakge_expected_semi);
  // ExpectAndConsumeSemi(diag::err_expected_semi_declaration);

  return Actions.ConvertDeclToDeclGroup(Package.get());
}

Decl *JavaParser::ParseJavaImport() {
  assert(Tok.is(tok::java_import) && "expected import");

  SourceLocation importLoc = ConsumeToken();
  return nullptr;
}

}