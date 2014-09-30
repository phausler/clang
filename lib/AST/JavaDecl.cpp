#include "clang/AST/Frontends/Java/JavaDecl.h"

namespace clang {

JavaPackageDecl::JavaPackageDecl(DeclContext *DC, 
                                 SourceLocation StartLoc,
                                 JavaClassPath ClassPath) :
  Decl(JavaPackage, DC, StartLoc),
  DeclContext(JavaPackage) {
  // SourceLocation *StoredLocs = reinterpret_cast<SourceLocation *>(this + 1);
  // memcpy(StoredLocs, IdentifierLocs.data(), 
  //        IdentifierLocs.size() * sizeof(SourceLocation));
}

JavaPackageDecl *JavaPackageDecl::Create(ASTContext &C, DeclContext *DC, 
                                         SourceLocation StartLoc,
                                         JavaClassPath ClassPath) {
  return new (C, DC, ClassPath.size() * sizeof(std::pair<IdentifierInfo *, SourceLocation>))
      JavaPackageDecl(DC, StartLoc, ClassPath);
}

JavaImportDecl::JavaImportDecl(DeclContext *DC, 
                               SourceLocation StartLoc,
                               JavaClassPath ClassPath) :
  Decl(JavaImport, DC, StartLoc) {
  // SourceLocation *StoredLocs = reinterpret_cast<SourceLocation *>(this + 1);
  // memcpy(StoredLocs, IdentifierLocs.data(), 
  //        IdentifierLocs.size() * sizeof(SourceLocation));
}

JavaImportDecl *JavaImportDecl::Create(ASTContext &C, DeclContext *DC, 
                                       SourceLocation StartLoc,
                                       JavaClassPath ClassPath) {
  return new (C, DC, ClassPath.size() * sizeof(std::pair<IdentifierInfo *, SourceLocation>))
      JavaImportDecl(DC, StartLoc, ClassPath);
}

JavaContainerDecl::JavaContainerDecl(Decl::Kind DK, DeclContext *DC, SourceLocation L, const IdentifierInfo *ClassPath) :
  NamedDecl(DK, DC, L, DeclarationName(ClassPath)),
  DeclContext(DK) {

}

JavaClassDecl::JavaClassDecl(DeclContext *DC, SourceLocation StartLoc, const IdentifierInfo *ClassPath) :
  JavaContainerDecl(JavaClass, DC, StartLoc, ClassPath) {
    
}

JavaClassDecl *JavaClassDecl::Create(ASTContext &C, DeclContext *DC, 
                                     SourceLocation Loc, JavaQualifiers modifiers, 
                                     const IdentifierInfo *ClassPath, 
                                     SourceLocation ExtendsLoc, const IdentifierInfo *Extends, 
                                     SourceLocation ImplementsLoc, ArrayRef<const IdentifierInfo *> ImplementsList) {
  return new (C, DC, 0) JavaClassDecl(DC, Loc, ClassPath);
}

JavaInterfaceDecl::JavaInterfaceDecl(DeclContext *DC, SourceLocation StartLoc, const IdentifierInfo *ClassPath) :
  JavaContainerDecl(JavaInterface, DC, StartLoc, ClassPath) {
    
}

JavaInterfaceDecl *JavaInterfaceDecl::Create(ASTContext &C, DeclContext *DC, 
                                   SourceLocation Loc, JavaQualifiers modifiers, 
                                   const IdentifierInfo *ClassPath, 
                                   SourceLocation ExtendsLoc, ArrayRef<const IdentifierInfo *> ExtendsList) {
  return new (C, DC, 0) JavaInterfaceDecl(DC, Loc, ClassPath);
}

}