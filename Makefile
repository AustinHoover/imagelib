libcv:
	gcc -c -fpic ./src/imageio.c      -o imageio.o                         -lm
	gcc -c -fpic ./src/imagecompare.c -o imagecompare.o
	gcc -c -fpic ./src/imagekernel.c    -o imagekernel.o
	gcc -shared -fpic imageio.o imagecompare.o imagekernel.o -o libimagelib.so
	rm ./*.o
