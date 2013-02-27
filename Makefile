REPORTER = spec
MOCHA = ./node_modules/mocha/bin/mocha
NODE = $(shell which node)
VALGRIND = $(shell which valgrind) --leak-check=full --show-reachable=yes
TPUT_HIGHLIGHT = $(shell tput setaf 1) 
TPUT_RESET = $(shell tput sgr0) 

all: compile

build: binding.gyp
	@node-gyp configure

compile: build
	@node-gyp build


clean:
	@node-gyp clean

test: compile node_modules
	@$(NODE) $(MOCHA) --reporter $(REPORTER)

valgrind: compile node_modules
	@script -qc "$(VALGRIND) $(NODE) $(MOCHA) --reporter $(REPORTER)" /dev/null 2>&1 | \
		grep --color -E "^|$$PWD"

node_modules:
	@npm install

.PHONY: test clean all compile valgrind
