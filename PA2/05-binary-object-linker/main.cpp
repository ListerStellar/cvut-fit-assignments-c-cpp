#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <memory>
#include <stdexcept>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#endif /* __PROGTEST__ */

struct SExport {
    std::string name;
    uint32_t offset;
};

struct SImport {
    std::string name;
    std::vector<uint32_t> offsets;
};

struct SObjectFile {
    std::vector<SExport> exports;
    std::vector<SImport> imports;
    std::vector<uint8_t> code;
};

struct SFunction {
    std::string name;
    const SObjectFile* parentFile;
    uint32_t startOffset;
    uint32_t endOffset;
};

uint32_t readUint32(std::istream& is) {
    uint32_t value = 0;
    if (!is.read(reinterpret_cast<char*>(&value), sizeof(value))) throw std::runtime_error("00");
    return value;
}

class CLinker {
public:
    CLinker() = default;
    ~CLinker() = default;
    CLinker(const CLinker&) = delete;
    CLinker &operator=(const CLinker&) = delete;

    CLinker &addFile(const std::string &fileName) {
        SObjectFile objectFile;
        std::ifstream f(fileName, std::ios::binary);
        if (!f) throw std::runtime_error("6");

        const uint32_t expNum = readUint32(f);
        const uint32_t imNum = readUint32(f);
        const uint32_t codeSize = readUint32(f);

        for (uint32_t i = 0; i < expNum; ++i) {
            uint8_t nameLen;
            if (!f.read(reinterpret_cast<char*>(&nameLen), 1)) throw std::runtime_error("7");
            std::string name(nameLen, '\0');
            if (!f.read(&name[0], nameLen)) throw std::runtime_error("8");
            const uint32_t offset = readUint32(f);
            objectFile.exports.push_back({ name, offset });
        }

        for (uint32_t i = 0; i < imNum; ++i) {
            uint8_t nameLen;
            if (!f.read(reinterpret_cast<char*>(&nameLen), 1)) throw std::runtime_error("9");
            std::string name(nameLen, '\0');
            if (!f.read(&name[0], nameLen)) throw std::runtime_error("10");
            const uint32_t useCount = readUint32(f);
            std::vector<uint32_t> positions(useCount);
            for (uint32_t j = 0; j < useCount; ++j) positions[j] = readUint32(f);
            objectFile.imports.push_back({ name, positions });
        }

        objectFile.code.resize(codeSize);
        if (!f.read(reinterpret_cast<char*>(&objectFile.code[0]), codeSize)) throw std::runtime_error("11");
        if (!f) throw std::runtime_error("12");
        objFiles.emplace_back(std::move(objectFile));
        return *this;
    }

    void linkOutput(const std::string &outFileName, const std::string &entryPoint) {
        expFuncs.clear();
        for (const auto &file : objFiles) {
            for (const auto &[name, offset] : file.exports) {
                if (expFuncs.contains(name)) throw std::runtime_error("2");
                SFunction function;
                function.name = name;
                function.parentFile = &file;
                function.startOffset = offset;
                function.endOffset = file.code.size();
                for (const auto &nxt_exp : file.exports) {
                    if (nxt_exp.offset > offset && nxt_exp.offset < function.endOffset)
                        function.endOffset = nxt_exp.offset;
                }
                expFuncs.emplace(name, function);
            }
        }
        if (!expFuncs.contains(entryPoint)) throw std::runtime_error("3");

        std::vector<uint8_t> output;
        std::map<std::string, uint32_t> funcAddr;
        std::set<std::string> visited;
        std::vector<std::string> toProcess = { entryPoint };

        while (!toProcess.empty()) {
            std::string name = toProcess.front();
            toProcess.erase(toProcess.begin());
            if (visited.contains(name)) continue;

            const SFunction& f = expFuncs.at(name);
            funcAddr[name] = output.size();

            std::vector<uint8_t> code(f.parentFile->code.begin() + f.startOffset, f.parentFile->code.begin() + f.endOffset);

            for (const auto &[name_, positions_] : f.parentFile->imports) {
                for (uint32_t pos : positions_) {
                    if (pos >= f.startOffset && pos < f.endOffset) {
                        if (!expFuncs.contains(name_)) throw std::runtime_error("4");
                        toProcess.push_back(name_);
                    }
                }
            }

            output.insert(output.end(), code.begin(), code.end());
            visited.insert(name);
        }

        for (const auto& name : visited) {
            const SFunction& f = expFuncs.at(name);
            uint32_t base = funcAddr.at(name);
            for (const auto&[name_, positions_] : f.parentFile->imports) {
                if (!funcAddr.contains(name_)) continue;
                uint32_t targetAddr = funcAddr.at(name_);
                for (uint32_t pos : positions_) {
                    if (pos >= f.startOffset && pos < f.endOffset) {
                        uint32_t relPos = base + (pos - f.startOffset);
                        std::memcpy(&output[relPos], &targetAddr, 4);
                    }
                }
            }
        }

        std::ofstream outFile(outFileName, std::ios::binary);
        if (!outFile) throw std::runtime_error("5");
        if (!outFile.write(reinterpret_cast<const char*>(&output[0]), output.size())) throw std::runtime_error("55");
    }

private:
    std::vector<SObjectFile> objFiles;
    std::map<std::string, SFunction> expFuncs;
};

#ifndef __PROGTEST__
int main() {
    CLinker().addFile("0in0.o").linkOutput("0out", "strlen");

    CLinker().addFile("1in0.o").linkOutput("1out", "main");

    CLinker().addFile("2in0.o").addFile("2in1.o").linkOutput("2out", "main");

    CLinker().addFile("3in0.o").addFile("3in1.o").linkOutput("3out", "towersOfHanoi");

    try {
        CLinker().addFile("4in1.o").linkOutput("4out", "unusedFunc");
        assert("missing an exception" == nullptr);
    } catch (const std::runtime_error &e) {
        // e . what (): Undefined symbol qsort
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        assert("invalid exception" == nullptr);
    }

    try {
        CLinker().addFile("5in0.o").linkOutput("5out", "main");
        assert("missing an exception" == nullptr);
    } catch (const std::runtime_error &e) {
        // e . what (): Duplicate symbol: printf
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        assert("invalid exception" == nullptr);
    }

    try {
        CLinker().addFile("6in0.o").linkOutput("6out", "strlen");
        assert("missing an exception" == nullptr);
    } catch (const std::runtime_error &e) {
        // e . what (): Cannot read input file
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        assert("invalid exception" == nullptr);
    }

    try {
        CLinker().addFile("7in0.o").linkOutput("7out", "strlen2");
        assert("missing an exception" == nullptr);
    } catch (const std::runtime_error &e) {
        // e . what (): Undefined symbol strlen2
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        assert("invalid exception" == nullptr);
    }

    CLinker () . addFile ( "0010_0.o" ) . addFile ( "0010_1.o" ) . addFile ( "0010_2.o" ) . addFile ( "0010_3.o" ) . linkOutput ( "0010_out", "pdrolowjjgdwxiadj" );
    CLinker () . addFile ( "0011_0.o" ) . addFile ( "0011_1.o" ) . linkOutput ( "0011_out", "yntvlhvtp" );
    CLinker () . addFile ( "0012_0.o" ) . addFile ( "0012_1.o" ) . addFile ( "0012_2.o" ) . linkOutput ( "0012_out", "acnskqfuegem" );
    CLinker () . addFile ( "0013_0.o" ) . addFile ( "0013_1.o" ) . addFile ( "0013_2.o" ) . linkOutput ( "0013_out", "yvjbkannhcusuktuhl" );
    CLinker () . addFile ( "0014_0.o" ) . addFile ( "0014_1.o" ) . addFile ( "0014_2.o" ) . linkOutput ( "0014_out", "adqcwiahautvfi" );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
