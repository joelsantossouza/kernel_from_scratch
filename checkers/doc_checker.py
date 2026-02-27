# File: test.py
# Author: Joel Souza
# Date: 2026-02-27
# Description: Checker to validate C, Assembly and python headers
#              and function documentation

from typing import TextIO, Optional, Callable, Iterator
import re


class HeaderPattern:
    """Contain all HeaderPatterns (x86 Assembly, python and C)"""
    python: list[str] = [
        r"# File:\s+\w+\.py",
        r"# Author:\s+[A-Za-z]+ [A-Za-z]+",
        r"# Date:\s+\d{4}-\d{2}-\d{2}",
        r"# Description:\s+\S.*",
    ]
    python_example: list[str] = [
        "# File: test.py",
        "# Author: Joel Souza",
        "# Date: 2026-02-27",
        "# Description: Testing header checker",
    ]

    c: list[str] = [
        r"/\*+",
        r"\s*\*\s+File:\s+\w+\.[ch]",
        r"\s*\*\s+Author:\s+[A-Za-z]+ [A-Za-z]+",
        r"\s*\*\s+Date:\s+\d{4}-\d{2}-\d{2}",
        r"\s*\*\s+Description:\s+\S.*",
        r"\s*\*+/",
    ]
    c_example: list[str] = [
        "/**",
        " * File: main.c",
        " * Author: Joel Souza",
        " * Date: 2026-02-27",
        " * Description: Testing header checker",
        " */",
    ]

    asm: list[str] = [
        r";\s+File:\s+\w+\.asm",
        r";\s+Author:\s+[A-Za-z]+ [A-Za-z]+",
        r";\s+Date:\s+\d{4}-\d{2}-\d{2}",
        r";\s+Description:\s+\S.*",
    ]
    asm_example: list[str] = [
        "; File: main.asm",
        "; Author: Joel Souza",
        "; Date: 2026-02-27",
        "; Description: Testing header checker",
    ]


def check_header(file: TextIO, pattern: list[str],
                 example: Optional[list[str]] = None) -> None:
    """Check if the file contain a valid header pattern"""
    is_valid_header: bool = True
    for i, pattern_line in enumerate(pattern):
        if not re.fullmatch(pattern_line, file.readline().rstrip()):
            is_valid_header = False
            break
    if is_valid_header is True:
        return
    error_message: str = f"Invalid Header in line {i}!"
    if example:
        error_message += "\nExample:\n"
        for example_line in example:
            error_message += f"{example_line}\n"
    raise ValueError(error_message)


class FuncDocPattern:
    """
    Describes function documentation pattern
    and function declarations in Assembly and C
    with regex
    """
    c_doc: list[str] = [

    ]
    c_function: str = r"(?!#)\w+\s+\w+\s*\(.*\)"


def check_func_doc(file: TextIO, pattern: list[str], func_pattern: str,
                   example: Optional[list[str]] = None) -> None:
    """
    Check if Assembly and C functions
    contain proper documentation
    """
    file_iter: Iterator = iter(file)
    i: int = -1
    for line in file_iter:
        i += 1
        if is_function(line):
            raise ValueError("Line {i}: Function without documentation.")
        if not re.fullmatch(pattern[0], line):
            continue
        for pat_line in pattern:
            if not re.fullmatch(pat_line, line):
                raise ValueError(f"Line {i}: Doc missing {pat_line}")
            line = next(line)
            i += 1
        line = next(line)
        i += 1
        if not is_function(line):
            raise ValueError(f"Line {i}: No function just after documentation")


with open("./doc_checker.py", "r") as file:
    check_header(file, HeaderPattern.python, HeaderPattern.python_example)
