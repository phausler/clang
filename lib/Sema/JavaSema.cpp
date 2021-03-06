#include "clang/Sema/Frontends/Java/JavaSema.h"
#include "clang/Parse/Frontends/Java/JavaParser.h"
#include "clang/AST/Frontends/Java/JavaDecl.h"
#include "clang/AST/Frontends/Java/JavaASTContext.h"

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

void JavaSema::CodeCompleteInterface(SourceLocation ImplementsLoc, JavaClassPath Path) {

}

DeclResult JavaSema::ActOnJavaPackage(SourceLocation PacakgeLoc, JavaClassPath Path) {
  JavaPackageDecl *Package = JavaPackageDecl::Create(Context, Context.getTranslationUnitDecl(),
                                                     PacakgeLoc, Path);
  Context.getTranslationUnitDecl()->addDecl(Package);
  PushDeclContext(getCurScope(), Package);
  return Package;
}

DeclResult JavaSema::ActOnJavaImport(SourceLocation ImportLoc, JavaClassPath Path) {
  JavaImportDecl *Import = JavaImportDecl::Create(Context, Context.getTranslationUnitDecl(),
                                                   ImportLoc, Path);
  Context.getTranslationUnitDecl()->addDecl(Import);
  return Import;
}

Decl *JavaSema::ActOnBeginJavaClass(SourceLocation Loc, JavaQualifiers modifiers, 
                                    const IdentifierInfo *ClassPath, 
                                    SourceLocation ExtendsLoc, const IdentifierInfo *Extends, 
                                    SourceLocation ImplementsLoc, ArrayRef<const IdentifierInfo *> ImplementsList) {
  JavaClassDecl *Cls = JavaClassDecl::Create(Context, CurContext,
                                             Loc, modifiers, ClassPath, ExtendsLoc, Extends,
                                             ImplementsLoc, ImplementsList);
  PushDeclContext(getCurScope(), Cls);
  return Cls;
}

Decl *JavaSema::ActOnBeginJavaInterface(SourceLocation Loc, JavaQualifiers modifiers, 
                                        const IdentifierInfo *ClassPath, 
                                        SourceLocation ExtendsLoc, ArrayRef<const IdentifierInfo *> ExtendsList) {
  JavaInterfaceDecl *Iface = JavaInterfaceDecl::Create(Context, CurContext,
                                                       Loc, modifiers, ClassPath, ExtendsLoc, ExtendsList);
  PushDeclContext(getCurScope(), Iface);
  return Iface;
}

Decl *JavaSema::ActOnJavaStaticInitializer(SourceLocation Loc, StmtResult Body) {
  return nullptr;
}

Decl *JavaSema::ActOnJavaParameter(SourceLocation Loc, QualType QTy, TypeSourceInfo *TInfo, IdentifierInfo *II, SourceLocation IDLoc) {
  ParmVarDecl *Param = ParmVarDecl::Create(Context, CurContext,
                                           Loc, 
                                           IDLoc, II, 
                                           QTy, TInfo, 
                                           SC_Register, nullptr);

  return Param;
}

Decl *JavaSema::ActOnJavaConstructor(SourceLocation Loc, JavaQualifiers modifiers, SmallVector<Decl *, 8>Args, StmtResult Body) {
  return nullptr;
}

Decl *JavaSema::ActOnJavaAbstractMethod(SourceLocation Loc, JavaQualifiers modifiers, ParsedType ReturnTy, IdentifierInfo *II, SmallVector<Decl *, 8>Args) {
  return nullptr;
}

Decl *JavaSema::ActOnJavaMethod(SourceLocation Loc, JavaQualifiers modifiers, ParsedType ReturnTy, IdentifierInfo *II, SmallVector<Decl *, 8>Args, StmtResult Body) {
  return nullptr;
}

Decl *JavaSema::ActOnJavaVariable(SourceLocation Loc, JavaQualifiers modifiers, ParsedType Ty, IdentifierInfo *II, SourceLocation InitializerLoc, Expr *Initializer) {
  return nullptr;
}

}
