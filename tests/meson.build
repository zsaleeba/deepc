gtest_lib = meson.get_compiler('cpp').find_library('gtest')

t = executable('deepctest', 
	'main.cpp', 
	include_directories : libdeepcc_inc,
	link_with : libdeepcc_lib,
	dependencies : [gtest_lib, pthread_lib])

test('deepctest', t)
