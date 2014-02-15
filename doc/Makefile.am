EXTRA_DIST=header.html footer.html

clean-local:
	rm -f index.html doxygen.config
	rm -rf html/

docs: doxygen.config
	doxygen doxygen.config
