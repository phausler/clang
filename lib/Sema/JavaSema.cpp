#include "clang/Sema/JavaSema.h"
#include "clang/Parse/JavaParser.h"
#include "clang/AST/JavaDecl.h"
#include "clang/AST/JavaASTContext.h"

namespace clang {

Parser *JavaSema::createParser(bool SkipFunctionBodies) {
  return new JavaParser(getPreprocessor(), *this, SkipFunctionBodies);
}

void JavaSema::CodeCompletePacakge(SourceLocation PacakgeLoc, JavaClassPath Path) {

}

void JavaSema::CodeCompleteImport(SourceLocation ImportLoc, JavaClassPath Path) {

}

void JavaSema::CodeCompleteClass(SourceLocation ClassLoc, JavaClassPath Path) {

}

DeclResult JavaSema::ActOnJavaPackage(SourceLocation PacakgeLoc, JavaClassPath Path) {
  JavaPackageDecl *Package = JavaPackageDecl::Create(Context, Context.getTranslationUnitDecl(),
                                                     PacakgeLoc, Path);
  Context.getTranslationUnitDecl()->addDecl(Package);
  return Package;
}

DeclResult JavaSema::ActOnJavaImport(SourceLocation PacakgeLoc, JavaClassPath Path) {
  JavaImportDecl *Package = JavaImportDecl::Create(Context, Context.getTranslationUnitDecl(),
                                                   PacakgeLoc, Path);
  Context.getTranslationUnitDecl()->addDecl(Package);
  return Package;
}

}
