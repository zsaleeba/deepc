project('deepc', 'cpp', 
	default_options : ['cpp_std=c++14'])

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
	'token.cpp']

flatc = find_program('flatc')
gen_src = custom_target('gen-sourcefile',
	input : ['sourcefile.fbs'],
	output: ['sourcefile_generated.h'],
	command: [flatc, '--cpp', '--binary', '@INPUT@'])

executable('deepc', src, gen_src)
