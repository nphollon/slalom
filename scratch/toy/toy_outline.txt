#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include <cstdio>
#include <string>
#include <map>
#include <vector>
using namespace llvm;
//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//
// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
};
static std::string IdentifierStr;  // Filled in if tok_identifier
static double NumVal;              // Filled in if tok_number
/// gettok - Return the next token from standard input.
static int gettok() {
}
//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
};
/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
};
/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
public:
};
/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
public:
};
/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
public:
};
/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
public:
};
/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
public:
};
/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
public:
};
/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
public:
};
//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//
/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() {
}
/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;
/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
}
/// Error* - These are little helper functions for error handling.
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }
static ExprAST *ParseExpression();
/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() {
}
/// numberexpr ::= number
static ExprAST *ParseNumberExpr() {
}
/// parenexpr ::= '(' expression ')'
static ExprAST *ParseParenExpr() {
}
/// ifexpr ::= 'if' expression 'then' expression 'else' expression
static ExprAST *ParseIfExpr() {
}
/// forexpr ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
static ExprAST *ParseForExpr() {
}
/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
///   ::= ifexpr
///   ::= forexpr
static ExprAST *ParsePrimary() {
}
/// binoprhs
///   ::= ('+' primary)*
static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
}
/// expression
///   ::= primary binoprhs
///
static ExprAST *ParseExpression() {
}
/// prototype
///   ::= id '(' id* ')'
static PrototypeAST *ParsePrototype() {
}
/// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {
}
/// toplevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr() {
}
/// external ::= 'extern' prototype
static PrototypeAST *ParseExtern() {
}
//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//
static Module *TheModule;
static IRBuilder<> Builder(getGlobalContext());
static std::map<std::string, Value*> NamedValues;
static FunctionPassManager *TheFPM;
Value *ErrorV(const char *Str) { Error(Str); return 0; }
Value *NumberExprAST::Codegen() {
}
Value *VariableExprAST::Codegen() {
}
Value *BinaryExprAST::Codegen() {
}
Value *CallExprAST::Codegen() {
}
Value *IfExprAST::Codegen() {
}
Value *ForExprAST::Codegen() {
}
Function *PrototypeAST::Codegen() {
}
Function *FunctionAST::Codegen() {
}
//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//
static ExecutionEngine *TheExecutionEngine;
static void HandleDefinition() {
}
static void HandleExtern() {
}
static void HandleTopLevelExpression() {
}
/// top ::= definition | external | expression | ';'
static void MainLoop() {
}
//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//
/// putchard - putchar that takes a double and returns 0.
extern "C" 
double putchard(double X) {
}
//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main() {
}
