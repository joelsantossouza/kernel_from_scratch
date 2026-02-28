# Author: Joel Souza
# Description: Python program to check if C and
#              Assembly files contain proper function
#              documentation

from typing import Iterator, Optional
import re
import sys
import os


class LangPattern:
    pass


class AsmPattern:
    doc_start: str = r";+"
    doc_end: str = r";+"
    func: str = r"\w+:"


class CPattern:
    doc_start: str = r"/\*"
    doc_end: str = r" \* \*/"
    func: str = r"(?!#)\w+\s+\w+\s*\(.*\)"


def read_comment_block(file_iter: Iterator[str], i: int,
                       doc_end: str) -> tuple[str, int]:
    """
    Read all lines between doc_start and doc_end
    and return as a single string
    """
    block: list[str] = []
    for line in file_iter:
        line = line.rstrip()
        i += 1
        if re.fullmatch(doc_end, line):
            return "\n".join(block), i
        block.append(line)
    raise ValueError(f"Line {i}: Unclosed comment block")


def check_func_doc(filepath: str, pattern: LangPattern) -> None:
    """
    Check if every function in the file
    has proper documentation above it
    """
    with open(filepath, "r") as file:
        file_iter: Iterator[str] = iter(file)
        i: int = 0
        for line in file_iter:
            line = line.rstrip()
            i += 1
            if re.fullmatch(pattern.func, line):
                raise ValueError(f"Line {i}: Function without documentation")
            if not re.fullmatch(pattern.doc_start, line):
                continue
            doc_idx: int = i
            content, i = read_comment_block(file_iter, i, pattern.doc_end)
            if not re.search(r"\w+\s+-\s+\S.*", content):
                raise ValueError(
                    f"Document {doc_idx}: Missing 'funcname - description'"
                )
            if not re.search(r"DESCRIPTION", content):
                raise ValueError(
                    f"Document {doc_idx}: Missing DESCRIPTION section"
                )
            if not re.search(r"RETURN VALUE", content):
                raise ValueError(
                    f"Document {doc_idx}: Missing RETURN VALUE section"
                )
            line = next(file_iter).rstrip()
            i += 1
            if re.fullmatch(pattern.func, line):
                continue
            line = next(file_iter).rstrip()
            i += 1
            if not re.fullmatch(pattern.func, line):
                raise ValueError(
                    f"Line {i}: No function declaration after documentation")


def check_file_doc_func(file: str, root: Optional[str] = "") -> None:
    """
    Check if a file contain any documentation missing.
    Support to Assembly and c files
    """
    try:
        extension: str = os.path.splitext(file)[1]
        file = os.path.join(root, file)
        if extension == ".c":
            check_func_doc(file, CPattern)
        elif extension == ".asm":
            check_func_doc(file, AsmPattern)
    except Exception as e:
        print(f"{file} => {e}")


if __name__ == "__main__":
    base_path: str = "./"
    if len(sys.argv) > 1:
        base_path = sys.argv[1]
    if os.path.isfile(base_path):
        check_file_doc_func(base_path)
    else:
        for root, dirs, files in os.walk(base_path):
            for file in files:
                check_file_doc_func(file, root)
