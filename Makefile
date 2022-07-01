target = list.c
flags  = -Wall -Wextra -Wpedantic
run:
	@del *.exe
	@gcc $(flags) $(target) -o $(target:.c=.exe)
	@$(target:.c=.exe)
docs:
	pdflatex -quiet -output-directory=pdf PDFmain.tex
clean:
	del *.exe