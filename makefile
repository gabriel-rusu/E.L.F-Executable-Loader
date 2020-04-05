test:
	cd ./src;\
	make
save: clean
	git add -A	&&\
	echo Enter commit message: &&\
	read line &&\
	git commit -m "$$line" &&\
	git push
clean:
	rm -f *.o