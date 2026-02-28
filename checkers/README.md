# 🔍 Code Design Checkers

> Static analysis tools to enforce code style and documentation standards
> across **C** and **Assembly** files.

---

## 📦 Tools Overview

| Tool | Description |
|------|-------------|
| [`header_checker.py`](#-header_checkerpy) | Validates file headers |
| [`doc_checker.py`](#-doc_checkerpy) | Validates function documentation |
| [`asm_bits_checker.py`](#-asm_bits_checkerpy) | Validates BITS directive in ASM files |

---

## 📄 header_checker.py

Validates that every **C** and **Assembly** file contains a proper file header
with all required fields correctly filled.

### Required fields

| Field | Rule |
|-------|------|
| `File` | Must match the actual filename |
| `Author` | First and last name |
| `Date` | Creation date in `YYYY-MM-DD` format |
| `Description` | Short description of the file's purpose |

### Examples

**C** (`.c` / `.h`)
```c
/*
 * File: main.c
 * Author: Joel Souza
 * Date: 2026-02-27
 * Description: Entry point of the kernel
 */
```

**Assembly** (`.asm`)
```asm
; File: boot.asm
; Author: Joel Souza
; Date: 2026-02-27
; Description: Bootloader entry point
```

### Usage
```bash
# Check a single file
python3 header_checker.py main.c

# Check all files in a directory recursively
python3 header_checker.py ./src

# Auto-fill missing headers with a standard template
python3 header_checker.py ./src --fill
```

### Options

| Flag | Description |
|------|-------------|
| `--fill` | Auto-fill missing headers with a standard template |

---

## 📝 doc_checker.py

Validates that every **C** and **Assembly** function has proper documentation
directly above its declaration. Documentation must contain all required
sections in the correct order.

### Required sections

| Section | Rule |
|---------|------|
| `funcname - short description` | One liner summary of what the function does |
| `DESCRIPTION` | Detailed explanation, minimum 1 line, maximum 5 lines |
| `RETURN VALUE` | What the function returns, minimum 1 line, maximum 5 lines |

### Example

```c
/*
 * strcpy - copy source string into destination string
 *
 * DESCRIPTION
 *     These functions copy the string pointed to by src
 *     into a string at the buffer pointed to by dst.
 *
 * RETURN VALUE
 *     Returns a pointer to the destination string dst.
 * */
char    *strcpy(char *dst, const char *src)
```

### Usage
```bash
# Check a single file
python3 doc_checker.py main.c

# Check all files in a directory recursively
python3 doc_checker.py ./src
```

---

## 🔧 asm_bits_checker.py

Validates that every **Assembly** file explicitly declares a `[BITS X]`
directive, enforcing processor mode declaration and preventing silent bugs
caused by the assembler defaulting to 16-bit mode.

### Accepted values

```asm
[BITS 16]   ; 16-bit real mode
[BITS 32]   ; 32-bit protected mode
[BITS 64]   ; 64-bit long mode
```

### Example

```asm
[BITS 64]

; File: boot.asm
; Author: Joel Souza
; Date: 2026-02-27
; Description: Bootloader entry point

global  _start

_start:
    mov rax, 1
```

### Usage
```bash
# Check a single file
python3 asm_bits_checker.py boot.asm

# Check all ASM files in a directory recursively
python3 asm_bits_checker.py ./src
```

---

## 🚀 Run all checkers

Via the root Makefile:

```bash
make check
```

Or manually:

```bash
python3 header_checker.py <file_or_directory>
python3 doc_checker.py <file_or_directory>
python3 asm_bits_checker.py <file_or_directory>
```
