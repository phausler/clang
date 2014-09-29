#ifndef IDENT_NS
#define IDENT_NS(X, Y)
#endif

IDENT_NS(JavaPackage  , 0)
IDENT_NS(JavaImport   , 0)
IDENT_NS(JavaInterface, IDNS_Ordinary | IDNS_Type)
IDENT_NS(JavaClass    , IDNS_Ordinary | IDNS_Type)

#undef IDENT_NS