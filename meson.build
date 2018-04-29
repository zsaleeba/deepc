project('deepc', 'cpp', 
	default_options : ['cpp_std=c++17'])

src =  ['clexer.cpp',
	'codegen.cpp', 
	'compileargs.cpp', 
	'compiler.cpp', 
	'cparser.cpp', 
	'fail.cpp', 
	'main.cpp', 
	'parsetree.cpp', 
	'preprocessor.cpp', 
	'programdb.cpp', 
	'sourcefile.cpp',
	'storable.cpp',
	'token.cpp']

flatc = find_program('flatc')
gen_src = custom_target('gen-sourcefile',
	input : ['storedobject.fbs'],
	output: ['storedobject_generated.h'],
	command: [flatc, '--cpp', '--binary', '@INPUT@'])

lmdb_lib = meson.get_compiler('cpp').find_library('lmdb')

executable('deepc', src, gen_src, dependencies : lmdb_lib)
