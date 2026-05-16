#include <fstream>
#include <filesystem>
#include <core/types.hpp>
#include <core/config.hpp>
#include <core/execontext.hpp>
#include <core/isa.hpp>
#include <meta/pools/name.hpp>
#include <meta/pools/type.hpp>
#include <meta/pools/constant.hpp>
#include <meta/pools/variable.hpp>
#include <meta/pools/function.hpp>
#include <meta/pools/array.hpp>
#include <meta/pools/struct.hpp>
#include <bytecode/loader.hpp>

template<typename Ty>
Ty loadNumeric(std::ifstream& file) {
    Ty numeric;
    file.read(reinterpret_cast<char*>(&numeric), sizeof(Ty));

    return numeric;
}

std::string loadString(std::ifstream& file) {
    uint8 length = loadNumeric<uint8>(file);

    std::string str;
    str.resize(length);

    file.read(str.data(), length);

    return str;
}

LoadStatus checkHeader(BytecodeHeader& header) {
    if (header.magic != bytecodeMagic) return {LoadStatus::CORRUPTED_FILE};
    if (header.bytecodeVersionMajor > bytecodeVersionMajor || (header.bytecodeVersionMajor == bytecodeVersionMajor && header.bytecodeVersionMinor > bytecodeVersionMinor))  return {LoadStatus::UNSUPPORTED_VERSION};

    return LoadStatus::OK;
}

LoadStatus loadHeader(std::ifstream& file, BytecodeHeader& header) {
    header.magic = loadNumeric<uint32>(file);

    header.bytecodeVersionMajor = loadNumeric<uint8>(file);
    header.bytecodeVersionMinor = loadNumeric<uint8>(file);

    header.codegenName = loadString(file);

    header.codegenVersionMajor = loadNumeric<uint8>(file);
    header.codegenVersionMinor = loadNumeric<uint8>(file);

    return checkHeader(header);
}

void loadNamePool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) {
        MetaNameEntry entry;
        entry.name = loadString(file);

        context.namePool.push_back(entry);
    }

    context.namePool.shrink_to_fit();
}

void loadTypePool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) {
        MetaTypeEntry entry;
        entry.typeName = loadNumeric<Index>(file);
        entry.typeBinding = static_cast<ValueType>(loadNumeric<uint8>(file));
        entry.subtypeBinding = loadNumeric<Index>(file);
        entry.externalTypeBinding = loadNumeric<Index>(file);

        context.typePool.push_back(entry);
    }

    context.typePool.shrink_to_fit();
}

MetaVariableEntry loadVariableEntry(std::ifstream& file) {
    MetaVariableEntry entry;
    entry.variableName = loadNumeric<Index>(file);
    entry.variableType = loadNumeric<Index>(file);

    return entry;
}

void loadGlobalVarPool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) context.globalVarPool.push_back(loadVariableEntry(file));
}

void loadLocalVarPool(std::ifstream& file, MetaFunctionEntry& functionEntry) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) functionEntry.localVarPool.push_back(loadVariableEntry(file));
}

void loadCatchBlockPool(std::ifstream& file, MetaTryBlockEntry& tryBlockEntry) {
    uint8 elemCount = loadNumeric<uint8>(file);

    for (uint8 i = 0; i < elemCount; i++) {
        MetaCatchBlockEntry entry;
        entry.codeStart = loadNumeric<Index>(file);
        entry.codeEnd = loadNumeric<Index>(file);

        entry.catchArg = loadNumeric<Index>(file);

        tryBlockEntry.catchBlockPool.push_back(entry);
    }
}

void loadTryBlockPool(std::ifstream& file, MetaFunctionEntry& functionEntry) {
    uint8 elemCount = loadNumeric<uint8>(file);

    for (uint8 i = 0; i < elemCount; i++) {
        MetaTryBlockEntry entry;
        entry.codeStart = loadNumeric<Index>(file);
        entry.codeEnd = loadNumeric<Index>(file);

        loadCatchBlockPool(file, entry);

        functionEntry.tryBlockPool.push_back(entry);
    }
}

void loadFunctionPool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) {
        MetaFunctionEntry entry;
        entry.functionName = loadNumeric<Index>(file);

        loadLocalVarPool(file, entry);
        loadTryBlockPool(file, entry);

        entry.codeStart = loadNumeric<Index>(file);
        entry.codeEnd = loadNumeric<Index>(file);
        entry.returnType = loadNumeric<Index>(file);

        context.functionPool.push_back(entry);
    }
}

void loadMemberPool(std::ifstream& file, MetaStructEntry& structEntry) {
    uint8 elemCount = loadNumeric<uint8>(file);

    for (uint8 i = 0; i < elemCount; i++) {
        MetaStructMemberEntry entry;
        entry.memberName = loadNumeric<Index>(file);
        entry.memberType = loadNumeric<Index>(file);

        structEntry.memberPool.push_back(entry);
    }
}

void loadArrayPool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) {
        MetaArrayEntry entry;
        entry.memberType = loadNumeric<Index>(file);

        context.arrayPool.push_back(entry);
    }
}

void loadStructPool(std::ifstream& file, ExecutionContext& context) {
    uint16 elemCount = loadNumeric<uint16>(file);

    for (uint16 i = 0; i < elemCount; i++) {
        MetaStructEntry entry;
        
        loadMemberPool(file, entry);

        context.structPool.push_back(entry);
    }
}

void loadPools(std::ifstream& file, ExecutionContext& context) {
    loadNamePool(file, context);
    loadTypePool(file, context);
    loadGlobalVarPool(file, context);
    loadFunctionPool(file, context);
    loadArrayPool(file, context);
    loadStructPool(file, context);
}

void loadCode(std::ifstream& file, InstructionList& list) {

}

LoadResult loadBytecode(std::string path) {
    LoadResult result{};

    if (!std::filesystem::exists(path)) return {LoadStatus::FILE_NOT_FOUND};

    std::ifstream file(path, std::ios::binary);

    if (!file.good()) return {LoadStatus::FILE_OPEN_ERROR};

    result.status = loadHeader(file, result.context.codeHeader);

    if (result.status == LoadStatus::OK) {
        loadPools(file, result.context);
        loadCode(file, result.list);
    }

    return result;
}