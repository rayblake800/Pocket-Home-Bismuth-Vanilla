# Documenting and Commenting within C++ Files
All .cpp and .h files should have complete Javadoc-style Doxygen documentation. When uncertain, it's always betterto have too much documentation than to have too little. It's easy to ignore a javadoc tag that just states the obvious, but difficult to track down problems when people have different ideas of what is obvious and documentation isn't provide.

## Documentation blocks:
- Documentation comment blocks should be formatted as follows, indenting the block so that the opening '/' character aligns with the start of the documented section.
```
 /**
  *
  */
```
- Documentation blocks should always be placed in the following locations:
  * At the top of any header file.
  * Before all non-empty function declarations.
  * Before all class definitions.

## Additional comments:
- Place at least one space after starting a comment line with '//'.
- Outside of documentation blocks, always use line comments instead of block comments. This makes it easier to temporarily disable sections of code using block comments.
