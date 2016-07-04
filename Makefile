all clean:
	@(cd persmem && make $@) && \
		(cd libdeepcc && make $@) && \
		(cd deepc && make $@)

count:
	wc `find . -name *.c; find . -name *.h`
