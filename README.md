# Compiler-Design
In my Compiler Design class project, I developed a grammar to accommodate the custom programming language bC. This language is a simplified version of C with a few custom features developed by Dr. "Joules BC" Beeston. This involved implementing a comprehensive compiler pipeline that encompassed lexical analysis via tokenization, rigorous syntax validation, and semantic analyis to construct abstract syntax trees (ASTs). The culmination of this effort was the generation of assembly code.

I used a combination of Flex and Bison code to build and drive a scanner for the bC programming language. 
The parser constructs the abstract syntax tree, performing semantic analysis and error generation. 
Syntax errors do not halt syntactic analysis in the compiler.
All errors and warnings have a consistent formatting.
After the tree is built, the compiler then computes the scope, size, and location of each symbol in an input program for code generation.
