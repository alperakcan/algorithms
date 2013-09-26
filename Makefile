
subdir-y = \
    src \
    test

test_depends-y = \
	src

include Makefile.lib

tests: test
	${Q}${MAKE} -C test tests
