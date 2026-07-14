#pragma once
#include <core/types.hpp>
#include <core/execontext.hpp>
#include <core/nptypes/value.hpp>
#include <core/nptypes/variable.hpp>
#include <core/nptypes/reference.hpp>
#include <core/nptypes/array.hpp>
#include <core/nptypes/struct.hpp>
#include <meta/pools/type.hpp>
#include <meta/pools/constant.hpp>
#include <meta/pools/variable.hpp>
#include <meta/pools/function.hpp>
#include <meta/pools/struct.hpp>

//-----------------internal-structures-----------------
enum class SpecialAction {
    NONE,
    DELETE,
    SETFLAG,
    REDUCE
};

struct ClearEntry {
    ValueType type;
    SpecialAction action;
    Index index;
    uint16 count;
};

using ClearList = std::vector<ClearEntry>;
//-----------------internal-structures-----------------

//-----------------------internal-----------------------
Index getCallFrameIndex(ExecutionContext& context);
Index getStackAddress(ExecutionContext& context);

MetaTypeEntry& getTypeMeta(ExecutionContext& context, Index typeIndex);
MetaConstantEntry& getConstantMetaEntry(ExecutionContext& context, Index constIndex);
MetaVariableEntry& getLocalVariableMetaEntry(ExecutionContext& context, Index varIndex);
MetaVariableEntry& getGlobalVariableMetaEntry(ExecutionContext& context, Index varIndex);
MetaFunctionEntry& getFunctionMetaEntry(ExecutionContext& context, Index funcIndex);
MetaArrayEntry& getArrayMetaEntry(ExecutionContext& context, Index arrayIndex);
MetaStructEntry& getStructMetaEntry(ExecutionContext& context, Index structIndex);

VariableEntry& getLocalVariableEntry(ExecutionContext& context, Index varIndex);
VariableEntry& getGlobalVariableEntry(ExecutionContext& context, Index varIndex);

ValueEntry& getValueEntry(ExecutionContext& context, Index valueIndex);
ValueEntry& createValueEntry(ExecutionContext& context, Index valueIndex);

ReferenceEntry& getReferenceEntry(ExecutionContext& context, Index refIndex);
ReferenceEntry& createReferenceEntry(ExecutionContext& context, Index refIndex);

LambdaEntry& getLambdaEntry(ExecutionContext& context, Index lambdaIndex);
LambdaEntry& createLambdaEntry(ExecutionContext& context, Index lambdaIndex);

ArrayEntry& getArrayEntry(ExecutionContext& context, Index arrayIndex);
ArrayEntry& createArrayEntry(ExecutionContext& context, Index arrayIndex);

StructEntry& getStructEntry(ExecutionContext& context, Index structIndex);
StructEntry& createStructEntry(ExecutionContext& context, Index structIndex);

bool checkLocalVariableEntry(ExecutionContext& context, Index varIndex);
bool checkGlobalVariableEntry(ExecutionContext& context, Index varIndex);
bool checkValueEntry(ExecutionContext& context, Index valueIndex);
bool checkReferenceEntry(ExecutionContext& context, Index refIndex);
bool checkLambdaEntry(ExecutionContext& context, Index lambdaIndex);
bool checkArrayEntry(ExecutionContext& context, Index arrayIndex);
bool checkStructEntry(ExecutionContext& context, Index structIndex);

Index getFreeValueIndex(ExecutionContext& context);
Index getFreeReferenceIndex(ExecutionContext& context);
Index getFreeLambdaIndex(ExecutionContext& context);
Index getFreeArrayIndex(ExecutionContext& context);
Index getFreeStructIndex(ExecutionContext& context);

Index findType(ExecutionContext& context, ValueType type, Index subtype);

void buildValue(ExecutionContext& context, Index valueIndex, Index typeIndex, bool bound);
void buildArray(ExecutionContext& context, Index arrayIndex, Index arrayMetaIndex);
void buildStruct(ExecutionContext& context, Index structIndex, Index structMetaIndex);

bool checkPrimitiveType(ExecutionContext& context, Index valueIndex);
bool checkReferenceType(ExecutionContext& context, Index valueIndex);
bool checkBuildableType(ExecutionContext& context, Index valueIndex);
bool checkCastAllowed(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc);

void castStore(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc);
void copyValue(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc);

ClearList generateClearList(ExecutionContext& context, Index valueIndex);
void optimizeClearList(ExecutionContext& context, ClearList& clearList);
void executeClearList(ExecutionContext& context, const ClearList& clearList);

CallFrame& getCallFrame(ExecutionContext& context);
CallFrame& pushCallFrame(ExecutionContext& context);
void popCallFrame(ExecutionContext& context);
void clearCallFrame(ExecutionContext& context);
//-----------------------internal-----------------------

void buildGlobals(ExecutionContext& context);
void buildLocals(ExecutionContext& context);

void pushValue(ExecutionContext& context, Index valueIndex);

Index popValue(ExecutionContext& context);

Index getConstantValue(ExecutionContext& context, Index constIndex);
Index getLocalVariableValue(ExecutionContext& context, Index varIndex);
Index getGlobalVariableValue(ExecutionContext& context, Index varIndex);
Index getArrayElementValue(ExecutionContext& context, Index arrayIndex, Index elemIndex);
Index getStructMemberValue(ExecutionContext& context, Index structIndex, Index memberIndex);

Index makeReference(ExecutionContext& context, Index valueIndex);
void initReference(ExecutionContext& context, Index initValueIndex, Index refValueIndex);
Index dereference(ExecutionContext& context, Index valueIndex);

Index makeLambda(ExecutionContext& context, Index functionMetaIndex, Index captureCount);
void initLambda(ExecutionContext& context, Index initValueIndex, Index lambdaValueIndex);

void storeValue(ExecutionContext& context, Index valueIndexDst, Index valueIndexSrc);

void clearValue(ExecutionContext& context, Index valueIndex);
void checkClearTemporary(ExecutionContext& context, Index valueIndex);

void setInstructionPointer(ExecutionContext& context, Index position);
void updateInstructionPointer(ExecutionContext& context);

Index getInstructionPointer(ExecutionContext& context);
Index getInstructionPointerCurrent(ExecutionContext& context);

bool checkCondition(ExecutionContext& context, Index valueIndex);

Index compareValues(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator);
Index comparePrimitives(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator);
Index compareBuildable(ExecutionContext& context, Index valueIndex1, Index valueIndex2, Index compareOperator);

void callFunction(ExecutionContext& context, Index functionIndex, Size argCount);
void callLambdaFunction(ExecutionContext& context, Index valueIndex, Size argCount);
void returnFunction(ExecutionContext& context);

void throwExceptionType(ExecutionContext& context, Index typeIndex);
void throwExceptionValue(ExecutionContext& context, Index valueIndex);
bool checkException(ExecutionContext& context);
void catchException(ExecutionContext& context);
void exitException(ExecutionContext& context);