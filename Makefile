libcv:
	gcc -c -fpic ./src/imageio.c      -o imageio.o                         -lm
	gcc -c -fpic ./src/imagecompare.c -o imagecompare.o
	gcc -c -fpic ./src/imgkernel.c    -o imgkernel.o
	gcc -shared -fpic imageio.o imagecompare.o imgkernel.o -o libimagelib.so
	rm ./*.o
