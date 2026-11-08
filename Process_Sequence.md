# Some Concepts & Process Sequence

## 1. Some Concepts

### Memory Blocks

**1. Text (Code Segment)**
- Stores the compiled machine code of the program.
- Contains the binary instructions executed by the CPU, such as `mov`, `add`, and `call` in assembly.

**2. Data (Initialized Data Segment)**
- Stores global and static variables explicitly initialized with non-zero values.

**3. BSS (Uninitialized Data Segment)**
- Stores global and static variables that are uninitialized or explicitly initialized to `0` or `NULL`.
- These variables are automatically initialized to zero.

**4. Heap**
- Used for dynamic memory allocation with `malloc()`, `calloc()`, and `realloc()`.
- Unlike stack memory, allocated memory is not automatically released when a function returns.
- Its lifetime continues until you explicitly call `free()` or the program terminates.
- Memory can be shared between functions by passing pointers.
- Its growth direction is implementation-dependent.

**5. Stack**
- Used for automatic storage, including local variables, function parameters, and return information.
- Usually operates on a Last-In, First-Out (LIFO) basis.
- Automatic local variables have a lifetime associated with their block or function execution.
- You don't call `free()` on stack variables.
- The stack typically grows downward on many architectures, but this is implementation-dependent.

### NULL

`NULL` is a special value representing a null pointer.

It indicates that a pointer does not point to a valid object.

Example:

```c
char *text = NULL;
```

Important:
- `free(NULL)` is safe.
- Dereferencing `NULL` is undefined behavior.

### File Descriptor (FD)

A **File Descriptor (FD)** is a non-negative integer used by a process to identify an open I/O resource through the kernel.

- The kernel assigns the lowest available FD when opening a resource.
- FD numbers commonly start at `0`.
- The maximum number of open descriptors depends on operating-system resource limits. It is not always `1024`.

Standard file descriptors:

| FD | Name | Purpose |
|---|---|---|
| `0` | `stdin` | Standard input |
| `1` | `stdout` | Standard output |
| `2` | `stderr` | Standard error |
| `3+` | Other resources | Commonly assigned to subsequently opened resources |

**FD Table:**
- The kernel maintains a file descriptor table for each process.
- Each entry refers to an open resource through a kernel-managed open-file description.
- When `read(fd, ...)` is called, the kernel uses the FD to identify which resource to read from.

Example:

```c
int fd = open("file.txt", O_RDONLY);
```

If `open()` returns `3`, FD `3` identifies the opened file.

**Important:** An FD is an identifier, not the file itself.

### /dev/tty

`/dev/tty` is a special device file in Unix-like systems that represents the controlling terminal of the current process.

It is not necessarily the destination of every `stdin` or `stdout` operation.

For example, input can come from a keyboard, a file, or a pipe.

### Static Variables

A `static` variable has **static storage duration**, meaning it exists for the entire lifetime of the program.

- Initialized only once.
- Retains its value between function calls.
- If not explicitly initialized, it is initialized to zero (`NULL` for pointers).
- Its scope depends on where it is declared.

Examples:

**Inside a function:**

```c
void counter(void)
{
    static int count = 0;
    count++;
}
```

The variable retains its value between calls but can only be accessed inside that function.

**Outside a function:**

```c
static int count = 0;
```

The variable has file scope and internal linkage, meaning it can only be accessed within that source file.

Static variables are commonly stored in the data or BSS segment, depending on their initialization.

---

## 2. get_next_line() Process Sequence

### 2.1. get_next_line() Function

**Purpose:** Read and return one line at a time from a file descriptor while preserving leftover text for the next call.

Declare the variables:

```c
static char *saved_text;
static size_t capacity;
char *line;
```

- `saved_text`: Stores the text read from the file that has not yet been returned.
- `capacity`: Stores the allocated memory size of `saved_text`.
- `line`: Stores the extracted line that will be returned.

Both `saved_text` and `capacity` are static because they must retain their values between calls.

**Argument validation:**

- If `fd < 0`, return `NULL`.
- If `BUFFER_SIZE <= 0`, return `NULL`.
- If an FD is invalid, `read()` will return `-1`.

If you use an explicit `FD_MAX`, the correct range condition is:

```c
if (fd < 0 || fd >= FD_MAX)
    return (NULL);
```

However, `FD_MAX` is not a universal Unix limit.

**Process:**

1. Call `ft_read(fd, saved_text, &capacity)` and update `saved_text` with its returned pointer.
2. Check whether `saved_text` is `NULL` or empty.
3. Call `ft_extract_line(saved_text)` to extract the first line.
4. Check whether line allocation succeeded.
5. Call `ft_update_saved_text(&saved_text, &capacity)` to keep only the leftover text.
6. If updating fails, free the allocated memory and return `NULL`.
7. Return `line`.

---

### 2.2. ft_read() Function

**Purpose:** Read from the file until a newline is found in the newly read buffer or EOF is reached.

#### 2.2.1. Declare variables

- `char *buffer`: Temporarily stores the newly read text.
- `ssize_t bytes_read`: Stores the number of bytes returned by `read()`.
- `char *tmp`: Temporarily stores the result of `ft_strjoin()`.

`ssize_t` is a signed integer type. Unlike `size_t`, it can represent negative values.

`bytes_read` can contain:

- Positive number: Number of bytes successfully read.
- `0`: EOF (End Of File).
- `-1`: An error occurred.

#### 2.2.2. Allocate memory for buffer

```c
buffer = malloc(BUFFER_SIZE + 1);
```

Allocate `BUFFER_SIZE + 1` bytes:

- `BUFFER_SIZE` bytes for the data.
- `1` extra byte for the null terminator (`'\0'`).

#### 2.2.3. Check the allocation

If `malloc()` fails:

- Free `saved_text`.
- Reset `saved_text` to `NULL`.
- Reset `capacity` to `0`.
- Return `NULL`.

#### 2.2.4. Initialize bytes_read

```c
bytes_read = 1;
```

This allows the reading loop to execute at least once.

If initialized to `0`, a loop conditioned on `bytes_read != 0` would never execute.

#### 2.2.5. Read loop

The new approach is to avoid repeatedly scanning the entire `saved_text`.

First, check whether `saved_text` already contains a newline. This is important because leftover text can contain another complete line.

If it doesn't contain a newline, start reading.

Inside the loop:

**1. Read data**

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Read up to `BUFFER_SIZE` bytes from the file.

**2. Check for errors**

```c
if (bytes_read < 0)
    return (ft_free_all(buffer, &saved_text, NULL, &capacity));
```

If `read()` returns `-1`, free the allocated memory and reset the saved state.

**3. Add the null terminator**

```c
buffer[bytes_read] = '\0';
```

This makes the newly read data a valid C string.

Only do this after checking that `bytes_read` is not negative.

**4. Append the new buffer**

```c
tmp = ft_strjoin(saved_text, buffer, &capacity);
```

`ft_strjoin()` appends the new buffer to `saved_text`.

**5. Check for allocation failure**

If `tmp == NULL`, call `ft_free_all()` to clean up and return `NULL`.

**6. Update saved_text**

```c
saved_text = tmp;
```

**7. Check only the new buffer for newline**

```c
if (is_has_newline(buffer) >= 0)
    break;
```

If the new buffer contains `\n`, stop reading.

The important point is that you only scan the newly read buffer on each iteration, rather than repeatedly scanning the potentially large `saved_text`.

However, check `saved_text` once before entering the loop so that an existing leftover newline is not missed.

**8. Stop at EOF**

If `bytes_read == 0`, stop reading.

#### 2.2.6. After the loop

- Free `buffer`.
- Return `saved_text`.

---

### 2.3. ft_strjoin() Function

**Purpose:** Append the new buffer to `saved_text` using dynamic memory allocation.

Unlike the previous implementation, this version uses `realloc()` and a capacity variable.

Example:

```c
saved_text = "Hello ";
buffer = "42\n";
```

Result:

```c
saved_text = "Hello 42\n";
```

**How capacity works:**

- `capacity` represents the number of bytes allocated for `saved_text`.
- `ft_strjoin()` checks whether the existing capacity is sufficient.
- If more memory is required, it increases the capacity using `realloc()`.
- The capacity can grow geometrically (for example, doubling its previous value).
- This avoids reallocating memory for every small append.

Important:

Always use a temporary pointer when calling `realloc()`:

```c
tmp = realloc(saved_text, new_capacity);
if (!tmp)
    return (NULL);
```

This prevents losing the original pointer if allocation fails.

---

### 3. Example: How ft_read() Works

Imagine the file contains:

```c
"Hello\nWorld\nFrom\nC"
```

And:

```c
BUFFER_SIZE = 4;
bytes_read = 1;
```

**First iteration**

Initially:

```c
saved_text = NULL;
capacity = 0;
```

Call:

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Result:

```c
buffer = "Hell";
bytes_read = 4;
```

Append the buffer:

```c
saved_text = "Hell";
```

No newline exists in the new buffer, so continue.

**Second iteration**

Read again:

```c
buffer = "o\nWo";
bytes_read = 4;
```

Append:

```c
saved_text = "Hello\nWo";
```

Check:

```c
is_has_newline(buffer);
```

The new buffer contains `\n`, so stop reading.

Free `buffer` and return:

```c
saved_text = "Hello\nWo";
```

**Important:** Even though `BUFFER_SIZE` is `4`, the total amount read is `8` bytes. `BUFFER_SIZE` is the maximum number of bytes requested per `read()` call, not the total amount read.

---

## 4. Back to get_next_line()

After `ft_read()` returns:

```c
saved_text = "Hello\nWo";
```

Check:

```c
if (!saved_text || *saved_text == '\0')
```

If either condition is true:

- Free `saved_text`.
- Set it to `NULL`.
- Reset `capacity` to `0`.
- Return `NULL`.

Otherwise, continue.

---

## 5. ft_extract_line() Function

**Purpose:** Extract the first line from `saved_text`, including the newline if one exists.

Call:

```c
line = ft_extract_line(saved_text);
```

Inside the function:

**1. Calculate the line length**

```c
line_length = ft_strlen(saved_text);
newline_index = is_has_newline(saved_text);

if (newline_index >= 0)
    line_length = newline_index + 1;
```

The first `+1` includes the newline character.

**2. Allocate memory**

```c
line = malloc(line_length + 1);
```

The second `+1` allocates space for the null terminator.

**3. Copy the characters**

Copy characters from `saved_text` into `line` until:

- A newline is found, or
- The string ends.

If a newline is found, include it.

**4. Add the null terminator**

```c
line[i] = '\0';
```

**5. Return the line**

Example:

```c
saved_text = "Hello\nWo";
```

Result:

```c
line = "Hello\n";
```

---

## 6. ft_update_saved_text() Function

**Purpose:** Remove the returned line from `saved_text` and preserve the leftover text for the next call.

Call:

```c
ft_update_saved_text(&saved_text, &capacity);
```

### Why pass &saved_text?

Because we need to modify the original pointer inside `get_next_line()`.

For example:

```c
char *saved_text = "Hello\nWorld";
```

- `saved_text` contains the address of the string.
- `&saved_text` contains the address of the pointer.
- `char **saved_text` allows the function to modify the original pointer.

### Inside ft_update_saved_text()

**1. Find the newline**

Move through `saved_text` until:

- A newline is found, or
- The string ends.

**2. Check whether leftover text exists**

If no newline exists, there is no remaining text after a complete returned line.

If the newline is the last character, there is also no leftover text.

In either case:

```c
free(*saved_text);
*saved_text = NULL;
*capacity = 0;
```

**3. Allocate the remaining text**

If leftover text exists, use `ft_strdup()` to allocate and copy it into `new_saved`.

Example:

```c
saved_text = "Hello\nWorld";
```

The remaining text is:

```c
new_saved = "World";
```

**4. Check allocation failure**

If `ft_strdup()` returns `NULL`:

- Return failure.
- Keep the original `saved_text` intact so the caller can clean it up.

**5. Free the old saved_text**

After successfully allocating the remaining text:

```c
free(*saved_text);
```

**6. Update the original pointer**

```c
*saved_text = new_saved;
```

**7. Update capacity**

If `ft_strdup()` allocates exactly enough memory for the leftover string:

```c
*capacity = ft_strlen(*saved_text) + 1;
```

The `+1` accounts for `'\0'`.

If there is no leftover text:

```c
*capacity = 0;
```

**8. Return success**

Return `1` to indicate success.

---

## 7. Return the Line

Back in `get_next_line()`:

If `ft_update_saved_text()` fails:

- Free `line`.
- Free `saved_text`.
- Set `saved_text` to `NULL`.
- Reset `capacity` to `0`.
- Return `NULL`.

Otherwise:

```c
return (line);
```

The caller receives:

```c
"Hello\n"
```

And the static variable retains:

```c
saved_text = "Wo";
```

On the next call to `get_next_line()`, reading continues using the same FD and the leftover text.

---

## 8. ft_free_all() Function

**Purpose:** Centralize memory cleanup and reset the saved state.

```c
char *ft_free_all(char *buffer, char **saved_text,
                  char *line, size_t *capacity)
{
    free(buffer);
    free(*saved_text);
    free(line);
    *saved_text = NULL;
    *capacity = 0;
    return (NULL);
}
```

Important:

- `free(NULL)` is safe.
- `saved_text` must be a valid pointer to a pointer.
- `capacity` must be a valid pointer.
- You can pass `NULL` for `buffer` or `line` if they are not allocated.
- You cannot pass `NULL` for `saved_text` or `capacity` because the function dereferences them.

Example:

```c
ft_free_all(NULL, &saved_text, line, &capacity);
```

This is valid if `saved_text` and `capacity` are valid variables.

