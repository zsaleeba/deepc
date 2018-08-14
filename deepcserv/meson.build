deepcserv_src = ['main.cpp', 'lspserver.cpp']

jsonrpcstub = find_program('jsonrpcstub')

deepcserv_gensrc = custom_target('gen-output',
                        input : ['lsp_spec.json'],
                        output : ['abstractlspserver.h'],
                        command : [jsonrpcstub, '@INPUT@', 
				'--cpp-server-file', '@OUTPUT@',
				'--cpp-server', 'AbstractLspServer'])

executable('deepcserv', 
	[deepcserv_src, deepcserv_gensrc], 
	include_directories : libdeepcc_inc,
	link_with : libdeepcc_lib)
