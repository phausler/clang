#include "clang/AST/Frontends/Java/JavaASTContext.h"
#include "clang/Sema/Frontends/Java/JavaSema.h"
namespace clang {

Sema* JavaASTContext::createSema(Preprocessor &pp, ASTConsumer &consumer,
                             TranslationUnitKind TUKind,
                             CodeCompleteConsumer *CompletionConsumer) {
  return new JavaSema(pp, *this, consumer, TUKind, CompletionConsumer);
}

}