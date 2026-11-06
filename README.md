*This project has been created as part of the 42 curriculum by yaboualk.*

# get_next_line

## Description

**get_next_line** is a project from the 42 curriculum. The goal is to create a function that reads a file or input one line at a time.

The function returns one line on each call, including the newline character (`\n`) when it exists. It also handles the last line of a file even if it does not end with a newline.

The main challenge of this project is managing memory and preserving unread data between function calls.

### Features

- Read a file one line at a time.
- Work with different `BUFFER_SIZE` values.
- Handle lines of different lengths.
- Preserve remaining text between function calls.
- Handle empty files and files without a final newline.
- Manage memory allocation and errors.

## Instructions

### Requirements

- A C compiler (GCC or CC).
- A Unix-like operating system.

### Compilation

Compile the project with:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o program
```

You can change `BUFFER_SIZE` to test different reading sizes:

```bash
-D BUFFER_SIZE=1
-D BUFFER_SIZE=4
-D BUFFER_SIZE=100
```

If `BUFFER_SIZE` is not defined during compilation, the header uses a default value of `42`.

### Execution

Run the program:

```bash
./program
```

The project does not include a permanent `main.c`. You can create your own test file to open a file, call `get_next_line()`, and print the returned lines.

### Function Prototype

```c
char *get_next_line(int fd);
```

**Parameters:**
- `fd`: The file descriptor to read from.

**Return value:**
- A pointer to the next line, including `\n` if present.
- `NULL` when there is nothing left to read or when an error occurs.

## Algorithm

The algorithm is based on reading data in chunks and storing unread text between function calls using a static variable.

### 1. Static variable

The function uses a static pointer called `saved_text`.

```c
static char *saved_text;
```

Unlike a regular local variable, a static variable keeps its value between function calls.

It stores the text that has been read but has not yet been returned as a line.

### 2. Reading data

A temporary buffer is allocated with a size of `BUFFER_SIZE + 1`.

The `read()` function reads up to `BUFFER_SIZE` bytes from the file descriptor.

The function continues reading while:
- No newline character (`\n`) has been found in `saved_text`.
- The end of the file has not been reached.

This ensures that the function reads enough data to extract one complete line.

### 3. Appending text

Each time new data is read, it is appended to `saved_text`.

A new memory block is allocated to contain both the previously saved text and the newly read data.

The old memory block is freed after the new text has been successfully created.

### 4. Extracting a line

Once a newline is found or the end of the file is reached, the function extracts the first line from `saved_text`.

The returned line:
- Includes `\n` if a newline exists.
- Does not include `\n` if it is the final line without a newline.

### 5. Updating saved text

After extracting a line, the remaining text is copied into a new memory block.

For example, if:

```text
saved_text = "Hello\nWorld\n42"
```

After extracting `"Hello\n"`, the remaining text becomes:

```text
saved_text = "World\n42"
```

This remaining text is preserved for the next function call.

### 6. Memory management

Memory is allocated dynamically using `malloc()` and released using `free()`.

The algorithm checks allocation failures and handles reading errors to avoid memory leaks and invalid memory access.

### Why this algorithm?

Using a static variable allows the function to preserve unread data without requiring the caller to manage a buffer.

Reading in chunks instead of one character at a time reduces the number of `read()` calls, while keeping the implementation flexible for different `BUFFER_SIZE` values.

## Project Structure

```text
get_next_line/
│
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line.h
└── README.md
```

## Resources

### Documentation

- [read() — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [malloc() — Linux manual](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [free() — Linux manual](https://man7.org/linux/man-pages/man3/free.3.html)
- [Static variables in C](https://en.wikipedia.org/wiki/Static_variable)

### AI Usage

AI tools were used as learning and support resources during this project.

They were used for:
- Learning how static variables preserve data between function calls.
- Understanding dynamic memory allocation and memory management.
