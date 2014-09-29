#include "clang/AST/Frontends/Java/JavaDecl.h"

namespace clang {

JavaPackageDecl::JavaPackageDecl(DeclContext *DC, 
                                 SourceLocation StartLoc,
                                 ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath) :
  Decl(JavaPackage, DC, StartLoc) {
  // SourceLocation *StoredLocs = reinterpret_cast<SourceLocation *>(this + 1);
  // memcpy(StoredLocs, IdentifierLocs.data(), 
  //        IdentifierLocs.size() * sizeof(SourceLocation));
}

JavaPackageDecl *JavaPackageDecl::Create(ASTContext &C, DeclContext *DC, 
                                         SourceLocation StartLoc,
                                         ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath) {
  return new (C, DC, ClassPath.size() * sizeof(std::pair<IdentifierInfo *, SourceLocation>))
      JavaPackageDecl(DC, StartLoc, ClassPath);
}

JavaImportDecl::JavaImportDecl(DeclContext *DC, 
                               SourceLocation StartLoc,
                               ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath) :
  Decl(JavaImport, DC, StartLoc) {
  // SourceLocation *StoredLocs = reinterpret_cast<SourceLocation *>(this + 1);
  // memcpy(StoredLocs, IdentifierLocs.data(), 
  //        IdentifierLocs.size() * sizeof(SourceLocation));
}

JavaImportDecl *JavaImportDecl::Create(ASTContext &C, DeclContext *DC, 
                                       SourceLocation StartLoc,
                                       ArrayRef<std::pair<IdentifierInfo *, SourceLocation>> ClassPath) {
  return new (C, DC, ClassPath.size() * sizeof(std::pair<IdentifierInfo *, SourceLocation>))
      JavaImportDecl(DC, StartLoc, ClassPath);
}

}