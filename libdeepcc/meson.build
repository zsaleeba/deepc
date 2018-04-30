libdeepcc_src =  ['clexer.cpp',
		'codegen.cpp', 
		'compileargs.cpp', 
		'compiler.cpp', 
		'cparser.cpp', 
		'fail.cpp', 
		'parsetree.cpp', 
		'preprocessor.cpp', 
		'programdb.cpp', 
		'sourcefile.cpp',
		'storable.cpp',
		'token.cpp']

libdeepcc_inc = include_directories('.')

flatc = find_program('flatc')
gen_src = custom_target('gen-sourcefile',
	input : ['storedobject.fbs'],
	output: ['storedobject_generated.h'],
	command: [flatc, '--cpp', '--binary', '-o', '@OUTDIR@', '@INPUT@'])

libdeepcc_lib = static_library('libdeepcc', libdeepcc_src, gen_src, dependencies : lmdb_lib)
