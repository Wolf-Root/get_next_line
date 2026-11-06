# Some Concepts && Process Sequence

## Some Concepts

### Memory Blocks

1- `Text` (Code segment):
    - This is where the compiled machine code of you program live.
    - The actual binary instructions executed by the CPU like `Assembly` instructions such as `mov`, `add`, `call`.

2- `data` (initialized data segment):
    - This segment stores `Global` and `Static` Variables that are explicity initialized with a non-zero value at compile.

3- `base` BSS: Black Started by Symbol (uninitialized segment):
    - This segment stores `Global` and `Static` Variables that are not initialized ore explicity initialized to `zero` or `NULL`.

4- `Heap`:
    - The region used for `dynamic memory allocation` when you use `malloc` or `calloc`, `realloc`.
    - Unlike `Stack` variables, `heap` memory doesn't clean itsulf up.
    - Whatever you allocat stays there until you explicity call `free()` or the program exits.
    - `Lifetme` lasts as long as you want it to, crossing functions boundaries freely.
    - `Direction` in most architectures, the keep grows `upward` (toward higher memory addresses) as you request more memory.

5- `Stack`:
    - The region used for `automatic memory` this is where normal local variables, functions arguments, and return addresses live.
    - `LIFO` Stracture it operates on a Last-In, First-Out basis, When you enter a function returns, the are automatically `popped` or wiped clean.
    - You naver call `free()` on a `Stack` variable; the `compiler` and `CPU` handlle it's creation and destruction automatically.
    - `Direction` The `Stack` typically grows `downward` (toward lower memory addresses) in `RAM`

### NULL

NULL:
    `NULL` is a special value that represents a "null pointer" - a pointer that does not point to anything.

### File Descriptor (FD)

FD:
    - The `file descriptor` (FD) is a none-nigativ integer assigned by the kernel to uniquely identify an open I/O resurces.
    - Is start from 0 to `OPEN_MAX` is 1024 by defaults but is can be increased.
    - `FD 0` is `stdiin` Standard input stream, like `keyboard`, it reads from `stdin` through fd 0 and saves it a file named `/dev/tty`.
    - `FD 1` is `stdout` Standard output stream, like `Terminal` window, it's from the file named `/dev/tty` and written to `stdout` in screen through FD 1.
    - `FD 2` is `stderr` Standard error stream, we see any error to the video screen, it is also from that file write to stderr in screen through fd 2.

### /dev/tty

`/dev/tty` is a special device file in `UNIX` that represents the `controlling terminal` of the current process `Active terminal session`.

### Static Variables

`Static` Variables: is a variable Has lifetime for `program` running time unlike normal variable which has only lifetime for its scope it's destroyed when the function finishes.
`Static` Variables: it's stored in `data` block memory unlike normal variable which is stored in the `stack`.
`Static` Variables: it is initialized to `NULL`, unlike normal variable that is initialized to a `garbage` value.
`Static` Variables: it `keeps` its value for every call, unlike normal variable that is destroyed and re-declared for every call.

## Process Sequence

### 1. `get_next_line()` function

1. Receive the `fd` argument (file descriptor).
2. Declare the variables:
   - `static char *saved_text`: stores the text read from the file that has not yet been returned. Because it is `static`, it keeps its value between function calls.
   - `char *line`: stores the line that will be returned.
3. Check the arguments:
   - If `fd < 0 && fd >= FD_MAX`, return `NULL`.
   - If `BUFFER_SIZE <= 0`, return `NULL`.
4. Call `ft_read(fd, saved_text)` and assign its return value to `saved_text`.

### 2. `ft_read()` function

**Purpose:** Read from the file until a newline is found or the end of the file is reached.

#### 2.1. Declare variables

- `char *tmp`: temporarily stores the result of `ft_append_text()`.
- `char *buffer`: stores the text read from the file.
- `ssize_t bytes_read`: stores the number of bytes returned by `read()`.

`ssize_t` is a signed integer type. Unlike `size_t`, it can store negative values.

`bytes_read` can contain:

- A positive number: the number of bytes successfully read.
- `0`: end of file (EOF).
- `-1`: an error occurred.

#### 2.2. Allocate memory for `buffer`

Allocate `BUFFER_SIZE + 1` bytes:

- `BUFFER_SIZE` bytes for the text.
- `1` extra byte for the null terminator (`'\0'`).

`BUFFER_SIZE` is the maximum number of bytes requested in each `read()` call.

#### 2.3. Check the allocation

If `malloc()` fails:

- Free `saved_text`.
- Return `NULL`.

#### 2.4. Initialize `bytes_read`

Set `bytes_read = 1`.

This makes the loop run at least once before the first call to `read()`.

If we initialized it to `0`, the condition `bytes_read != 0` would be false, and the loop would never run.

#### 2.5. Read loop

```c
while (!is_has_newline(saved_text) && bytes_read != 0)
```

The loop continues while:

- `saved_text` does not contain a newline.
- `bytes_read` is not `0` (EOF has not been reached).

Inside the loop:

1. Call `read(fd, buffer, BUFFER_SIZE)`.
   - Read up to `BUFFER_SIZE` bytes from the file.
   - Store the bytes in `buffer`.
   - Store the number of bytes read in `bytes_read`.

2. Check for a read error.

   If `bytes_read == -1`, call `ft_free_all(buffer, &saved_text)`.

   `ft_free_all()`:
   - Frees `buffer`.
   - Frees `saved_text`.
   - Sets `saved_text` to `NULL`.

3. Add the null terminator:

   ```c
   buffer[bytes_read] = '\0';
   ```

   This makes the content of `buffer` a valid C string.

4. Call `ft_append_text(saved_text, buffer)` and assign its return value to `tmp`.

5. Check if `ft_append_text()` failed.

   If `tmp == NULL`, call `ft_free_all()` to free the allocated memory and return `NULL`.

6. Update `saved_text`:

   ```c
   saved_text = tmp;
   ```

#### 2.6. After the loop

Free `buffer` because we do not need it anymore.

Return `saved_text`.

---

### 3. Example: How `ft_read()` works

Imagine the file contains:

```c
"Hello\nWorld\nFrom\nC"
```

And:

```c
BUFFER_SIZE = 4;
bytes_read = 1;
```

**First loop iteration**
Initially:

```c
saved_text = NULL;
```

The loop condition is true:

- `saved_text` has no newline.
- `bytes_read != 0`.

Call:

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Result:

```c
bytes_read = 4;
buffer = "Hell";
```

Add the null terminator:

```c
buffer = "Hell\0";
```

Call:

```c
tmp = ft_append_text(saved_text, buffer);
```

Inside `ft_append_text()`:

- `ft_strlen(saved_text)` returns `0` because `saved_text` is `NULL`.
- `ft_strlen(buffer)` returns `4`.
- Allocate `4 + 0 + 1 = 5` bytes for `new_text`.
- Copy the contents of `saved_text` (nothing).
- Copy `"Hell"` from `buffer` into `new_text`.
- Add `'\0'` at the end.
- Free the old `saved_text` (which is `NULL`).
- Return `new_text`.

Result:

```c
tmp = "Hell\0";
saved_text = tmp;
```

**Second loop iteration**
Check the condition again:

```c
!is_has_newline(saved_text) && bytes_read != 0
```

- `saved_text` is `"Hell"`, so it has no newline.
- `bytes_read` is `4`, so it is not `0`.

The loop continues.

Call `read()` again:

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Result:

```c
bytes_read = 4;
buffer = "o\nWo";
```

Add the null terminator:

```c
buffer = "o\nWo\0";
```

Call:

```c
tmp = ft_append_text(saved_text, buffer);
```

Inside `ft_append_text()`:

- `saved_text` is `"Hell"`.
- `buffer` is `"o\nWo"`.
- Allocate enough memory for both strings plus `'\0'`.
- Copy `"Hell"` and then `"o\nWo"`.
- Add the null terminator.
- Free the old `saved_text`.
- Return the new string.

Result:

```c
tmp = "Hello\nWo\0";
saved_text = tmp;
```

**Check the loop condition again**
Now `saved_text` contains `'\n'`.

Therefore:

```c
!is_has_newline(saved_text)
```

is false, and the loop stops.

Free `buffer` and return:

```c
saved_text = "Hello\nWo\0";
```

**Important:** Even though `BUFFER_SIZE` is `4`, we have read more than 4 bytes in total. Each `read()` call requests up to 4 bytes, and the loop keeps reading until it finds a newline or reaches EOF.

---

### 4. Back to `get_next_line()`

After `ft_read()` returns:

```c
saved_text = "Hello\nWo\0";
```

Check whether `saved_text` is `NULL` or empty:

```c
if (!saved_text || *saved_text == '\0')
```

If either condition is true:

- Free `saved_text`.
- Set it to `NULL`.
- Return `NULL`.

Otherwise, continue.

### 5. `ft_extract_line()` function

Call:

```c
line = ft_extract_line(saved_text);
```

**Purpose:** Extract the first line from `saved_text`.

For this example:

```c
saved_text = "Hello\nWo\0";
```

Inside `ft_extract_line()`:

1. Call `ft_line_memory_allocate(saved_text)` to allocate enough memory for the line.
   - If a newline exists, allocate `i + 2` bytes: `i` characters + `'\n'` + `'\0'`.
   - If no newline exists, allocate `i + 1` bytes: `i` characters + `'\0'`.

2. Loop through `saved_text` and copy characters into `line` until:
   - A newline is found, or
   - The string ends.

3. If a newline is found, copy it into `line` too.

4. Add the null terminator (`'\0'`) to close the string.

5. Return `line`.

Result:

```c
line = "Hello\n";
```

### 6. `ft_update_saved_text()` function

Now we need to remove the returned line from `saved_text`, because we do not want to return it again during the next call.

Call:

```c
ft_update_saved_text(&saved_text);
```

**Why pass `&saved_text`?**

We pass the address of `saved_text` because the function needs to modify the original pointer in `get_next_line()`.

Inside `ft_update_saved_text()`:

1. Declare:
   - `size_t i`: used to find the newline.
   - `char *new_saved`: stores the remaining text.

2. Start from index `0` and move through `saved_text` until:
   - A newline is found, or
   - The string ends.

3. If no newline exists:
   - Free the entire `saved_text`.
   - Set `*saved_text = NULL`.
   - Return `1`.

4. If a newline exists:
   - Move to the character immediately after it.
   - If there is no remaining text, free the old string, set `*saved_text = NULL`, and return `1`.
   - Otherwise, call `ft_subdup()` to copy the remaining text into `new_saved`.

5. Check if `ft_subdup()` failed.
   - If it returns `NULL`, return `0` without freeing the original `saved_text`.

6. Free the old `saved_text`.

7. Assign the new string to the original pointer:

   ```c
   *saved_text = new_saved;
   ```

8. Return `1` to indicate success.

**Example:**

Before updating:

```c
saved_text = "Hello\nWo";
```

After updating:

```c
saved_text = "Wo";
```

The returned line was `"Hello\n"`, so only `"Wo"` remains.

### 7. Return the line

Back in `get_next_line()`:

If `ft_update_saved_text()` returns `0`:

- Free `line`.
- Free `saved_text`.
- Set `saved_text` to `NULL`.
- Return `NULL`.

Otherwise:

```c
return (line);
```

The caller receives:

```c
"Hello\n"
```

The remaining text (`"Wo"`) stays in the static `saved_text` variable for the next call to `get_next_line()`.
