# Minishell

<p align="center">
  <img src="https://github.com/ayogun/42-project-badges/raw/main/badges/minishelle.png" alt="Minishell Achievement" width="200" height="200">
  <img src="https://res.cloudinary.com/dfjub9qt4/image/upload/v1712254907/push_swap_42545.png" alt="Minishell Square" width="200" height="200">
</p>

Minishell is a simplified Unix shell implementation written in C, designed to mimic basic functionalities of popular Unix shells like Bash.

## Introduction

Minishell is a command-line interpreter that allows users to execute commands, navigate directories, and manage processes within a Unix-like environment. It supports basic shell operations such as command execution, input/output redirection, environment variable handling, and more. This project serves both as a learning tool for understanding shell operations and as a practical utility for executing commands in a Unix environment.

## Features

Minishell provides the following features:

- Interactive prompt with command execution.
- Command history with arrow key navigation.
- Path variable (`$PATH`) support for executing commands.
- Handling of single and double quotes for command arguments.
- Environment variable expansion (e.g., `$HOME`, `$USER`).
- Handling of special shell variables like `$?` for exit status.
- Signal handling:
  - Ctrl-C: Displays a new prompt on a new line.
  - Ctrl-D: Exits the shell.
  - Ctrl-\: No action (behaves like in Bash).
- Input redirection (`<`), output redirection (`>`, `>>`), and appending (`>>`).
- Here documents (`<<`).
- Pipelines (`|`) for chaining commands.
- Execution of all binaries available in the system.
- Built-in commands:
  - `echo` with `-n` option.
  - `cd` with relative or absolute path.
  - `pwd` with no options.
  - `export` with no options.
  - `unset` with no options.
  - `env` with no options or arguments.
  - `exit` with no options.


## Compilation

To compile the Minishell program, follow these steps:
Clone this repository:
```bash
git clone git@github.com:vitoriavital/minishell-42.git
cd minishell
```
Use the provided Makefile to compile the Minishell program:
```bash
make
```
This will generate the executable minishell.

## Usage (Main Part)
Run minishell:
```bash
./minishell
```

This will start the Minishell interactive prompt where you can enter commands and interact with the shell.

It is built to behave exactly the same as the shell command bellow:
# Simple Commands
```bash
ls -la
```
# Input/Output/Append Redirection
```bash
< input.txt cat | grep a output.txt
```
```bash
echo "Hello world!" > test.txt
```
```bash
wc -l < input.txt
```
```bash
echo "Additional line" >> test.txt
```
# Environment Variable Handling
```bash
echo $HOME
```
# Command with Quotes
```bash
echo "Hello world!"
```
# Here Documents
```bash
wc -l << EOF
```
# Pipeline (|) for Chaining Commands
```bash
cat input.txt | grep "pattern" | wc -l
```
# Built-in Commands
```bash
echo -n "Hello, "
```
```bash
cd ..
```
```bash
pwd
```
```bash
export MY_VAR=value
```
```bash
unset MY_VAR
```
```bash
env
```
```bash
exit
```

## Contributors

Minishell was created and developed by:

- [Vitória Vital](https://github.com/vitoriavital)
- [André Campos](https://github.com/Andrefcampos)

