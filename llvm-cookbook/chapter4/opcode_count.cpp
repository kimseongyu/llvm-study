#include <map>

#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class OpcodeCountPass : public PassInfoMixin<OpcodeCountPass> {
 private:
  std::map<std::string, int> opcodeCounter;

 public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    errs() << "Function " << F.getName() << "\n";

    for (auto &BB : F) {
      for (auto &I : BB) {
        if (opcodeCounter.find(I.getOpcodeName()) == opcodeCounter.end())
          opcodeCounter[I.getOpcodeName()] = 1;
        else
          opcodeCounter[I.getOpcodeName()] += 1;
      }
    }

    for (const auto &[opcode, count] : opcodeCounter) {
      errs() << opcode << ": " << count << "\n";
    }

    errs() << "\n";
    opcodeCounter.clear();

    return PreservedAnalyses::all();
  }
  static bool isRequired() { return true; }
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "OpcodeCount", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "opcode-count") {
                    FPM.addPass(OpcodeCountPass());
                    return true;
                  }
                  return false;
                });
          }};
}