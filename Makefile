arg_inject.so:
	gcc -std=c11 -O2 -fPIC -shared -ldl -o arg_inject.so arg_inject.c
