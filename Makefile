TARGET	= pa2
CFLAGS	= -g

all: pa2

pa2: pa2.c
	gcc $(CFLAGS) $^ -o $@

pa2a: pa2.c
	gcc -DINPUT_ASSEMBLY $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf pa2 pa2a *.o pa2.dSYM pa2a.dSYM

.PHONY: test-basic
test-basic: pa2 testcases/basic
	./pa2 < testcases/basic 2>&1 >/dev/null

.PHONY: test-load
test-load: pa2 testcases/load testcases/program-basic
	./pa2 < testcases/load 2>&1 >/dev/null
