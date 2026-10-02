# ViraVM

> Experiment in building a programming-language runtime from scratch.

ViraVM is an experimental **language-independent runtime and virtual machine written in C++**.

The idea is to provide a common execution environment that programming languages can target instead of implementing their own runtime from scratch.

The project is primarily focused on exploring how a programming-language runtime can be designed and built from the ground up.

> **Status:** Experimental / Work in Progress
> **VM version:** `0.1`
> **Bytecode version:** `0.1`

---

## 💡 The idea

ViraVM is built around a **table-based runtime**.

Runtime entities such as values, types, variables, functions, arrays and structures are stored in dedicated tables and referenced through indices.

This gives the runtime a common representation of program state and allows the virtual machine to operate on that state without being tied to a particular source language.

In simplified form:

```text
             Programming Language
                      │
                      ▼
                   Compiler
                      │
                      ▼
              ViraVM Bytecode
                      │
                      ▼
             ┌─────────────────┐
             │ ViraVM Runtime  │
             │                 │
             │ Runtime Tables  │
             │       +         │
             │       VM        │
             └────────┬────────┘
                      │
                      ▼
                  Execution
```

The goal is for a language to only need to translate its own concepts into ViraVM's runtime model and bytecode.

---

## ✨ What exists

The current runtime already has the basic building blocks required for executing programs:

* custom bytecode instruction set;
* stack-based virtual machine;
* runtime type system;
* primitive values;
* constants and variables;
* arrays and structures;
* references;
* functions and lambdas;
* function calls;
* basic control flow;
* runtime lifetime management;
* binary bytecode format;
* metadata loading;
* initial exception-handling infrastructure.

The runtime can currently be exercised directly from C++ while the surrounding toolchain is still being developed.

---

## 🚧 What is missing

ViraVM is **not yet a complete standalone runtime**.

The main unfinished parts are:

* complete bytecode instruction loading;
* executing programs loaded directly from bytecode files;
* complete exception unwinding;
* a compiler/code generator targeting ViraVM;
* stabilization of the bytecode format and ISA;
* a proper user-facing runtime/toolchain.

These are part of the ongoing development of the project.

---

## 🏗️ Current architecture

At the moment, ViraVM can be viewed as three major parts:

```text
┌──────────────────────┐
│    Bytecode Format   │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│    Runtime Tables    │
│                      │
│ Values               │
│ Types                │
│ Variables            │
│ Functions            │
│ Arrays               │
│ Structures           │
│ References           │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│     Virtual Machine  │
└──────────────────────┘
```

The tables form the persistent runtime state, while the VM provides the mechanism for executing bytecode against that state.

---

## 🧪 Current development stage

ViraVM is currently in the **runtime development stage**.

The VM and its runtime model are being built before a complete compiler and standalone bytecode toolchain.

For development and testing, programs can currently be constructed directly through the C++ API and executed by the VM.

The long-term execution path is:

```text
Source Code
     │
     ▼
Compiler
     │
     ▼
ViraVM Bytecode
     │
     ▼
ViraVM Runtime
     │
     ▼
Program Execution
```

---

## 🎯 Goals

ViraVM aims to become a reusable runtime for programming languages that target its bytecode.

The project focuses on:

* a language-independent execution environment;
* a well-defined runtime model;
* compact representation of runtime state;
* a relatively small virtual machine;
* an extensible bytecode format;
* experimentation with programming-language runtime design.

ViraVM is not intended to be tied to one specific programming language.

---

## 🔨 Building

ViraVM uses CMake.

```bash
git clone https://github.com/Demowinter/viravm.git
cd viravm

cmake -S . -B build
cmake --build build
```

The current development executable is:

```bash
./build/viravm-devel
```

At this stage it is primarily used for testing the runtime and VM rather than running arbitrary compiled programs.

---

## 📜 License

See the repository for the current license information.

---
