#include <vector>
#include <set>
#include <algorithm>
#include <core/types.hpp>
#include <core/execontext.hpp>
#include <core/utils.hpp>
#include <core/config.hpp>
#include <core/nptypes/value.hpp>
#include <core/nptypes/variable.hpp>
#include <core/nptypes/reference.hpp>
#include <core/nptypes/lambda.hpp>
#include <core/nptypes/array.hpp>
#include <core/nptypes/struct.hpp>
#include <meta/pools/type.hpp>
#include <meta/pools/constant.hpp>
#include <meta/pools/variable.hpp>
#include <meta/pools/function.hpp>
#include <meta/pools/struct.hpp>
#include <core/runtime.hpp>

//-----------------------internal-----------------------

Index getCallFrameIndex(ExecutionContext& context) {
    return context.callStack.size() - 1;
}

Index getLastValueIndex(ExecutionContext& context) {
    return context.valueStack.size() - 1;
}

MetaTypeEntry& getTypeMeta(ExecutionContext& context, Index typeIndex) {
    return context.typePool[typeIndex];
}

MetaConstantEntry& getConstantMetaEntry(ExecutionContext& context, Index constIndex) {
    return context.constPool[constIndex];
}

MetaVariableEntry& getLocalVariableMetaEntry(ExecutionContext& context, Index varIndex) {
    CallFrame& frame = getCallFrame(context);
    VariableEntry& varEntry = getLocalVariableEntry(context, varIndex);

    return context.functionPool[frame.functionMeta].localVarPool[varEntry.variableMeta];
}

MetaVariableEntry& getGlobalVariableMetaEntry(ExecutionContext& context, Index varIndex) {
    return context.globalVarPool[getGlobalVariableEntry(context, varIndex).variableMeta];
}

MetaFunctionEntry& getFunctionMetaEntry(ExecutionContext& context, Index funcIndex) {
    return context.functionPool[funcIndex];
}

MetaArrayEntry& getArrayMetaEntry(ExecutionContext& context, Index arrayIndex) {
    return context.arrayPool[arrayIndex];
}

MetaStructEntry& getStructMetaEntry(ExecutionContext& context, Index structIndex) {
    return context.structPool[structIndex];
}

VariableEntry& getLocalVariableEntry(ExecutionContext& context, Index varIndex) {
    return getCallFrame(context).localVarTable[varIndex];
}

VariableEntry& getGlobalVariableEntry(ExecutionContext& context, Index varIndex) {
    return context.globalVarTable[varIndex];
}

ValueEntry& getValueEntry(ExecutionContext& context, Index valueIndex) {
    return context.valueTable.at(valueIndex);
}

ValueEntry& createValueEntry(ExecutionContext& context, Index valueIndex) {
    return context.valueTable[valueIndex];
}

ReferenceEntry& getReferenceEntry(ExecutionContext& context, Index refIndex) {
    return context.referenceTable.at(refIndex);
}

ReferenceEntry& createReferenceEntry(ExecutionContext& context, Index refIndex) {
    return context.referenceTable[refIndex];
}

LambdaEntry& getLambdaEntry(ExecutionContext& context, Index lambdaIndex) {
    return context.lambdaTable.at(lambdaIndex);
}

LambdaEntry& createLambdaEntry(ExecutionContext& context, Index lambdaIndex) {
    return context.lambdaTable[lambdaIndex];
}

ArrayEntry& getArrayEntry(ExecutionContext& context, Index arrayIndex) {
    return context.arrayTable.at(arrayIndex);
}

ArrayEntry& createArrayEntry(ExecutionContext& context, Index arrayIndex) {
    return context.arrayTable[arrayIndex];
}

StructEntry& getStructEntry(ExecutionContext& context, Index structIndex) {
    return context.structTable.at(structIndex);
}

StructEntry& createStructEntry(ExecutionContext& context, Index structIndex) {
    return context.structTable[structIndex];
}

Index getFreeValueIndex(ExecutionContext& context) {
    static Index freeIndex = 0;

    return freeIndex++;
}

Index getFreeReferenceIndex(ExecutionContext& context)  {
    static Index freeIndex = 0;

    return freeIndex++;
}

Index getFreeLambdaIndex(ExecutionContext& context)  {
    static Index freeIndex = 0;

    return freeIndex++;
}

Index getFreeArrayIndex(ExecutionContext& context)  {
    static Index freeIndex = 0;

    return freeIndex++;
}

Index getFreeStructIndex(ExecutionContext& context) {
    static Index freeIndex = 0;

    return freeIndex++;
}

Index findType(ExecutionContext& context, ValueType type, Index subtype) {
    for (Index i = 0; i < context.typePool.size(); i++) {
        MetaTypeEntry& typeEntry = getTypeMeta(context, i);

        if (typeEntry.typeBinding == type && typeEntry.subtypeBinding == subtype) return i;
    }

    return invalidIndex;
}

void buildValue(ExecutionContext& context, Index valueIndex, Index typeIndex, bool bound) {
    if (checkException(context)) return;

    ValueEntry& valEntry = createValueEntry(context, valueIndex);
    valEntry.refIndex = invalidIndex;
    valEntry.value.type = typeIndex;
    valEntry.bound = bound;

    MetaTypeEntry& typeEntry = getTypeMeta(context, typeIndex);

    switch (typeEntry.typeBinding) {
        case ValueType::STRUCT: {
            Index structIndex = getFreeStructIndex(context);

            buildStruct(context, structIndex, typeEntry.externalTypeBinding);

            valEntry.value.value = structIndex;
            valEntry.initialized = true;

            break;
        }

        case ValueType::ARRAY: {
            Index arrayIndex = getFreeArrayIndex(context);

            buildArray(context, arrayIndex, typeEntry.externalTypeBinding);

            valEntry.value.value = arrayIndex;
            valEntry.initialized = true;

            break;
        }

        default: {
            valEntry.value.value = invalidIndex;
            valEntry.initialized = false;

            break;
        }
    }
}

void buildArray(ExecutionContext& context, Index arrayIndex, Index arrayMetaIndex) {
    if (checkException(context)) return;

    ArrayEntry& arrayEntry = createArrayEntry(context, arrayIndex);
    arrayEntry.arrayMeta = arrayMetaIndex;
}

void buildStruct(ExecutionContext& context, Index structIndex, Index structMetaIndex) {
    if (checkException(context)) return;

    StructEntry& structEntry = createStructEntry(context, structIndex);
    structEntry.structMeta = structMetaIndex;

    MetaStructEntry& structMetaEntry = getStructMetaEntry(context, structMetaIndex);

    for (MetaStructMemberEntry& memberMeta : structMetaEntry.memberPool) {
        Index valueIndex = getFreeValueIndex(context);

        buildValue(context, valueIndex, memberMeta.memberType, true);

        structEntry.memberTable.push_back(valueIndex);
    }
}

bool checkPrimitiveType(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

    return typeEntry.typeBinding <= ValueType::BOOL;
}

bool checkReferenceType(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

    return typeEntry.typeBinding == ValueType::REFERECNE;
}

bool checkLambdaType(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

    return typeEntry.typeBinding == ValueType::LAMBDA;
}

bool checkBuildableType(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

    return typeEntry.typeBinding == ValueType::STRUCT || typeEntry.typeBinding == ValueType::ARRAY;
}

bool checkCastAllowed(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc) {
    ValueEntry& valDstEntry = getValueEntry(context, valueIndexDst);
    ValueEntry& valSrcEntry = getValueEntry(context, valueIndexSrc);

    return checkPrimitiveType(context, valueIndexDst) && checkPrimitiveType(context, valueIndexSrc);
}

void castStore(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc) {
    if (checkException(context)) return;

    ValueEntry& valDstEntry = getValueEntry(context, valueIndexDst);
    ValueEntry& valSrcEntry = getValueEntry(context, valueIndexSrc);

    if (!valSrcEntry.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    MetaTypeEntry& typeDstEntry = getTypeMeta(context, valDstEntry.value.type);

    switch (typeDstEntry.typeBinding) {
        case ValueType::INT8: { valDstEntry.value.value = castTo<int8>(valSrcEntry.value); break; }
        case ValueType::INT16: { valDstEntry.value.value = castTo<int16>(valSrcEntry.value); break; }
        case ValueType::INT32: { valDstEntry.value.value = castTo<int32>(valSrcEntry.value); break; }
        case ValueType::INT64: { valDstEntry.value.value = castTo<int64>(valSrcEntry.value); break; }
        case ValueType::UINT8: { valDstEntry.value.value = castTo<uint8>(valSrcEntry.value); break; }
        case ValueType::UINT16: { valDstEntry.value.value = castTo<uint16>(valSrcEntry.value); break; }
        case ValueType::UINT32: { valDstEntry.value.value = castTo<uint32>(valSrcEntry.value); break; }
        case ValueType::UINT64: { valDstEntry.value.value = castTo<uint64>(valSrcEntry.value); break; }
        case ValueType::FLOAT32: { valDstEntry.value.value = castTo<float32>(valSrcEntry.value); break; }
        case ValueType::FLOAT64: { valDstEntry.value.value = castTo<float64>(valSrcEntry.value); break; }
        case ValueType::BOOL: { valDstEntry.value.value = castTo<bool>(valSrcEntry.value); break; }
        default: { throwExceptionType(context, invalidIndex); break; }
    }
}

void copyValue(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc) {
    if (checkException(context)) return;

    ValueEntry& valDstEntry = getValueEntry(context, valueIndexDst);
    ValueEntry& valSrcEntry = getValueEntry(context, valueIndexSrc);

    if (!valSrcEntry.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }
    
    if (checkPrimitiveType(context, valueIndexSrc)) {
        valDstEntry.value = valSrcEntry.value;

        return;
    }

    if (checkReferenceType(context, valueIndexSrc)) {
        Index valueIndexDstRef = dereference(context, valueIndexDst);
        Index valueIndexSrcRef = dereference(context, valueIndexSrc);

        copyValue(context, valueIndexDstRef, valueIndexSrcRef);

        return;
    }

    if (checkBuildableType(context, valueIndexSrc)) {
        MetaTypeEntry& typeEntry = getTypeMeta(context, valSrcEntry.value.type);

        Index indexSrc = std::get<Index>(valSrcEntry.value.value);
        Index indexDst = std::get<Index>(valDstEntry.value.value);

        switch (typeEntry.typeBinding) {
            case ValueType::STRUCT: {
                StructEntry& structSrcEntry = getStructEntry(context, indexSrc);
                StructEntry& structDstEntry = getStructEntry(context, indexDst);

                for (Index i = 0; i < structSrcEntry.memberTable.size(); i++)
                    copyValue(context, structDstEntry.memberTable[i], structSrcEntry.memberTable[i]);
            
                break;
            }

            case ValueType::ARRAY: {
                ArrayEntry& arraySrcEntry = getArrayEntry(context, indexSrc);
                ArrayEntry& arrayDstEntry = getArrayEntry(context, indexDst);

                if (arrayDstEntry.valueArray.size() > arraySrcEntry.valueArray.size()) {
                    for (Index i = arraySrcEntry.valueArray.size(); i < arrayDstEntry.valueArray.size(); i++)
                        clearValue(context, arrayDstEntry.valueArray[i]);
                }

                arrayDstEntry.valueArray.resize(arraySrcEntry.valueArray.size());

                for (Index i = 0; i < arraySrcEntry.valueArray.size(); i++)
                    copyValue(context, arrayDstEntry.valueArray[i], arraySrcEntry.valueArray[i]);
            
                break;
            }
        }
    }

    else throwExceptionType(context, invalidIndex);
}

ClearList generateClearList(ExecutionContext& context, Index valueIndex) {
    ClearList clearList;
    clearList.push_back({ValueType::VALUE, SpecialAction::DELETE, valueIndex});

    ValueEntry& valEntry = getValueEntry(context, valueIndex);

    if (valEntry.refIndex != invalidIndex) clearList.push_back({ValueType::REFERECNE, SpecialAction::SETFLAG, valEntry.refIndex});

    if (checkReferenceType(context, valueIndex)) 
        clearList.push_back({ValueType::REFERECNE, SpecialAction::REDUCE, std::get<Index>(valEntry.value.value), 1});

    if (checkBuildableType(context, valueIndex)) {
        MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

        Index indexObj = std::get<Index>(valEntry.value.value);

        switch (typeEntry.typeBinding) {
            case ValueType::ARRAY: {
                ArrayEntry& arrayEntry = getArrayEntry(context, indexObj);

                for (Index i : arrayEntry.valueArray) {
                    ClearList tmp = generateClearList(context, i);

                    clearList.insert(clearList.end(), tmp.begin(), tmp.end());
                }

                clearList.push_back({ValueType::ARRAY, SpecialAction::DELETE, indexObj});

                break;
            }

            case ValueType::STRUCT: {
                StructEntry& structEntry = getStructEntry(context, indexObj);

                for (Index i : structEntry.memberTable) {
                    ClearList tmp = generateClearList(context, i);

                    clearList.insert(clearList.end(), tmp.begin(), tmp.end());
                }

                clearList.push_back({ValueType::STRUCT, SpecialAction::DELETE, indexObj});

                break;
            }
        }
    }

    return clearList;
}

void optimizeClearList(ExecutionContext& context, ClearList& clearList) {
    std::unordered_map<Index, uint16> refCounters;

    std::set<Index> flaggedRef;
    std::set<Index> deletedRef;

    std::set<Index> deletedValue;
    std::set<Index> deletedArray;
    std::set<Index> deletedStruct;

    for (ClearEntry& entry : clearList)
        if (entry.type == ValueType::REFERECNE)
            refCounters[entry.index] = getReferenceEntry(context, entry.index).count;
    
    for (ClearEntry& entry : clearList) {
        if (entry.type == ValueType::VALUE && context.valueTable.find(entry.index) != context.valueTable.end()) deletedValue.insert(entry.index);
        else if (entry.type == ValueType::ARRAY && context.arrayTable.find(entry.index) != context.arrayTable.end()) deletedArray.insert(entry.index);
        else if (entry.type == ValueType::STRUCT && context.structTable.find(entry.index) != context.structTable.end()) deletedStruct.insert(entry.index);
        else if (entry.type == ValueType::REFERECNE && context.referenceTable.find(entry.index) != context.referenceTable.end()) {
            if (entry.action == SpecialAction::REDUCE) {
                uint16& count = refCounters[entry.index];

                if (count > 1) count--;
                else {
                    deletedRef.insert(entry.index);

                    refCounters.erase(entry.index);
                }
            }

            else if (entry.action == SpecialAction::SETFLAG) flaggedRef.insert(entry.index);
        }
    }

    clearList.clear();

    for (Index index : deletedValue) clearList.push_back({ValueType::VALUE, SpecialAction::DELETE, index});
    for (Index index : deletedArray) clearList.push_back({ValueType::ARRAY, SpecialAction::DELETE, index});
    for (Index index : deletedStruct) clearList.push_back({ValueType::STRUCT, SpecialAction::DELETE, index});
    for (Index index : deletedRef) clearList.push_back({ValueType::REFERECNE, SpecialAction::DELETE, index});

    for (Index index : flaggedRef)
        if (deletedRef.find(index) == deletedRef.end()) clearList.push_back({ValueType::REFERECNE, SpecialAction::SETFLAG, index});

    for (auto [key, value] : refCounters) clearList.push_back({ValueType::REFERECNE, SpecialAction::REDUCE, key, value});

}

void executeClearList(ExecutionContext& context, const ClearList& clearList) {
    if (checkException(context)) return;

    for (const ClearEntry& entry : clearList) {
        if (entry.type == ValueType::VALUE) context.valueTable.erase(entry.index);
        else if (entry.type == ValueType::ARRAY) context.arrayTable.erase(entry.index);
        else if (entry.type == ValueType::STRUCT) context.structTable.erase(entry.index);
        else if (entry.type == ValueType::REFERECNE) {
            if (entry.action == SpecialAction::DELETE) context.referenceTable.erase(entry.index);
            else if (entry.action == SpecialAction::REDUCE) getReferenceEntry(context, entry.index).count = entry.count;
            else if (entry.action == SpecialAction::SETFLAG) getReferenceEntry(context, entry.index).alive = false;
        }
    }
}

CallFrame& getCallFrame(ExecutionContext& context) {
    return context.callStack.back();
}

CallFrame& pushCallFrame(ExecutionContext& context) {
    return context.callStack.emplace_back();
}

void popCallFrame(ExecutionContext& context) {
    context.callStack.pop_back();
}

void clearCallFrame(ExecutionContext& context) {
    CallFrame& frame = getCallFrame(context);
    
    for (VariableEntry& varEntry : frame.localVarTable) clearValue(context, varEntry.value);

    while (getLastValueIndex(context) - frame.returnStackAddress) checkClearTemporary(context, popValue(context));
}

//-----------------------internal-----------------------

void buildGlobals(ExecutionContext& context) {
    if (checkException(context)) return;

    for (Index i = 0; i < context.globalVarPool.size(); i++) {
        MetaVariableEntry& varMetaEntry = context.globalVarPool[i];

        VariableEntry varEntry;
        varEntry.variableMeta = i;
        varEntry.value = getFreeValueIndex(context);

        context.globalVarTable.push_back(varEntry);

        buildValue(context, varEntry.value, varMetaEntry.variableType, true);
    }
}

void buildLocals(ExecutionContext& context) {
    if (checkException(context)) return;

    CallFrame& frame = getCallFrame(context);
    MetaFunctionEntry& funcMetaEntry = getFunctionMetaEntry(context, frame.functionMeta);

    for (Index i = 0; i < funcMetaEntry.localVarPool.size(); i++) {
        MetaVariableEntry& varMetaEntry = funcMetaEntry.localVarPool[i];

        VariableEntry varEntry;
        varEntry.variableMeta = i;
        varEntry.value = getFreeValueIndex(context);

        frame.localVarTable.push_back(varEntry);

        buildValue(context, varEntry.value, varMetaEntry.variableType, true);
    }
}

void pushValue(ExecutionContext& context, Index valueIndex) {
    context.valueStack.push_back(valueIndex);
}

Index popValue(ExecutionContext& context) {
    Index ret = context.valueStack.back();
    context.valueStack.pop_back();

    return ret;
}

Index getConstantValue(ExecutionContext& context, Index constIndex) {
    MetaConstantEntry& constEntry = getConstantMetaEntry(context, constIndex);

    Index freeValIndex = getFreeValueIndex(context);

    ValueEntry& valEntryNew = createValueEntry(context, freeValIndex);
    valEntryNew.value = constEntry.value;
    valEntryNew.refIndex = invalidIndex;
    valEntryNew.bound = false;
    valEntryNew.initialized = true;

    return freeValIndex;
}

Index getLocalVariableValue(ExecutionContext& context, Index varIndex) {
    return getLocalVariableEntry(context, varIndex).value;
}

Index getGlobalVariableValue(ExecutionContext& context, Index varIndex) {
    return getGlobalVariableEntry(context, varIndex).value;
}

Index getArrayElementValue(ExecutionContext& context, Index arrayIndex, Index elemIndex) {
    return getArrayEntry(context, arrayIndex).valueArray[elemIndex];
}

Index getStructMemberValue(ExecutionContext& context, Index structIndex, Index memberIndex) {
    return getStructEntry(context, structIndex).memberTable[memberIndex];
}

Index makeReference(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);

    if (!valEntry.bound) {
        throwExceptionType(context, invalidIndex);

        return invalidIndex;
    }

    if (valEntry.refIndex != invalidIndex) getReferenceEntry(context, valEntry.refIndex).count++;

    else {
        valEntry.refIndex = getFreeReferenceIndex(context);

        ReferenceEntry& refEntry = createReferenceEntry(context, valEntry.refIndex);
        refEntry.alive = true;
        refEntry.count++;
        refEntry.typeMeta = valEntry.value.type;
        refEntry.value = valueIndex;
    }

    Index freeValIndex = getFreeValueIndex(context);

    ValueEntry& valEntryNew = createValueEntry(context, freeValIndex);
    valEntryNew.value.type = findType(context, ValueType::REFERECNE, valEntry.value.type);
    valEntryNew.value.value = valEntry.refIndex;
    valEntryNew.refIndex = invalidIndex;
    valEntryNew.bound = false;
    valEntryNew.initialized = true;

    return freeValIndex;
}

void initReference(ExecutionContext& context, Index initValueIndex, Index refValueIndex) {
    if (checkException(context)) return;

    if (!checkReferenceType(context, initValueIndex) || !checkReferenceType(context, refValueIndex)) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    ValueEntry& initValEntry = getValueEntry(context, initValueIndex);
    ValueEntry& refValEntry = getValueEntry(context, refValueIndex);

    if (initValEntry.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    initValEntry.value = refValEntry.value;
    initValEntry.initialized = true;

    getReferenceEntry(context, std::get<Index>(refValEntry.value.value)).count++;
}

Index dereference(ExecutionContext& context, Index valueIndex) {
    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    MetaTypeEntry& typeEntry = getTypeMeta(context, valEntry.value.type);

    if (typeEntry.typeBinding != ValueType::REFERECNE) {
        throwExceptionType(context, invalidIndex);

        return invalidIndex;
    }

    ReferenceEntry& refEntry = getReferenceEntry(context, std::get<Index>(valEntry.value.value));

    if (!refEntry.alive) {
        throwExceptionType(context, invalidIndex);

        return invalidIndex;
    }

    return refEntry.value;
}

Index makeLambda(ExecutionContext& context, Index functionMetaIndex, Index captureCount) {
    Index newLambdaIndex = getFreeLambdaIndex(context);
    Index newValueIndex = getFreeValueIndex(context);

    LambdaEntry& lambdaEntry = createLambdaEntry(context, newLambdaIndex);
    lambdaEntry.functionMeta = functionMetaIndex;

    for (Index i = 0; i < captureCount; i++) {
        Index capValueIndex = popValue(context);
        Index newCapValueIndex = getFreeValueIndex(context);

        ValueEntry& capValEntry = createValueEntry(context, capValueIndex);

        ValueEntry& newCapValEntry = createValueEntry(context, newCapValueIndex);
        newCapValEntry.value.type = capValEntry.value.type;
        newCapValEntry.refIndex = invalidIndex;
        newCapValEntry.bound = false;

        storeValue(context, newCapValueIndex, capValueIndex);

        lambdaEntry.capturedValues.push_back(newCapValueIndex);

        checkClearTemporary(context, capValueIndex);
    }

    ValueEntry& newValEntry = createValueEntry(context, newValueIndex);
    newValEntry.value.type = findType(context, ValueType::LAMBDA, invalidIndex);
    newValEntry.value.value = newLambdaIndex;
    newValEntry.refIndex = invalidIndex;
    newValEntry.bound = false;
    newValEntry.initialized = true;

    return newValueIndex;
}

void initLambda(ExecutionContext& context, Index initValueIndex, Index lambdaValueIndex) {
    if (checkException(context)) return;

    if (!checkLambdaType(context, initValueIndex) || !checkLambdaType(context, lambdaValueIndex)) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    ValueEntry& initValEntry = getValueEntry(context, initValueIndex);
    ValueEntry& lambdaValEntry = getValueEntry(context, lambdaValueIndex);

    if (initValEntry.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    initValEntry.value = lambdaValEntry.value;
    initValEntry.initialized = true;
}

void storeValue(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc) {
    if (checkException(context)) return;

    ValueEntry& valDstEntry = getValueEntry(context, valueIndexDst);
    ValueEntry& valSrcEntry = getValueEntry(context, valueIndexSrc);

    if (!valSrcEntry.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    if (checkLambdaType(context, valueIndexDst) || checkLambdaType(context, valueIndexSrc)) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return;
    }

    else if (valDstEntry.value.type == valSrcEntry.value.type) {
        copyValue(context, valueIndexDst, valueIndexSrc);

        valDstEntry.initialized = true;
    }
    
    else if (checkCastAllowed(context, valueIndexDst, valueIndexSrc)) {
        castStore(context, valueIndexDst, valueIndexSrc);

        valDstEntry.initialized = true;
    }

    else if (checkReferenceType(context, valueIndexDst)) 
        storeValue(context, dereference(context, valueIndexDst), valueIndexSrc);

    else if (checkReferenceType(context, valueIndexSrc))
        storeValue(context, valueIndexDst, dereference(context, valueIndexSrc));

    else throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення
}

void clearValue(ExecutionContext& context, Index valueIndex) {
    if (checkException(context)) return;

    ClearList clearList = generateClearList(context, valueIndex);
    optimizeClearList(context, clearList);
    executeClearList(context, clearList);
}

void checkClearTemporary(ExecutionContext& context, Index valueIndex) {
    if (checkException(context)) return;

    ValueEntry& valueEntry = getValueEntry(context, valueIndex);

    if (!valueEntry.bound) clearValue(context, valueIndex);
}

void setInstructionPointer(ExecutionContext& context, Index position) {
    context.instructionPointerNext = position;
}

void updateInstructionPointer(ExecutionContext& context) {
    context.instructionPointer = context.instructionPointerNext;
    context.instructionPointerNext++;
}

Index getInstructionPointer(ExecutionContext& context) {
    return context.instructionPointerNext;
}

Index getInstructionPointerCurrent(ExecutionContext& context) {
    return context.instructionPointer;
}

bool checkCondition(ExecutionContext& context, Index valueIndex) {
    if (!checkPrimitiveType(context, valueIndex)) {
        throwExceptionType(context, invalidIndex);

        return false;
    }

    ValueEntry& valEntry = getValueEntry(context, valueIndex);

    return castTo<bool>(valEntry.value);
}


Index compareValues(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator) {
    ValueEntry& valEntry1 = getValueEntry(context, valueIndex1);
    ValueEntry& valEntry2 = getValueEntry(context, valueIndex2);

    if (!valEntry1.initialized || !valEntry2.initialized) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return invalidIndex;
    }

    if (checkLambdaType(context, valueIndex1) || checkLambdaType(context, valueIndex2)) {
        throwExceptionType(context, invalidIndex); // invalidIndex замінити на тип виключення

        return invalidIndex;
    }

    if (checkReferenceType(context, valueIndex1)) {
        return compareValues(context, dereference(context, valueIndex1), valueIndex2, compareOperator);
    }

    if (checkReferenceType(context, valueIndex2)) {
        return compareValues(context, valueIndex1, dereference(context, valueIndex2), compareOperator);
    }

    if (checkPrimitiveType(context, valueIndex1) && checkPrimitiveType(context, valueIndex2))
        return comparePrimitives(context, valueIndex1, valueIndex2, compareOperator);
    

    if (checkBuildableType(context, valueIndex1) && checkBuildableType(context, valueIndex2))
        return compareBuildable(context, valueIndex1, valueIndex2, compareOperator);

    throwExceptionType(context, invalidIndex);

    return invalidIndex;
}

Index comparePrimitives(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator) {
    ValueEntry& valEntry1 = getValueEntry(context, valueIndex1);
    ValueEntry& valEntry2 = getValueEntry(context, valueIndex2);

    Index freeValIndex = getFreeValueIndex(context);

    ValueEntry& valEntryNew = createValueEntry(context, freeValIndex);
    valEntryNew.value.type = findType(context, ValueType::BOOL, invalidIndex);
    valEntryNew.value.value = compareValuesVariant(valEntry1.value, valEntry2.value, compareOperator);
    valEntryNew.refIndex = invalidIndex;
    valEntryNew.bound = false;
    valEntryNew.initialized = true;
    
    return freeValIndex;
}

Index compareBuildable(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator) {
    ValueEntry& valEntry1 = getValueEntry(context, valueIndex1);
    ValueEntry& valEntry2 = getValueEntry(context, valueIndex2);

    Index freeValIndex = getFreeValueIndex(context);

    ValueEntry& valEntryNew = createValueEntry(context, freeValIndex);
    valEntryNew.value.type = findType(context, ValueType::BOOL, invalidIndex);
    valEntryNew.refIndex = invalidIndex;
    valEntryNew.bound = false;
    valEntryNew.initialized = true;

    if (static_cast<OperatorCategory>(compareOperator) != OperatorCategory::EQUALS) {
        valEntryNew.value.value = false;

        return freeValIndex;
    }

    MetaTypeEntry& typeEntry1 = getTypeMeta(context, valEntry1.value.type);
    MetaTypeEntry& typeEntry2 = getTypeMeta(context, valEntry2.value.type);

    if (typeEntry1.typeBinding != typeEntry2.typeBinding) {
        throwExceptionType(context, invalidIndex);

        return invalidIndex;
    }

    Index indexObj1 = std::get<Index>(valEntry1.value.value);
    Index indexObj2 = std::get<Index>(valEntry2.value.value);

    switch (typeEntry1.typeBinding) {
        case ValueType::ARRAY: {
            ArrayEntry& arrayEntry1 = getArrayEntry(context, indexObj1);
            ArrayEntry& arrayEntry2 = getArrayEntry(context, indexObj2);

            if (arrayEntry1.valueArray.size() != arrayEntry2.valueArray.size()) valEntryNew.value.value = false;

            else {
                valEntryNew.value.value = true;

                for (Index i = 0; i < arrayEntry1.valueArray.size(); i++) {
                    if (!compareValues(context, arrayEntry1.valueArray[i], arrayEntry2.valueArray[i], compareOperator)) {
                        valEntryNew.value.value = false;

                        break;
                    }
                }
            }

            break;
        }

        case ValueType::STRUCT: {
            StructEntry& structEntry1 = getStructEntry(context, indexObj1);
            StructEntry& structEntry2 = getStructEntry(context, indexObj2);

            if (structEntry1.structMeta != structEntry2.structMeta) valEntryNew.value.value = false;

            else {
                valEntryNew.value.value = true;

                for (Index i = 0; i < structEntry1.memberTable.size(); i++) {
                    if (!compareValues(context, structEntry1.memberTable[i], structEntry2.memberTable[i], compareOperator)) {
                        valEntryNew.value.value = false;

                        break;
                    }
                }
            }

            break;
        }
    }

    return freeValIndex;
}

void callFunction(ExecutionContext& context, Index functionIndex, Size argCount) {
    if (checkException(context)) return;    

    MetaFunctionEntry& funcMetaEntry = getFunctionMetaEntry(context, functionIndex);

    CallFrame& frame = pushCallFrame(context);
    frame.functionMeta = functionIndex;
    frame.returnAddress = getInstructionPointer(context);
    frame.returnStackAddress = getLastValueIndex(context) - argCount;

    setInstructionPointer(context, funcMetaEntry.codeStart);

    buildLocals(context);
}

void callLambdaFunction(ExecutionContext& context, Index valueIndex, Size argCount) {
    if (checkException(context)) return;

    ValueEntry& valEntry = getValueEntry(context, valueIndex);
    LambdaEntry& lambdaEntry = getLambdaEntry(context, std::get<Index>(valEntry.value.value));
    MetaFunctionEntry& funcMetaEntry = getFunctionMetaEntry(context, lambdaEntry.functionMeta);

    for (Index valueIndex : lambdaEntry.capturedValues) pushValue(context, valueIndex);

    CallFrame& frame = pushCallFrame(context);
    frame.functionMeta = lambdaEntry.functionMeta;
    frame.returnAddress = getInstructionPointer(context);
    frame.returnStackAddress = getLastValueIndex(context) - argCount - lambdaEntry.capturedValues.size();

    setInstructionPointer(context, funcMetaEntry.codeStart);

    buildLocals(context);
}

void returnFunction(ExecutionContext& context) {
    clearCallFrame(context);
    popCallFrame(context);
}

void throwExceptionType(ExecutionContext& context, Index typeIndex)  {
    if (checkException(context)) return;

    if (!context.callStack.size()) exitException(context);

    // getCallFrame(context).
    getCallFrame(context).exception.active = true;
}

void throwExceptionValue(ExecutionContext& context, Index valueIndex) {
    if (checkException(context)) return;
    
    if (!context.callStack.size()) exitException(context);

    getCallFrame(context).exception.active = true;
}

bool checkException(ExecutionContext& context) {
    if (!context.callStack.size()) return false;

    return getCallFrame(context).exception.active;
}

void catchException(ExecutionContext& context) {
    Exception exception = getCallFrame(context).exception;

    while (context.callStack.size()) {
        CallFrame& frame = getCallFrame(context);
        MetaFunctionEntry& funcEntry = getFunctionMetaEntry(context, frame.functionMeta);

        for (auto tryBlock : funcEntry.tryBlockPool) {
            if (checkInBlock(tryBlock.codeStart, tryBlock.codeEnd, getInstructionPointer(context))) {
                
            }
        }
    }
}

void exitException(ExecutionContext& context) {
    // TODO: replace with error message print
    std::exit(1);
}