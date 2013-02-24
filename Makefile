REPORTER = spec

all: compile

build:
	@node-gyp configure

compile: build
	@node-gyp build


clean:
	@node-gyp clean

test: build
	@node_modules/mocha/bin/mocha --reporter $(REPORTER)

.PHONY: test clean all compile
