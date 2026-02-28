from typing import TextIO, Optional
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


def check_header(filepath: str, pattern: list[str],
                 example: Optional[list[str]] = None) -> None:
    """Check if the file contain a valid header pattern"""
    with open(filepath, "r") as file:
        is_valid_header: bool = True
        for i, pattern_line in enumerate(pattern, start=1):
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
