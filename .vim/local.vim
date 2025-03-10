setlocal makeprg=cmake\ --build\ ./build
setlocal errorformat=%f:%l:%m
set path+=src/emu/lib,src/emu/executable,src/utils,src/asm/lib,src/asm/executable,src/** 
