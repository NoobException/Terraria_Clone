def FlagsForFile(filename, **kwargs):
    return{
        'flags':['-x', 'c++', '-Iheaders', '-Wall', '-Wextra', '-Werror', '-Iusr/include', '-I./Headers']
    }
