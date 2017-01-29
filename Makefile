all clean:
	@(cd persmem && $(MAKE) $@) && \
		(cd antlr4_runtime/runtime/src && $(MAKE) $@) && \
		(cd cparser && $(MAKE) $@) && \
		(cd libdeepcc && $(MAKE) $@) && \
		(cd deepc && $(MAKE) $@)

count:
	wc `find . -name *.c; find . -name *.h`
