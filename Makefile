arg_inject.so:
	gcc -std=c11 -o arg_echo arg_echo.c
	gcc -std=c11 -O2 -fPIC -shared -ldl -o arg_inject.so arg_inject.c

test: arg_inject.so
	/bin/bash -c "LD_PRELOAD=./arg_inject.so ./arg_echo <(echo -e 'testing\n1\n2\n3')"
	LD_PRELOAD=./arg_inject.so ./arg_echo
