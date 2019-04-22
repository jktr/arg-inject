# Arg Inject

Have you ever wanted to add config file support into
binary-only applications? Or maybe your application takes a password
via command line and you don't want it showing up in `ps`?

Enter Arg Inject, a horrible bit of LD_PRELOAD trickery.

## usage

Build the shared object via `$ make`.

Place a file named `inject.conf` (or set a filename via
ENV_INJECT_FILE) in your $PWD that contains your program's
arguments. Note that each argument should be on a new line.

Example:
```
$ cat inject.conf
-l
-a
```

You can then proceed to call your program as follows:

```
$ LD_PRELOAD=$PWD/arg_inject.so ls -these-are-ignored -in-favor-of -inject.conf
total 44
drwx------ 1 jktr users   154 Apr 22 06:47 .
drwx------ 1 jktr users   126 Apr 22 06:36 ..
-rw------- 1 jktr users  2030 Apr 22 06:44 arg_inject.c
-rwx------ 1 jktr users 16576 Apr 22 06:46 arg_inject.so
-rw------- 1 jktr users     6 Apr 22 06:46 inject.conf
-rw------- 1 jktr users    82 Apr 22 06:46 Makefile
-rw------- 1 jktr users   648 Apr 22 06:47 README.md
```

## license 

This work is licensed (CC BY-SA 3.0); it's largely adapted from MvG's
Answer on Serverfault, which can be found
[here](https://serverfault.com/questions/592744/how-to-hide-a-password-passed-as-command-line-argument/592941#592941).

