#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENV_INJECT_FILE "INJECTEE"
#define DEFAULT_INJECT_FILE "./inject.conf"

typedef struct
{
  char **args;
  size_t used;
  size_t size;
} arg_array;

static void
init_arg_array (arg_array * a)
{
  a->args = malloc (sizeof (char *));
  a->used = 0;
  a->size = 1;
}

static void
append_arg_array (arg_array * a, char *arg)
{
  if (a->used == a->size)
    {
      a->size *= 2;
      a->args = realloc (a->args, a->size * sizeof (char *));
    }
  a->args[a->used++] = arg;
}

static int (*real_main) (int, char **, char **);

static int
inject (int argc, char **argv, char **env)
{
  char *inject_path = getenv (ENV_INJECT_FILE);
  if (inject_path == NULL)
    inject_path = DEFAULT_INJECT_FILE;

  FILE *fp = fopen (inject_path, "r");
  if (!fp)
    {
      fprintf (stderr, "could't open arg inject file %s\n", inject_path);
      return EXIT_FAILURE;
    }

  static arg_array inject;
  init_arg_array (&inject);	//unsafe init of static member
  if (argc > 0)
    append_arg_array (&inject, argv[0]);

  ssize_t nread;
  do
    {
      char *line = NULL;
      size_t len = 0;
      nread = getline (&line, &len, fp);
      if (nread != -1)
	append_arg_array (&inject, strtok (line, "\n"));	//leaks 1 byte at end
    }
  while (nread != -1);

  fclose (fp);

  return real_main (inject.used, inject.args, env);
}

int
__libc_start_main (int (*main) (int, char **, char **),
		   int argc, char **ubp_av,
		   void (*init) (void),
		   void (*fini) (void),
		   void (*rtld_fini) (void), void (*stack_end))
{
  int (*next) (int (*main) (int, char **, char **),
	       int argc, char **ubp_av,
	       void (*init) (void),
	       void (*fini) (void),
	       void (*rtld_fini) (void),
	       void (*stack_end)) = dlsym (RTLD_NEXT, "__libc_start_main");
  real_main = main;
  return next (inject, argc, ubp_av, init, fini, rtld_fini, stack_end);
}
