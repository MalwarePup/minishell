# <h1 align = "center"> minishell - as beautiful as a shell </h1>

<p align="center"> This README documents the completion of the 42Paris minishell project.</p>

> The existence of shells is linked to the very existence of IT. At the time, all developers agreed that communicating with a computer using aligned 1/0 switches was seriously irritating.

> It was only logical that they came up with the idea of creating a software to communicate with a computer using interactive lines of commands in a language somewhat close to the human language.

> The minishell project is about creating our own shell based on bash. It implements redirections and pipes, as well as environment variable expansions and the cd, echo, env, exit, export, pwd and unset builtin commands.

<h2> Project Requirements </h2>

<p align = "center">
<img width="616" alt="Screenshot 2023-05-08 at 08 29 11" src="https://user-images.githubusercontent.com/113340699/236750719-1f037981-0d76-4674-9a91-bdae3deaae1a.png">
</p>

<details>
<summary><h3>Expected shell behavior and limitations <i>(+ click to expand)</i> </h3></summary>


* Display a **prompt** when waiting for a new command.
* Have a working **history**.
* Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
* Not use more than **one global variable**. Think about it. You will have to explain its purpose.
* Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
* Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
* Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).

* Implement **redirections**:
	◦ < should redirect input.
	◦ > should redirect output.
	◦ << should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
	◦ >> should redirect output in append mode.
* Implement **pipes** (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
* Handle **environment variables** ($ followed by a sequence of characters) which should expand to their values.
* Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
* Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
* In interactive mode:
	◦ ctrl-C displays a new prompt on a new line.
	◦ ctrl-D exits the shell.
	◦ ctrl-\ does nothing.

* Your shell must implement the following **builtins**:
	◦ echo with option -n
	◦ cd with only a relative or absolute path.
	◦ pwd with no options
	◦ export with no options
	◦ unset with no options
	◦ env with no options or arguments
	◦ exit with no options

**BONUS**

Your program has to implement:
* && and || with parenthesis for priorities.
* Wildcards * should work for the current working directory.

The readline() function can cause memory leaks. We don’t have to fix them.
</details>

<h3>Usage</h3>

Clone the repository ```git@github.com:MalwarePup/minishell.git```
Compile with make ``` cd minishell && make```
Run the program : ```./minishell```
A prompt appears, now enter a command to be executed.

<h2>Basic Notions</h2>

<details>
<summary> <h3> What is Bash? </h3> </summary>
Bash is the shell, or command language interpreter, for the GNU operating system. The name is an acronym for the ‘Bourne-Again SHell’, a pun on Stephen Bourne, the author of the direct ancestor of the current Unix shell sh, which appeared in the Seventh Edition Bell Labs Research version of Unix.

Bash is largely compatible with sh and incorporates useful features from the Korn shell ksh and the C shell csh. It is intended to be a conformant implementation of the IEEE POSIX Shell and Tools portion of the IEEE POSIX specification (IEEE Standard 1003.1). It offers functional improvements over sh for both interactive and programming use.

While the GNU operating system provides other shells, including a version of csh, Bash is the default shell. Like other GNU software, Bash is quite portable. It currently runs on nearly every version of Unix and a few other operating systems - independently-supported ports exist for MS-DOS, OS/2, and Windows platforms.
</details>


<details>
<summary> <h3> What is a shell? </h3> </summary>
At its base, a shell is simply a macro processor that executes commands. The term macro processor means functionality where text and symbols are expanded to create larger expressions.

A Unix shell is both a command interpreter and a programming language. As a command interpreter, the shell provides the user interface to the rich set of GNU utilities. The programming language features allow these utilities to be combined. Files containing commands can be created, and become commands themselves. These new commands have the same status as system commands in directories such as /bin, allowing users or groups to establish custom environments to automate their common tasks.

Shells may be used interactively or non-interactively. In interactive mode, they accept input typed from the keyboard. When executing non-interactively, shells execute commands read from a file.

A shell allows execution of GNU commands, both synchronously and asynchronously. The shell waits for synchronous commands to complete before accepting more input; asynchronous commands continue to execute in parallel with the shell while it reads and executes additional commands. The redirection constructs permit fine-grained control of the input and output of those commands. Moreover, the shell allows control over the contents of commands’ environments.

Shells also provide a small set of built-in commands (builtins) implementing functionality impossible or inconvenient to obtain via separate utilities. For example, cd, break, continue, and exec cannot be implemented outside of the shell because they directly manipulate the shell itself. The history, getopts, kill, or pwd builtins, among others, could be implemented in separate utilities, but they are more convenient to use as builtin commands. All of the shell builtins are described in subsequent sections.

While executing commands is essential, most of the power (and complexity) of shells is due to their embedded programming languages. Like any high-level language, the shell provides variables, flow control constructs, quoting, and functions.

Shells offer features geared specifically for interactive use rather than to augment the programming language. These interactive features include job control, command line editing, command history and aliases. Each of these features is described in this manual.
</details>


<details>
<summary> <h3> How does a shell work? </h3> </summary>

The shell operates according to the following general overview of operations. The specific details are in the bash manual.

 Markup : * The shell reads its input from a file (see sh), from the -c option or from the system() and popen() functions defined in the System Interfaces volume of IEEE Std 1003.1-2001. If the first line of a file of shell commands starts with the characters "#!", the results are unspecified.

          * The shell breaks the input into tokens: words and operators; see Token Recognition.

          * The shell parses the input into simple commands (see Simple Commands) and compound commands (see Compound Commands).

          * The shell performs various expansions (separately) on different parts of each command, resulting in a list of pathnames and fields to be treated as a command and arguments; see Word Expansions.

          * The shell performs redirection (see Redirection) and removes redirection operators and their operands from the parameter list.

          * The shell executes a function (see Function Definition Command), built-in (see Special Built-In Utilities), executable file, or script, giving the names of the arguments as positional parameters numbered 1 to n, and the name of the command (or in the case of a function within a script, the name of the script) as the positional parameter numbered 0 (see Command Search and Execution).

          * The shell optionally waits for the command to complete and collects the exit status (see Exit Status for Commands).
</details>


<h2>Step by Step</h2>

 <details>

 <summary> <h3>PART 0 : ENVIRONMENT</h3></summary>


Chaque programme C possède une fonction principale qui doit être nommée main. La fonction main sert de point de départ pour l’exécution du programme. Elle contrôle généralement l'exécution du programme en dirigeant les appels à d'autres fonctions du programme. Il n’existe aucun prototype déclaré pour main et cette fonction peut être définie avec zéro, deux ou trois paramètres.  Le troisième paramètre, envp, est un tableau de pointeurs vers des variables d’environnement. Le tableau envp se termine par un pointeur null.

 ```
int main( void )
int main( int argc, char *argv[] )
int main( int argc, char *argv[], char *envp[] )
 ```

 Sometimes it is useful to communicate with a program in a semi-permanent way, so that you do not need to specify a command-line option every time you type the command to execute the program. One way to do this is to generate a configuration file, in which you can store data that will be used by the program every time it is run. This approach is typically useful if you have a large amount of data that you want to pass to a program every time it runs, or if you want the program itself to be able to change the data.

However, environment variables provide a more lightweight approach. Environment variables, sometimes called shell variables, are usually set with the export command in the shell. (This section assumes you are using the GNU Bash shell.) Standard environment variables are used for information about your home directory, terminal type, and so on; you can define additional variables for other purposes. The set of all environment variables that have values is collectively known as the environment.

Environment variables are stored in a special array that can be read by your main function. Envp is an array of strings, just as argv is. It consists of a list of the environment variables of your shell, in the following format: NAME=value.

Just as you can manually process command-line options from argv, so can you manually process environment variables from envp. However, the simplest way to access the value of an environment variable is with the getenv function, defined in the system header stdlib.h. It takes a single argument, a string containing the name of the variable whose value you wish to discover. It returns that value, or a null pointer if the variable is not defined.

We need to store this data in minishell because 1. if the command inputted by the user is a builtin we have been asked to recreate, it will run our implementation of this function, otherwise it will look for the corresponding executable in the directories contained in the PATH environment variable.

After check, we realised the usage of a third argument like this ```int	main(int ac, char **av, char **envp)``` is not specified in the C standard or POSIX (see this stack overflow thread: https://stackoverflow.com/questions/10321435/is-char-envp-as-a-third-argument-to-main-portable). As a reminder, POSIX refers to a set of standardized functions, variables, and definitions that provide a consistent interface for developers to write software that can run on any POSIX-compliant operating system. A POSIX-compliant method to get all is the extern char **environ :

```
int	main(void)
{
	extern char	**environ; // this variable
	char		**env;

	env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}

}
```

Therefore, at this stage :
- [ ] in main, call the extern char **environ and pass it to a function that will fill a structure with its content
- [ ] in the include .h file, add a structure for the environment that has three values : char *name, char *value and a pointer to the next node s_env *next, this structure is itself accessible through the master structure
- [ ] in env.c, the function ```create_environment``` browses through the char **environ and sends each char * to a node creator function named ```fill_environment```
- [ ] ```fill_envirnoment``` returns a new node for the env structure, with for example ```SHELL=/bin/zsh : char *name = "SHELL" char *value="=/bin/zsh"```


Source : http://crasseux.com/books/ctutorial/Environment-variables.html
Source : https://github.com/mavileo/minishell-42
 </details>

<details>
<summary><h3>PART 1 : LEXER</h3></summary>
The goal of the parsing in minishell is to (i) exit if there is an incorrect input (e.g an open double quote or extra and/or invalid operators) otherwise it is (ii) to sort and store the input in a linked list so it can eventually be executed. This process is called the tokenisation.

To collect the input, we use ```readline``` which takes the prompt as argument. For example, our minishell displays ```minishell:~$``` when launched, ```readline``` then returns a char * we named ```input_text``` that is the user's input. ```Input_text``` is passed as argument to the lexer ready for the tokenisation process.

Therefore, at this stage :
- [ ] ```check_quotes``` exits if ```input_text``` includes an open double quote ```"``` or open quote ```'```
- [ ] ```manage_token``` creates the token linked list whereby each node contains (i) a type and (ii) data. The detail of the lexing method is described below.
- [ ] for the type : in ```lexer.h``` we created an enum of all the operators so each type of token has an assigned value corresponding to its operators OR builtin by default.
- [ ] for the data : in ```trim_spaces``` we malloc the data making sure we (i) remove extra spaces, we (ii) only malloc the trimmed string that is the string until an operator but (iii) we take into account that the data we are trying to store is within quotes so must not be stored until the next operator but at the end of the quoting mechanism.

Enumeration (or enum) is a user defined data type in C. It is mainly used to assign names to integral constants, the names make a program easy to read and maintain. Our operators enum is as follows :

```
typedef enum e_token_type
{
	T_BUILTIN,
	T_PIPE,
	T_D_PIPE,
	T_AMP,
	T_D_AMP,
	T_RED_IN,
	T_D_RED_IN,
	T_RED_OUT,
	T_D_RED_OUT,
	T_END
}	t_token_type;
```

For example ```echo "BONJOUR" | cat -e``` will be splitted into the following tokens: ```[type:BUILTIN, data:(echo "BONJOUR")][type:PIPE, data:NULL][type:CMD, data:(cat -e)]```

Source : https://www.geeksforgeeks.org/enumeration-enum-c/
</details>

<details>
<summary><h3>PART 2 : PARSER</h3></summary>
At the parsing stage, the goal is to insert the token list in an  <a href="https://en.wikipedia.org/wiki/Abstract_syntax_tree" target="_blank">Abstract Syntax Tree</a> following the <a href="https://brilliant.org/wiki/shunting-yard-algorithm/" target="_blank">Shunting Yard Algorythm</a>'s methodology.

**Concrete syntax trees** reflect the syntax of the input language.
![image](https://user-images.githubusercontent.com/113340699/236853495-a68e56ad-56de-4a05-b034-eb6f72597c4d.png)

On the other hand, **abstract syntax tree**, or just syntax tree, is a tree representation of the abstract syntactic structure of text written in a formal language. Each node of the tree denotes a construct occurring in the text. The syntax is "abstract" in the sense that it does not represent every detail appearing in the real syntax, but rather just the structural or content-related details.

![image](https://user-images.githubusercontent.com/113340699/236852251-849674b2-cc8f-4c1e-b4bd-a4cdc5a1d955.png)

The AST is a way for us to structure our data following an analysis of the best way / order to the execute it. The rules to fill the tree are found in the Shunting yard algorithm, a "method for parsing arithmetical or logical expressions, or a combination of both, specified in [infix notation](https://www.cs.man.ac.uk/~pjj/cs212/fix.html) [...] that produces a postfix notation string or an abstract syntax tree (AST).". It is stack based :
* one stack is called the output queue and takes the numbers - in our case the commands
* one stack is called the operator stack and well takes the operators - which is similar to our case
* once the input has been read and filled that way in each stack, the operator stack is added at the end of the output queue.

For example : "3 + 4" becomes "3 4 +", or "3 + 4 × (2 − 1)" becomes "3 4 2 1 − × +". In our case, we will fill our tree with the following prority order:

| **Operator** | **Precedence** |
| -------------|:-------------:|
| ```\|```      | 	4 	|
| ```\|\|```     | 	3 	|
| ```&&```     |	3	|
| ```&```      |	2	|
| ```<```      |	1	|
| ```<<```     |	1	|
| ```>```      |	1	|
| ```>>```     |	1	|

For example : ```echo "BONJOUR" | cat -e``` becomes ```echo "BONJOUR" cat -e |``` in postfix notation and the tree is then filled from the output that is read from right to left. Within the operators stack, since we operate with a "first in, last out" implementation, we need to have a precedence order when evaluating whether we move the operator out. Put simply, if we have a pipe (precedence 4 so highest) in our operator stack, we take it out first.

```
         	|
	      /	  \
	    /	    \
	   |  	     |
     echo "BONJOUR" cat -e
```

> Put simply, user inputs commands in the terminal. We tokenise it, then we parse the tokens into a tree. To do so, we use two stacks, one for operators and one everything else. The goal is to end up with a "polish style" output meaning a command line ordered with every comman then all the operators from less important to most important. Eventually, we fill the tree reading that polish style output from right to left. We start by filling it with operators then we fill it with the corresponding commands.

</details>
	
	

<details>
<summary><h3>PART 3 : EXECUTION</h3></summary>
	
*1. PIPES* 
In this part, we are taking as input our ast nodes that give the order of execution. When it comes to pipes, more explanation below...

> The general idea: we read from infile, execute cmd1 with infile as input, send the output to cmd2, which will write to outfile.
pipe() sends the output of the first execve() as input to the second execve(); fork() runs two processes (i.e. two commands) in one single program; dup2() swaps our files with stdin and stdout. Visually,
	
![image](https://github.com/MalwarePup/minishell/assets/113340699/60a292eb-9abc-4a5f-b969-0a8745d2152e)
	
If we take an example from Mcombeau's article : 
	
![image](https://github.com/MalwarePup/minishell/assets/113340699/fe5cb65d-c7e2-4be2-b37f-c80a23506162)

Évidemment, le shell peut enchaîner plus de deux commandes avec l’opérateur pipe « | », comme par exemple man bash | head -n 50 | grep shell | grep bash | wc -l. C’est ce qu’on appelle une pipeline, c’est à dire une série de pipes. Si, pour répliquer un tel pipeline, on se contente d’utiliser un seul pipe pour l’entrée et la sortie de tous les processus fils, on rencontrera de gros problèmes. Vu que les processus fils sont exécutés simultanément, ils se bagarreront pour lire et écrire dans un seul tube. Et inévitablement, l’un d’entre eux finira par attendre une entrée qui ne viendra jamais.

Ce qu’il faut faire si l’on souhaite construire une pipeline, c’est créer un tube (une paire de descripteurs de fichiers) pour chaque processus fils, moins 1. Comme ça, le premier peut écrire dans la sortie de son propre tube, le deuxième lire depuis le tube du premier et écrire dans son propre tube et ainsi de suite.
	
	![image](https://github.com/MalwarePup/minishell/assets/113340699/9b829790-3528-4991-b796-bde5a6e06046)


Questions : 
	- should we unset PATH so that only the builtins we recreated are available to the user?
	
	
Source : https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
Source : https://www.youtube.com/watch?v=6xbLgZpOBi8
Source : https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/
	
	
*2. BUILTINS*
	
When our token type has been identified as BUILTIN, it means it might be one of the following builtins we have implemented : cd, echo, unset, export, env, pwd or exit. So we have : 

*0 Filled a char ** with the name of the builtins
*1 Inspect token sent - assign a type based on the builtin, if any 
	The type will be an int that corresponds to the relevant builtin enum 
*2 Generate an arg to send to builtin, in the format argc argv
*3 Send to correct builtin (using the type identified earlier)

	
	
</details>


<h2>Glossary</h2>

In the glossary, you can find the terms used and their meaning.

*builtin*
A command that is implemented internally by the shell itself, rather than by an executable program somewhere in the file system.
Source : https://www.gnu.org/software/bash/manual/bash.html

*environment variables*
A variable that is specific to an environment, mostly variables that are continuously used throughout your programme but that must be set by the user if need be to keep it secret/or specific to the environment you're working in. To access environment variables, simply put 'env' in your shell.To print values of envirnonment variable, type 'printevnv "nameofvariable"', or simply put "$variablename".
Source : https://www.youtube.com/watch?v=yM8v5i2Qjgg

*exit status*
The value returned by a command to its caller. The value is restricted to eight bits, so the maximum value is 255. It is a synonym of return status.
Source : https://www.gnu.org/software/bash/manual/bash.html

*token*
A sequence of characters considered a single unit by the shell. It is either a word or an operator.
Source : https://www.gnu.org/software/bash/manual/bash.html



<h2>Random notes</h2>

What is $PATH on a Linux Shell?
PATH is a list of directories including binaries/scripts that you can run separated by a colon. So PATH allows us to shorten commands, it is literally the reason why we can type 'ls', 'nano' etc without typing the path to those commands. PATH is a variable so typing 'echo $PATH' will give you the list of what is stored in this variable. So when we type a command, it searches for a directoy or a script in PATH with that name. It is not in home for security reasons. ( I infer that by adding scripts with sudo in PATH we can create our own functions but TBC )
Source : https://www.youtube.com/watch?v=hk0RwVC6uts

We should make our tests using **bash --posix**

### readline signal
By default readline use this builtin signal management. To avoid conflict modifiy the variable: int rl_catch_signals.
```c
	rl_catch_signals = 0;
```
This modification is present at the main of our project. See the [Readline Signal Handling](https://docs.rtems.org/releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html) for more details.

Using Excalidraw and VSC's LiveShare extension to work together


<h2>Interesting Sources</h2>

* [Shell Command Language Definition in POSIX](https://pubs.opengroup.org/onlinepubs/009604499/utilities/xcu_chap02.html)
* @pablo-lamtenzan's [tokenisation process](https://github.com/pablo-lamtenzan/miniShell)
* [@n1kito's](https://github.com/n1kito/minishell) minishell
* @vietdu91's [project](https://github.com/vietdu91/42_minishell) and his [minishell bible](https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit#gid=0)
* @bboisset's [checklist](https://docs.google.com/spreadsheets/d/1ssdLRjY8lJu4GK5IuoA3nf5Plkt7Kx-dNfc5KxvIcXg/edit?usp=sharing)
