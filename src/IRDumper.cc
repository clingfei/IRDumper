#include "IRDumper.h"

using namespace llvm;

char LegacyIRDumper::ID = 0;
static llvm::RegisterPass<LegacyIRDumper> X("IRDumper", "IRDumper pass", false,
                                            false);

void LegacyIRDumper::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

cl::opt<std::string> input("input",
                                    cl::desc("<input bitcode/asm files>"),
                                    cl::value_desc("filename"));

static std::set<std::string> file_list;

static bool isLoaded = false;

void LegacyIRDumper::saveModule(llvm::Module &M, llvm::Twine filename) {
  int bc_fd, ll_fd;
  llvm::StringRef FN = filename.getSingleStringRef();
  llvm::sys::fs::openFileForWrite(FN.take_front(FN.size() - 2) + ".bc", bc_fd);
  llvm::raw_fd_ostream bc_file(bc_fd, true, true);
  llvm::WriteBitcodeToFile(M, bc_file);
  llvm::sys::fs::openFileForWrite(FN.take_front(FN.size() - 2) + ".ll", ll_fd);
  llvm::raw_fd_ostream ll_file(ll_fd, true, true);
  M.print(ll_file, nullptr);
  llvm::errs() << "Writing " << FN.take_front(FN.size() - 2) + ".bc\n";
}

std::set<std::string> LegacyIRDumper::readFromFile(std::string path) {
  std::set<std::string> ret;
  std::ifstream ReadFile(path);
  if (!ReadFile.good()) {
    llvm::outs() << "\033[31m Error! File " << path << " not found\033[0m\n";
    return ret;
  }
  for (std::string line; std::getline(ReadFile, line);) {
    ret.insert(line);
  }
  return ret;
}

bool LegacyIRDumper::runOnModule(llvm::Module &M) {
  if (!isLoaded) {
    file_list = readFromFile(input);
    isLoaded = true;
  }
  if (file_list.find(M.getSourceFileName()) != file_list.end())
    saveModule(M, M.getName());

  return false;
}

static void registerLegacyPass(const llvm::PassManagerBuilder &,
                               llvm::legacy::PassManagerBase &PM) {
  PM.add(new LegacyIRDumper());
}

static llvm::RegisterStandardPasses
    RegisterLegacyPass(llvm::PassManagerBuilder::EP_OptimizerLast,
                       registerLegacyPass);

static llvm::RegisterStandardPasses
    RegisterLegacyPass0(llvm::PassManagerBuilder::EP_EnabledOnOptLevel0,
                        registerLegacyPass);
