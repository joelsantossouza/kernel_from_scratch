# Code Design Checkers

A set of static analysis tools to enforce code style and documentation
standards across C and Assembly files.

## Tools

### header_checker.py
Validates that every C and Assembly file contains a proper file header.

## Options

| Flag     | Description                                      |
|----------|--------------------------------------------------|
| `--fill` | Auto-fill missing headers with a standard template |

**Required fields:**
- `File`        — must match the actual filename
- `Author`      — first and last name
- `Date`        — creation date in YYYY-MM-DD format
- `Description` — short description of the file's purpose

**C example:**
    /*
     * File: main.c
     * Author: Joel Souza
     * Date: 2026-02-27
     * Description: Entry point of the kernel
     */

**Assembly example:**
    ; File: boot.asm
    ; Author: Joel Souza
    ; Date: 2026-02-27
    ; Description: Bootloader entry point

---

### doc_checker.py
Validates that every C and Assembly function has proper documentation
directly above its declaration.

**Required sections:**
- `funcname - short description` — one liner summary
- `DESCRIPTION`                  — detailed explanation
- `RETURN VALUE`                 — what the function returns

**Example:**
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

---

### asm_bits_checker.py
Validates that every Assembly file declares a `[BITS X]` directive,
enforcing explicit processor mode declaration.

**Accepted values:** `[BITS 16]`, `[BITS 32]`, `[BITS 64]`

---

## Usage

    python3 header_checker.py <file_or_directory>
    python3 doc_checker.py <file_or_directory>
    python3 asm_bits_checker.py <file_or_directory>

Or run all at once via the root Makefile:

    make check
