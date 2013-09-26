
tests-y = \
	list-00 \
	list-01 \
	list-02 \
	queue-00 \
	queue-01 \
	queuea-00 \
	queuea-01 \
	queuea-02 \
	stack-00 \
	stack-01 \
	stack-02 \
	stacka-00 \
	stacka-01 \
	stacka-02 \
	selection-sort-00 \
	insertion-sort-00 \
	shell-sort-00 \
	merge-sort-00 \
	bottom-up-merge-sort-00 \
	quick-sort-00 \
	quick-select-00 \
	quick-sort-3way-00 \
	pqueue-00 \
	binary-heap-00 \
	heap-sort-00 \
	binary-search-00 \
	tree-00 \
	tree-rb-00

target-y = \
	${tests-y}

define test-defaults
	$1_files-y = \
		$(addsuffix .c, $1)
	
	$1_includes-y = \
		../src
		
	$1_libraries-y = \
		../src
	
	$1_ldflags-y = \
		-lsrc
	
	$1_depends-y = \
		../src/libsrc.a

endef

$(eval $(foreach T,$(target-y), $(eval $(call test-defaults,$T))))

include ../Makefile.lib

tests: all
ifeq ($(uname_S),Darwin)
	${Q}( \
	  for t in ${tests-y}; do \
	  echo "testing ./$$t $$(cat ./$$t.in)"; \
	  ./$$t $$(cat ./$$t.in) &> /tmp/$$t.out; \
	  diff -Naur ./$$t.out /tmp/$$t.out; \
	  done \
	)
else
	${Q}( \
	  for t in ${tests-y}; do \
	  echo "testing ./$$t $$(cat ./$$t.in)"; \
	  valgrind -q --leak-check=full ./$$t $$(cat ./$$t.in) &> /tmp/$$t.out; \
	  diff -Naur ./$$t.out /tmp/$$t.out; \
	  done \
	)
endif
