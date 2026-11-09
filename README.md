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

### 1. Static variable and data structure

The implementation uses a static structure to preserve the necessary data between function calls.

```c
typedef struct s_data
{
    char    *saved_text;
    size_t  capacity;
}   t_data;
```

The structure contains two members:

- `saved_text`: Stores the text that has been read but has not yet been returned as a line.
- `capacity`: Tracks the allocated memory size of `saved_text`.

The structure is declared as a single static variable:

```c
static t_data data;
```

Unlike regular local variables, static variables retain their values between function calls.

Using a structure allows multiple pieces of persistent data to be managed through one static variable.

### 2. Reading data

A temporary buffer is allocated with a size of `BUFFER_SIZE + 1`.

The `read()` function reads up to `BUFFER_SIZE` bytes from the file descriptor.

The function continues reading until:
- A newline character (`\n`) is found.
- The end of the file is reached.

Each successful read is null-terminated before being processed.

### 3. Appending text

Each time new data is read, it is appended to `saved_text`.

The implementation uses dynamic memory allocation to ensure there is enough space for the existing text and the newly read data.

When the current capacity is insufficient:
1. A larger memory block is allocated using `malloc()`.
2. The existing text is copied into the new memory block.
3. The old memory block is freed.
4. The capacity is updated.

This approach allows the allocated memory to grow as needed.

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

## Bonus Part

The bonus part focuses on managing persistent data while respecting the requirement of using only one static variable.

Instead of declaring separate static variables, the implementation groups the required data inside a structure:

```c
typedef struct s_data
{
    char    *saved_text;
    size_t  capacity;
}   t_data;

static t_data data;
```

This approach keeps the implementation organized while allowing both the saved text and its allocated capacity to persist between calls.

**Multiple file descriptors:** To support multiple file descriptors simultaneously, the saved state must be maintained separately for each descriptor, for example, using a linked list managed through a single static pointer.

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
