// This code doesn't work as intended in the book.
// It have to be modified to work with the LLVM 20 version.
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

class EverythingMustAliasAAResult : public AAResultBase {
 public:
  explicit EverythingMustAliasAAResult() = default;
  EverythingMustAliasAAResult(EverythingMustAliasAAResult &&) = default;

  AliasResult alias(const MemoryLocation &LocA, const MemoryLocation &LocB,
                    AAQueryInfo &AAQI) const {
    return AliasResult::MustAlias;
  }
};

class EverythingMustAliasAA : public AnalysisInfoMixin<EverythingMustAliasAA> {
  friend AnalysisInfoMixin<EverythingMustAliasAA>;
  static AnalysisKey Key;

 public:
  using Result = EverythingMustAliasAAResult;
  Result run(Function &F, FunctionAnalysisManager &AM) {
    return EverythingMustAliasAAResult();
  }
};

AnalysisKey EverythingMustAliasAA::Key;

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "EverythingMustAlias", "v0.1",
          [](PassBuilder &PB) {
            PB.registerAnalysisRegistrationCallback(
                [](FunctionAnalysisManager &FAM) {
                  FAM.registerPass([&] { return EverythingMustAliasAA(); });
                });
          }};
}