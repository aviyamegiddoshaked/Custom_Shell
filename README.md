# Custom_Shell


This program is a basic shell implementation written in C, designed to provide a functional command-line interface for executing various commands. It supports built-in commands such as `cd`, `pwd`, `history`, and `exit`, ensuring essential shell functionality. Furthermore, the shell allows users to extend its command execution capabilities by specifying additional paths as arguments when launching the program. These paths are appended to the PATH environment variable, enabling seamless execution of a broader range of commands directly from the shell.


- Executes external commands through the `execvp` function.  
- Provides support for built-in commands such as `cd`, `pwd`, `history`, and `exit`.  
- Manages a command history with a configurable limit on stored entries.  
- Enables dynamic modification of the PATH environment variable.


  ## How to Use

1. **Compile the Program**: Use a C compiler like GCC to compile the source code.

    ```bash
    gcc -o myshell myshell.c
    ```

2. **Run the Shell**: Execute the compiled program.

    ```bash
    ./myshell
    ```

3. **Available Commands**:
    - **External Commands**: Run any external command available on your system.
    - **Built-in Commands**:
        - `cd [directory]`: Change the current working directory to the specified directory.
        - `pwd`: Display the current working directory path.
        - `history`: Show a list of previously entered commands.
        - `exit`: Terminate the shell session.

4. **Extending PATH**: You can add extra paths to the `PATH` environment variable by providing them as arguments when launching the shell. For example:

    ```bash
    ./myshell /usr/local/bin /opt/bin
    ```

    This allows you to execute commands located in `/usr/local/bin` and `/opt/bin` directly from the shell without specifying their full paths.
