#include "clang/AST/JavaASTContext.h"
#include "clang/Sema/JavaSema.h"
namespace clang {

Sema* JavaASTContext::createSema(Preprocessor &pp, ASTConsumer &consumer,
                             TranslationUnitKind TUKind,
                             CodeCompleteConsumer *CompletionConsumer) {
  return new JavaSema(pp, *this, consumer, TUKind, CompletionConsumer);
}

}