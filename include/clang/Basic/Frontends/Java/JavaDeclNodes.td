def JavaPackage: Decl, DeclContext;
  def JavaImport: Decl;
  def JavaContainer: DDecl<Named, 1>, DeclContext;
    def JavaClass : DDecl<JavaContainer>;
    def JavaInterface : DDecl<JavaContainer>;