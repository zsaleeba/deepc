all clean:
	@(cd libdeepcc; make $@)
	@(cd deepc; make $@)

count:
	wc `find . -name *.c; find . -name *.h`
