build:
	cd ./src;\
	make
run: build
	cd ./src;\
	make -f Makefile.example
save: clean
	git add -A	&&\
	echo Enter commit message: &&\
	read line &&\
	git commit -m "$$line" &&\
	git push
clean:
	rm -f *.o