#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class FunctionBlockCount : public PassInfoMixin<FunctionBlockCount> {
 public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);

    errs() << "Function " << F.getName() + "\n";

    for (Loop *L : LI) countBlocksInLoop(L, 0);

    return PreservedAnalyses::all();
  }

 private:
  void countBlocksInLoop(Loop *L, unsigned nest) {
    unsigned num_Blocks = L->getBlocks().size();

    errs() << "Loop level" << nest << " has " << num_Blocks << " blocks\n";

    for (Loop *SubLoop : L->getSubLoops()) countBlocksInLoop(SubLoop, nest + 1);
  }
};

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "FunctionBlockCount", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "func-block-count") {
                    FPM.addPass(FunctionBlockCount());
                    return true;
                  }
                  return false;
                });
          }};
}
