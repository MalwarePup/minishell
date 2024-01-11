# Minishell
As beautiful as a shell - Version: 7.1

## Scale for project
### Mandatory
#### Compile
- [x] Use "make -n" to see if compilation use "-Wall -Wextra -Werror".
	If not, select the "invalid compilation" flag.
- [x] minishell compiles without any errors. If not, select the flag.
- [x] The Makefile must not re-link. If not, select the flag.

#### Simple Command & global variables
- [x] Execute a simple command with an absolute path like /bin/ls, or any
	other command without any options.
- [ ] How many global variables are used? Why? Ask the evaluated student
	to give you a concrete example of why it feels mandatory or logical.
- [ ] Check the global variable. This global variable cannot provide any
	other information or data access than the number of a received signal.
- [ ] Test an empty command.
- [ ] Test only spaces or tabs.

#### Arguments
- [x] Execute a simple command with an absolute path like /bin/ls, or any
	other command with arguments but without any quotes or double quotes.
- [x] Repeat multiple times with different commands and arguments.

#### echo
- [ ] Execute the echo command with or without arguments, or the -n option.
- [ ] Repeat multiple times with different arguments.

#### exit
- [x] Execute exit command with or without arguments.
- [x] Repeat multiple times with different arguments.
- [x] Don't forget to relaunch the minishell

#### Return value of a process
- [ ] Execute a simple command with an absolute path like /bin/ls, or any other
	command with arguments but without any quotes and double quotes. Then
	execute echo $?
- [ ] Check the printed value. You can do the same in bash in order to compare
	the results.
- [ ] Repeat multiple times with different commands and arguments. Try some
	wrong commands like '/bin/ls filethatdoesntexist'
- [x] Try anything like expr $? + $?

#### Signals

- [x] ctrl-C in an empty prompt should display a new line with a new prompt.
- [x] ctrl-\ in an empty prompt should not do anything.
- [x] ctrl-D in an empty prompt should quit minishell. Then relaunch it
- [x] ctrl-C in a prompt after you wrote some stuff should display a new line
	with a new prompt.
- [x] The buffer should be clean too. Press "Enter" to make sure nothing from
	the previous line is executed.
- [x] ctrl-D in a prompt after you wrote some stuff should not do anything.
- [x] ctrl-\ in a prompt after you wrote some stuff should not do anything.
- [ ] Try ctrl-C after running a blocking command like cat without arguments or grep
	“something“.
- [x] Try ctrl-\ after running a blocking command like cat without arguments or grep
	“something“.
- [x] Try ctrl-D after running a blocking command like cat without arguments or grep
	“something“.
- [x] Repeat multiple times using different commands.

#### Double Quotes
- [x] Execute a simple command with arguments and, this time, use also double
	quotes (you should try to include whitespaces too).
- [x] Try a command like : echo "cat leaks_ignore.supp | cat > lol.c"
- [x] Try anything except $.

#### Single Quotes
- [x] Execute commands with single quotes as arguments.
- [x] Try empty arguments.
- [x] Try environment variables, whitespaces, pipes, redirection in the single quotes.
- [x] echo '$USER' must print "$USER".
- [ ] Nothing should be interpreted.

#### env
- [x] Check if env shows you the current environment variables.

#### export
- [x] Export environment variables, create new ones and replace old ones.
- [x] Check the result with env.

#### unset
- [x] Export environment variables, create new ones and replace old ones.
- [x] Use unset to remove some of them.
- [x] Check the result with env.

#### cd
- [x] Use the command cd to move the working directory and check if you are in
	the right directory with /bin/ls
- [x] Repeat multiple times with working and not working cd
- [x] Also, try '.' and '..' as arguments.

#### pwd
- [x] Use the command pwd.
- [x] Repeat multiple times in different directories.

#### Relative Path
- [ ] Execute commands but this time use a relative path.
- [ ] Repeat multiple times in different directories with a complex
	relative path (lots of ..).

#### Environment path
- [ ] Execute commands but this time without any path (ls, wc, awk and so forth).
- [ ] Unset the $PATH and ensure commands are not working anymore.
- [ ] Set the $PATH to a multiple directory value (directory1:directory2) and
	ensure that directories are checked in order from left to right.

#### Redirection
- [ ] Execute commands with redirections < and/or >
- [ ] Repeat multiple times with different commands and arguments and sometimes
	change > with >.
- [ ] Check if multiple tries of the same redirections fail.
- [ ] Test << redirection (it doesn't have to update the history).

#### Pipes
- [ ] Execute commands with pipes like 'cat file | grep bla | more'
- [ ] Repeat multiple times with different commands and arguments.
- [ ] Try some wrong commands like 'ls filethatdoesntexist | grep bla | more'
- [ ] Try to mix pipes and redirections.

#### Go Crazy and history
- [ ] Type a command line, then use ctrl-C and press "Enter". The buffer should
	be clean and there should be nothing left to execute.
- [ ] Can we navigate through history using Up and Down? Can we retry some command?
- [ ] Execute commands that should not work like 'dsbksdgbksdghsd'.
	Ensure minishell doesn't crash and prints an error.
- [ ] 'cat | cat | ls' should behave in a "normal way".
- [ ] Try to execute a long command with a ton of arguments.
- [ ] Have fun with that beautiful minishell and enjoy it!

#### Environment variables
- [ ] Execute echo with some environment variables ($variable) as arguments.
- [ ] Check that $ is interpreted as an environment variable.
- [ ] Check that double quotes interpolate $.
- [ ] Check that USER exists. Otherwise, set it.
- [ ] echo "$USER" should print the value of the USER variable.

### Bonus
#### And, Or
- [ ] Use &&, || and parenthesis with commands and ensure minishell behaves
	the same way bash does.

#### Wildcard
- [ ] Use wildcards in arguments in the current working directory.

#### Surprise! (or not...)
- [ ] Set the USER environment variable.
- [ ] echo "'$USER'" should print the value of the USER variable.
- [ ] echo '"$USER"' should print "$USER".
