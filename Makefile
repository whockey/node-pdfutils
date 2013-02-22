REPORTER = spec

build:
	@node-gyp configure build

all: build

clean:
	@node-gyp clean

test: build
	@node_modules/mocha/bin/mocha --reporter $(REPORTER)

.PHONY: test clean all build
