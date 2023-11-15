game_of_life:
	cd c && gcc game_of_life.c -o game_of_life -Werror -Wall
test_sparse:
	cd c && gcc test_sparse.c sparse.c -o test_sparse -Werror -Wall
progress.pdf:
	pdflatex --shell-escape --jobname=progress --output-directory=latex latex/progress.tex
	pdflatex --shell-escape --jobname=progress --output-directory=latex latex/progress.tex
clean:
	rm -f c/test_sparse
	rm -f c/game_of_life
	rm -f latex/progress.pdf
	rm -f latex/*.aux
	rm -f latex/*.log
	rm -f latex/*.out
