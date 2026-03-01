#!/bin/python3

# Author: Joel Souza
# Description: Python program to check if C and
#              assembly files contain proper headers

from typing import Optional
from datetime import date
import re
import os
import sys

AUTHOR = "Joel Souza"


class LangHeader:
    pass


class PythonHeader:
    pattern: list[str] = [
        r"# File:\s+",
        r"# Author:\s+[A-Za-z]+ [A-Za-z]+",
        r"# Date:\s+\d{4}-\d{2}-\d{2}",
        r"# Description:\s+\S.*",
    ]
    example: list[str] = [
        "# File: test.py",
        "# Author: Joel Souza",
        "# Date: 2026-02-27",
        "# Description: Testing header checker",
    ]


class CHeader:
    pattern: list[str] = [
        r"/\*+",
        r"\s*\*\s+File:\s+",
        r"\s*\*\s+Author:\s+[A-Za-z]+ [A-Za-z]+",
        r"\s*\*\s+Date:\s+\d{4}-\d{2}-\d{2}",
        r"\s*\*\s+Description:\s+\S.*",
        r"\s*\*+/",
    ]
    example: list[str] = [
        "/**",
        " * File: main.c",
        " * Author: Joel Souza",
        " * Date: 2026-02-27",
        " * Description: Testing header checker",
        " */",
    ]


class AsmHeader:
    pattern: list[str] = [
        r";\s+File:\s+",
        r";\s+Author:\s+[A-Za-z]+ [A-Za-z]+",
        r";\s+Date:\s+\d{4}-\d{2}-\d{2}",
        r";\s+Description:\s+\S.*",
    ]
    example: list[str] = [
        "; File: main.asm",
        "; Author: Joel Souza",
        "; Date: 2026-02-27",
        "; Description: Testing header checker",
    ]


def check_header(filepath: str, header: LangHeader,
                 with_example: bool = False) -> None:
    """Check if the file contain a valid header pattern"""
    filename: str = os.path.basename(filepath)
    pattern: list[str] = header.pattern.copy()
    for i, p in enumerate(pattern):
        if r"File:\s+" in p:
            pattern[i] = p + re.escape(filename)
            break
    with open(filepath, "r") as file:
        is_valid_header: bool = True
        for i, pattern_line in enumerate(pattern, start=1):
            if not re.fullmatch(pattern_line, file.readline().rstrip()):
                is_valid_header = False
                break
        if is_valid_header is True:
            return
        error_message: str = f"Invalid Header in line {i}!"
        if with_example:
            example: list[str] = header.example
            error_message += "\nExample:\n"
            for example_line in example:
                error_message += f"{example_line}\n"
        raise ValueError(error_message)


def fill_file_header(filepath: str) -> None:
    """Add a standard header to the file if it doesn't have one"""
    filename = os.path.basename(filepath)
    ext = os.path.splitext(filepath)[1]
    today = date.today().strftime("%Y-%m-%d")

    headers = {
        ".py": (
            f"# File: {filename}\n"
            f"# Author: {AUTHOR}\n"
            f"# Date: {today}\n"
            f"# Description: \n\n"
        ),
        ".c": (
            f"/*\n"
            f" * File: {filename}\n"
            f" * Author: {AUTHOR}\n"
            f" * Date: {today}\n"
            f" * Description: \n"
            f" */\n\n"
        ),
        ".asm": (
            f"; File: {filename}\n"
            f"; Author: {AUTHOR}\n"
            f"; Date: {today}\n"
            f"; Description: \n\n"
        ),
    }
    headers[".h"] = headers[".c"]
    headers[".inc"] = headers[".asm"]

    header = headers.get(ext)
    if header is None:
        raise ValueError(f"Unsupported file type: {ext}")

    with open(filepath, "r") as f:
        content = f.read()

    with open(filepath, "w") as f:
        f.write(header + content)


def check_file_header(file: str, option: str,
                      root: Optional[str] = "") -> None:
    """
    Check if a file contain proper header.
    Support to python, Assembly and c files
    """
    try:
        extension: str = os.path.splitext(file)[1]
        file = os.path.join(root, file)
        if extension == ".c" or extension == ".h":
            check_header(file, CHeader)
        elif extension == ".asm" or extension == ".inc":
            check_header(file, AsmHeader)
    except Exception as e:
        if option == "--fill":
            fill_file_header(file)
        print(f"{file} => {e}")


if __name__ == "__main__":
    base_path: str = "./"
    flags: str = ""
    if len(sys.argv) > 1:
        base_path = sys.argv[1]
    if len(sys.argv) > 2:
        flags = sys.argv[2]
    if os.path.isfile(base_path):
        check_file_header(base_path, flags)
    else:
        for root, dirs, files in os.walk(base_path):
            for file in files:
                check_file_header(file, flags, root)
