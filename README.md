# Regex Engine Project

This is a simple regex engine project implemented in C++ that allows you to perform pattern matching using regular expressions. The project includes a Makefile for easy compilation.

## Features

- **Regex Matching:** The engine can match strings against regular expressions.
- **Basic Syntax:** Supports basic regular expression syntax such as `*`, `+`, `?`, '|', `()`.
- **Makefile:** Includes a Makefile for easy compilation and building of the project.

## Getting Started

### Prerequisites

- Make sure you have the GNU C++ Compiler (g++) version 13 installed on your system. You can install it using your package manager or by downloading it from the official GNU website.
- Ensure that `make` is installed.

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/Moidhas/regex-engine.git
    ```

2. Navigate to the project directory:

    ```bash
    cd regex-engine
    ```

3. Compile the project using the Makefile:

    ```bash
    make
    ```

## Usage

Once the project is compiled, you can use the regex engine by running the executable. Here's a simple example:

```bash
./regrep "pattern" "input_string"
```

### Examples

```bash
./regrep "a*b" "aaab"   # match
./regrep "a+b" "aaab"   # match
./regrep "a?b" "ab"     # match
./regrep "(a|b)*c" "abaabaabbaac" # match
```
