#include "clang/AST/ASTContext.h"

namespace clang {
class CASTContext : public ASTContext {
public:
  CASTContext(LangOptions &LOpts, SourceManager &SM, IdentifierTable &idents,
              SelectorTable &sels, Builtin::Context &builtins) : 
    ASTContext(LOpts, SM, idents, sels, builtins) {

  }
  
  virtual Sema* createSema(Preprocessor &pp, ASTConsumer &consumer,
                             TranslationUnitKind TUKind = TU_Complete,
                             CodeCompleteConsumer *CompletionConsumer = nullptr);
};
}