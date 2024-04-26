doc :
	doxygen ./Doxyfile

latex : doc
	make -C ./latex
	cp ./latex/refman.pdf doc.pdf
	