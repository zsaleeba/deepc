project('deepc', 'cpp', 
	default_options : ['cpp_std=c++17'],
	version : '0.0.1',
	license : 'BSD')

lmdb_lib = meson.get_compiler('cpp').find_library('lmdb')
pthread_lib = meson.get_compiler('cpp').find_library('pthread')

subdir('libdeepcc')
subdir('deepc')
subdir('tests')
