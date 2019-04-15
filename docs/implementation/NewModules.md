## Module Implementation Guide
Follow these steps when adding new modules to the project:

#### Initial design:
- Figure out where the module fits. What module category does it belong in? Would it make more sense as a submodule of an existing module?
- Choose a short, descriptive name for the new module.
- Create module directories in `[ProjectDir]/Source` and `[ProjectDir]/Tests`.
- Create a makefile for the module at `[ProjectDir]/makefiles/[moduleName].mk`,using the [module makefile template](../templates/ModuleMakefile.mk) as a guide.
- Create a documentation file for the module at `[ProjectDir]/docs/modules/[ModuleName].md`, using the [module documentation template](../templates/ModuleDoc.md) as a guide.
