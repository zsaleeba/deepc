all clean:
	@(cd persmem && make $@) && \
		(cd cparser && make $@) && \
		(cd libdeepcc && make $@) && \
		(cd deepc && make $@)

count:
	wc `find . -name *.c; find . -name *.h`
