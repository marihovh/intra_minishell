Minishell is a lightweight implementation of POSIX shell designed to provide a minimalistic command-line interface for interacting with an operating system.

Minishell will support the following features:

1.Display a user-friendly prompt when waiting for a new command, providing an intuitive user experience.

2.Maintain a working command history, allowing users to easily navigate and recall previously executed commands.

3.Search and launch the correct executable based on PATH variable or relative/absolute paths, offering a smooth command execution process.

4.Forgo support for unclosed quotes and special characters such as \ (backlash) and ; (semicolon), ensuring a cleaner and less error-prone user experience.

5.Support single quotes ' , preventing the shell from interpreting characters within the quoted sequence, allowing users to work with literal strings.

6.Support double quotes " , preventing the shell from interpreting characters within the quoted sequence, except for the $ (dollar sign) for variable expansion.

7.Provide support for I/O redirections and here documents (< , > , >> , << ), enabling users to redirect input and output between commands and files.

8.Handle environment variables, allowing users to set, modify, and retrieve variables as needed.

9.Manage ctrl-C , ctrl-D and ctrl-\ signals, ensuring proper behavior similar to that of bash.

10.Support special variables, such as $?, for context-based command behavior.

11.Implement built-in commands, including echo, cd, pwd, export, unset, env and exit, providing essential shell functionality out-of-the-box.

