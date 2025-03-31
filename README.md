# Unix-like Utilities in C

A minimal implementation of core Unix commands.

## Programs
- **`My_Cp`** - File copy utility (`cp` clone)
- **`My_Echo`** - Print text to stdout (`echo` clone)  
- **`My_Mv`** - File move/rename utility (`mv` clone)
- **`My_Pwd`** - Print working directory (`pwd` clone)

## Build
```bash
gcc My_Cp.c -o My_Cp
gcc My_Echo.c -o My_Echo
gcc My_Mv.c -o My_Mv
gcc My_Pwd.c -o My_Pwd
```
## Runtime Examples

# Copy with safety checks
```bash
./My_Cp secrets.txt backup.txt
```

# Echo with precise spacing
```bash
./My_Echo Hello World
```

# Move with atomic replacement
```bash
./My_Mv old_name.txt new_location.txt"
```

# Show current directory path
```bash
./My_Pwd"
```




















