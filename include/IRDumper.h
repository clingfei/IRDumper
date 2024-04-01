#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/Twine.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>
#include <iostream>
#include <set>


class LegacyIRDumper : public llvm::ModulePass {
    public:
        static char ID;
        
        LegacyIRDumper() : ModulePass(ID) {}

        bool runOnModule(llvm::Module &M) override;

        void saveModule(llvm::Module &M, llvm::Twine filename);

        void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;

        std::set<std::string> readFromFile(std::string path);

        void LoadDriverFiles(std::string path);
};