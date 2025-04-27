#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

// =======================
// Token
// =======================
enum Token_Type {
  EOF_TOKEN = 0,

  NUMERIC_TOKEN,
  IDENTIFIER_TOKEN,
  PARAN_TOKEN,

  DEF_TOKEN,
};

static FILE *file;
static int Numeric_Val;
static std::string Identifier_string;

static int get_token() {
  static int LastChar = ' ';
  while (isspace(LastChar)) {
    LastChar = fgetc(file);
    if (LastChar == EOF) return EOF_TOKEN;
  }

  if (isalpha(LastChar)) {
    Identifier_string = LastChar;
    while (isalnum((LastChar = fgetc(file)))) Identifier_string += LastChar;

    if (Identifier_string == "def") return DEF_TOKEN;

    return IDENTIFIER_TOKEN;
  }

  if (isdigit(LastChar)) {
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = fgetc(file);
    } while (isdigit(LastChar));

    Numeric_Val = std::stoi(NumStr.c_str(), 0);
    return NUMERIC_TOKEN;
  }

  if (LastChar == '#') {
    do LastChar = fgetc(file);
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) return get_token();
  }

  if (LastChar == EOF) return EOF_TOKEN;

  int ThisChar = LastChar;
  LastChar = fgetc(file);
  return ThisChar;
}

// =======================
// AST Classes
// =======================
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual Value *Codegen() = 0;
};

class NumericAST : public BaseAST {
  int numeric_val;

 public:
  NumericAST(int val) : numeric_val(val) {}
  Value *Codegen() override;
};

class VariableAST : public BaseAST {
  std::string Var_Name;

 public:
  VariableAST(std::string &name) : Var_Name(name) {}
  Value *Codegen() override;
};

class BinaryAST : public BaseAST {
  std::string Bin_Operator;
  BaseAST *LHS, *RHS;

 public:
  BinaryAST(std::string op, BaseAST *lhs, BaseAST *rhs)
      : Bin_Operator(op), LHS(lhs), RHS(rhs) {}
  Value *Codegen() override;
};

class FunctionCallAST : public BaseAST {
  std::string Function_Callee;
  std::vector<BaseAST *> Function_Arguments;

 public:
  FunctionCallAST(const std::string &callee, std::vector<BaseAST *> &args)
      : Function_Callee(callee), Function_Arguments(args) {}
  Value *Codegen() override;
};

class FunctionDeclAST {
  std::string Func_Name;
  std::vector<std::string> Arguments;

 public:
  FunctionDeclAST(const std::string &name, const std::vector<std::string> &args)
      : Func_Name(name), Arguments(args) {}
  Function *Codegen();
};

class FunctionDefnAST {
  FunctionDeclAST *Func_Decl;
  BaseAST *Body;

 public:
  FunctionDefnAST(FunctionDeclAST *proto, BaseAST *body)
      : Func_Decl(proto), Body(body) {}
  Function *Codegen();
};

// =======================
// Parser
// =======================

static int Current_token;
static std::map<char, int> Operator_Precedence;

static int next_token() { return Current_token = get_token(); }

static int getBinOpPrecedence() {
  if (!isascii(Current_token)) return -1;

  int TokPrec = Operator_Precedence[Current_token];
  if (TokPrec <= 0) return -1;
  return TokPrec;
}

static BaseAST *expression_parser();

static BaseAST *numeric_parser() {
  BaseAST *Result = new NumericAST(Numeric_Val);
  next_token();
  return Result;
}

static BaseAST *identifier_parser() {
  std::string IdName = Identifier_string;
  next_token();  // eat identifier

  if (Current_token != '(') return new VariableAST(IdName);
  next_token();  // eat '('

  std::vector<BaseAST *> Args;
  if (Current_token != ')') {
    while (true) {
      BaseAST *Arg = expression_parser();
      if (!Arg) return nullptr;

      Args.push_back(Arg);

      if (Current_token == ')') break;

      if (Current_token != ',') return nullptr;
      next_token();  // eat ','
    }
  }

  next_token();  // eat ')'
  return new FunctionCallAST(IdName, Args);
}

static BaseAST *paran_parser() {
  next_token();  // eat '('
  BaseAST *V = expression_parser();
  if (!V) return nullptr;

  if (Current_token != ')') return nullptr;

  next_token();  // eat ')'
  return V;
}

static BaseAST *base_parser() {
  switch (Current_token) {
    case IDENTIFIER_TOKEN:
      return identifier_parser();
    case NUMERIC_TOKEN:
      return numeric_parser();
    case '(':
      return paran_parser();
    default:
      return nullptr;
  }
}

static BaseAST *binary_op_parser(int Old_Prec, BaseAST *LHS) {
  while (true) {
    int Operator_Prec = getBinOpPrecedence();

    if (Operator_Prec < Old_Prec) return LHS;

    int BinOp = Current_token;
    next_token();  // eat binary operator

    BaseAST *RHS = base_parser();
    if (!RHS) return nullptr;

    int Next_Prec = getBinOpPrecedence();
    if (Operator_Prec < Next_Prec) {
      RHS = binary_op_parser(Operator_Prec + 1, RHS);
      if (!RHS) return nullptr;
    }

    LHS = new BinaryAST(std::to_string(BinOp), LHS, RHS);
  }
}

static BaseAST *expression_parser() {
  BaseAST *LHS = base_parser();
  if (!LHS) return nullptr;

  return binary_op_parser(0, LHS);
}

static FunctionDeclAST *func_decl_parser() {
  if (Current_token != IDENTIFIER_TOKEN) return nullptr;

  std::string Function_Name = Identifier_string;
  next_token();  // eat identifier

  if (Current_token != '(') return nullptr;

  std::vector<std::string> Function_Argument_Names;
  while (next_token() == IDENTIFIER_TOKEN)
    Function_Argument_Names.push_back(Identifier_string);

  if (Current_token != ')') return nullptr;
  next_token();  // eat ')'

  return new FunctionDeclAST(Function_Name, Function_Argument_Names);
}

static FunctionDefnAST *func_defn_parser() {
  next_token();  // eat 'def'

  FunctionDeclAST *Decl = func_decl_parser();
  if (!Decl) return nullptr;

  if (BaseAST *Body = expression_parser())
    return new FunctionDefnAST(Decl, Body);
  return nullptr;
}

static FunctionDefnAST *top_level_parser() {
  if (BaseAST *E = expression_parser()) {
    FunctionDeclAST *Decl = new FunctionDeclAST("", std::vector<std::string>());
    return new FunctionDefnAST(Decl, E);
  }
  return nullptr;
}

// =======================
// Code Generation
// =======================

static Module *TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::map<std::string, Value *> Named_Values;

Value *NumericAST::Codegen() {
  return ConstantInt::get(Type::getInt32Ty(TheContext), numeric_val);
}

Value *VariableAST::Codegen() {
  Value *V = Named_Values[Var_Name];
  return V ? V : nullptr;
}

Value *BinaryAST::Codegen() {
  Value *L = LHS->Codegen();
  Value *R = RHS->Codegen();
  if (!L || !R) return nullptr;

  switch (std::stoi(Bin_Operator)) {
    case '+':
      return Builder.CreateAdd(L, R, "addtmp");
    case '-':
      return Builder.CreateSub(L, R, "subtmp");
    case '*':
      return Builder.CreateMul(L, R, "multmp");
    case '/':
      return Builder.CreateSDiv(L, R, "divtmp");
    default:
      return nullptr;
  }
}

Value *FunctionCallAST::Codegen() {
  Function *CalleeF = TheModule->getFunction(Function_Callee);

  std::vector<Value *> ArgsV;
  for (unsigned i = 0, e = Function_Arguments.size(); i != e; ++i) {
    ArgsV.push_back(Function_Arguments[i]->Codegen());
    if (!ArgsV.back()) return nullptr;
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *FunctionDeclAST::Codegen() {
  std::vector<Type *> Integers(Arguments.size(), Type::getInt32Ty(TheContext));
  FunctionType *FT =
      FunctionType::get(Type::getInt32Ty(TheContext), Integers, false);
  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Func_Name, TheModule);

  if (F->getName() != Func_Name) {
    F->eraseFromParent();
    F = TheModule->getFunction(Func_Name);

    if (!F->empty()) return nullptr;
    if (F->arg_size() != Arguments.size()) return nullptr;
  }

  unsigned Idx = 0;
  for (Function::arg_iterator AI = F->arg_begin(); Idx != Arguments.size();
       ++AI, ++Idx) {
    AI->setName(Arguments[Idx]);
    Named_Values[Arguments[Idx]] = AI;
  }

  return F;
}

Function *FunctionDefnAST::Codegen() {
  Named_Values.clear();

  Function *TheFunction = Func_Decl->Codegen();
  if (!TheFunction) return nullptr;

  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);

  if (Value *RetVal = Body->Codegen()) {
    Builder.CreateRet(RetVal);
    verifyFunction(*TheFunction);
    return TheFunction;
  }

  TheFunction->eraseFromParent();
  return nullptr;
}

// =======================
// Driver
// =======================
static void HandleDefn() {
  if (FunctionDefnAST *F = func_defn_parser()) {
    if (Function *LF = F->Codegen()) {
    }
  } else {
    next_token();
  }
}

static void HandleTopLevelExpression() {
  if (FunctionDefnAST *F = top_level_parser()) {
    if (Function *LF = F->Codegen()) {
    }
  } else {
    next_token();
  }
}

static void Driver() {
  while (true) {
    switch (Current_token) {
      case EOF_TOKEN:
        return;
      case ';':
        next_token();  // eat ';'
        break;
      case DEF_TOKEN:
        HandleDefn();
        break;
      default:
        HandleTopLevelExpression();
        break;
    }
  }
}

static void init_operator_precedence() {
  Operator_Precedence['-'] = 20;
  Operator_Precedence['+'] = 30;
  Operator_Precedence['/'] = 40;
  Operator_Precedence['*'] = 50;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
    return 1;
  }

  file = fopen(argv[1], "r");
  if (!file) {
    std::cerr << "Unable to open file: " << argv[1] << std::endl;
    return 1;
  }

  // Initialize LLVM
  LLVMContext &Context = TheContext;
  TheModule = new Module("toy compiler", Context);
  init_operator_precedence();

  // Run the main parser loop
  next_token();
  Driver();

  TheModule->print(outs(), nullptr);

  fclose(file);
  return 0;
}