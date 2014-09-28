#include "clang/Sema/JavaSema.h"
#include "clang/Parse/JavaParser.h"
#include "clang/AST/JavaDecl.h"
#include "clang/AST/JavaASTContext.h"

namespace clang {

Parser *JavaSema::createParser(bool SkipFunctionBodies) {
  return new JavaParser(getPreprocessor(), *this, SkipFunctionBodies);
}

void JavaSema::CodeCompletePacakge(SourceLocation PacakgeLoc, PacakgeClassPath Path) {

}

DeclResult JavaSema::ActOnJavaPackage(SourceLocation PacakgeLoc, PacakgeClassPath Path) {
	JavaPackageDecl *Package = JavaPackageDecl::Create(Context, Context.getTranslationUnitDecl(),
							 							PacakgeLoc, Path);
	Context.getTranslationUnitDecl()->addDecl(Package);
	return Package;
}

}
