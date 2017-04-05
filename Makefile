all clean:
	@(cd persmem && $(MAKE) $@) && \
		(cd deepcomp && $(MAKE) $@) && \
		(cd cparser && $(MAKE) $@) && \
		(cd libdeepcc && $(MAKE) $@) && \
		(cd deepc && $(MAKE) $@)

count:
	wc `find . -name *.c; find . -name *.h`
