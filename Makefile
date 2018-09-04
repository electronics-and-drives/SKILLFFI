ffi:
	gcc -g -o skillFFI ffi.c -ldl -lffi

clean:
	rm -f *.o
	rm skillFFI
