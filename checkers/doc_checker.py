#!/bin/python3

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
    doc_start: str = r";{2,}"
    doc_end: str = r";{2,}"
    func: str = r"\w+:"


class CPattern:
    doc_start: str = r"/\*"
    doc_end: str = r" \* \*/"
    func: str = r"(?!#)\w+[\s\*]+\w+\s*\(.*"


def read_comment_block(filepath: str, file_iter: Iterator[str],
                       i: int, doc_end: str) -> tuple[str, int]:
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
    raise ValueError(f"{filepath}:{i}> Unclosed comment block")


def skip_header(file_iter: Iterator[str]) -> int:
    """Skip the first header comment block"""
    i: int = 0
    for line in file_iter:
        i += 1
        if line.rstrip() == "":
            return i
    return i


MAX_LINES_AFTER_DOC: int = 3


def max_lines_after_doc(filepath: str, line_no: int, file_iter: Iterator[str],
                        pattern: LangPattern) -> int:
    """
    Check if there's a function declaration at maximum
    MAX_LINES_AFTER_DOC lines below the doc block
    """
    i: int = 0
    for _ in range(MAX_LINES_AFTER_DOC):
        try:
            line = next(file_iter).rstrip()
        except StopIteration:
            break
        i += 1
        if re.fullmatch(pattern.func, line):
            return i
    print(f"{filepath}:{line_no + i}: No function after documentation")
    return i


def check_func_doc(filepath: str, pattern: LangPattern) -> None:
    """
    Check if every function in the file
    has proper documentation above it
    """
    with open(filepath, "r") as file:
        file_iter: Iterator[str] = iter(file)
        i = skip_header(file_iter)
        for line in file_iter:
            line = line.rstrip()
            i += 1
            if re.fullmatch(pattern.func, line):
                print(
                    f"{filepath}:{i}> Function/label without documentation"
                )
                continue
            if not re.fullmatch(pattern.doc_start, line):
                continue
            doc_idx: int = i
            content, i = read_comment_block(
                filepath, file_iter, i, pattern.doc_end
            )
            if not re.search(r"\w+\s+-\s+\S.*", content):
                print(
                    f"{filepath}:{doc_idx}> Missing 'funcname - description'"
                )
                continue
            if not re.search(r"DESCRIPTION", content):
                print(
                    f"{filepath}:{doc_idx}> Missing DESCRIPTION section"
                )
                continue
            if not re.search(r"RETURN VALUE", content):
                print(
                    f"{filepath}:{doc_idx}> Missing RETURN VALUE section"
                )
                continue
            i += max_lines_after_doc(filepath, i, file_iter, pattern)


def check_file_doc_func(file: str, root: Optional[str] = "") -> None:
    """
    Check if a file contain any documentation missing.
    Support to Assembly and c files
    """
    extension: str = os.path.splitext(file)[1]
    file = os.path.join(root, file)
    if extension == ".c" or extension == ".h":
        check_func_doc(file, CPattern)
    elif extension == ".asm":
        check_func_doc(file, AsmPattern)


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
