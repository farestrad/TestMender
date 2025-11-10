# 🧩 Juliet CWE Mapping for TestMender / SafeScript

This document lists the selected **Juliet Test Suite CWEs** currently used for static analysis and Semgrep evaluation.  
Each CWE corresponds to a folder in the dataset and a category of vulnerabilities tested by the analysis pipeline.

---

## ✅ Active CWEs

## total is 8 CWE, for each pick 15 .c files then for each c file do 10 test case (8 * 15 * 10 = 1200) 

| **Folder Name** | **CWE ID** | **Title** | **Category** | **Notes** |
|-----------------|------------|------------|---------------|------------|
| `CWE121_...` | **CWE-121** | Stack-Based Buffer Overflow | Memory Safety | Classic stack overflow cases. |
| `CWE122_...` | **CWE-122** | Heap-Based Buffer Overflow | Memory Safety | Heap overflow, complements CWE-121. |
| `CWE190_...` | **CWE-190** | Integer Overflow or Wraparound | Arithmetic Errors | Covers signed/unsigned overflow. |
| `CWE364_...` | **CWE-364** | Signal Handler Race Condition | Concurrency | Race condition within signal handlers. |
(result 600 test cases)



| `CWE476_...` | **CWE-476** | NULL Pointer Dereference | Memory Safety | Common dereference vulnerability. |
| `CWE835_...` | **CWE-835** | Infinite Loop / Uncontrolled Resource Consumption | Control Flow | Infinite loop and CPU/memory exhaustion. |
| `CWE546_...` | **CWE-546** | 
| `CWE665_...` | **CWE-665** | 
| `CWE23_...` | **CWE-23** | Relative Path Traversal | Input Validation | Unvalidated relative path sequences (`../`). |
| `CWE36_...` | **CWE-36** | Absolute Path Traversal | Input Validation | Full path misuse (e.g., `/etc/passwd`). |
(result 600 test cases)

---

## 📘 Summary

- These eight CWEs are **confirmed present in the official Juliet C/C++ dataset**.
- Together, they provide balanced coverage across:
  - **Memory Safety** (CWE-121, 122, 476)
  - **Arithmetic Errors** (CWE-190)
  - **Concurrency** (CWE-364)
  - **Control Flow / Resource Management** (CWE-835)
  - **Input Validation / Path Manipulation** (CWE-23, 36)
- Ideal for Semgrep, static, and hybrid analysis pipelines within **TestMender** or **SafeScript**.

---

**Last Updated:** November 2025  
**Maintainer:** Fares Trad  
**Project:** TestMender / SafeScript Security Analysis
