build:
	cd ./src;\
	make
run: build
	cd ./src;\
	make -f Makefile.example;\
	LD_LIBRARY_PATH=. ./so_exec so_test_prog > ../result.out
save: clean
	git add -A	&&\
	echo Enter commit message: &&\
	read line &&\
	git commit -m "$$line" &&\
	git push
clean:
	cd ./src;\
	rm -f *.o so_exec so_test_prog libso_loader.so ../result.out