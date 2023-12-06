gol_sparse:
	cd c && gcc gol_sparse.c sparse.c gol_util.c -o gol_sparse -Werror -Wall -lm
game_of_life:
	cd c && gcc game_of_life.c -o game_of_life -Werror -Wall -lm
gol_parallel:
	cd c && mpicc -std=c99 gol_parallel.c gol_util.c sparse.c parallel.c communication.c -o gol_parallel -Werror -Wall -lm
test_sparse:
	cd c && gcc test_sparse.c sparse.c gol_util.c -o test_sparse -Werror -Wall -lm
test_parallel:
	cd c && gcc test_parallel.c parallel.c gol_util.c sparse.c -o test_parallel -Werror -Wall -lm
test_communication:
	cd c && mpicc test_communication.c gol_util.c sparse.c parallel.c communication.c -o test_communication -Werror -Wall -lm
test_toroidal:
	cd c && mpicc test_toroidal.c communication.c -o test_toroidal -Werror -Wall -lm
progress.pdf:
	pdflatex --shell-escape --jobname=progress --output-directory=latex latex/progress.tex
	biber latex/progress.bcf
	pdflatex --shell-escape --jobname=progress --output-directory=latex latex/progress.tex
final.pdf:
	pdflatex --shell-escape --jobname=final --output-directory=latex latex/final.tex
	biber latex/final.bcf
	pdflatex --shell-escape --jobname=final --output-directory=latex latex/final.tex
clean:
	rm -f c/test_sparse
	rm -f c/test_parallel
	rm -f c/test_communication
	rm -f c/test_toroidal
	rm -f c/game_of_life
	rm -f c/gol_sparse
	rm -f c/gol_parallel
	rm -f latex/progress.pdf
	rm -f latex/final.pdf
	rm -f latex/*.aux
	rm -f latex/*.log
	rm -f latex/*.out
	rm -f latex/*.bbl
	rm -f latex/*.bcf
	rm -f latex/*.blg
	rm -f latex/*.run.xml
	rm -rf _minted-progress
	rm -rf _minted-final
