TARGETS = all clean
BAT_TARGETS = $(addsuffix .bat, $(addprefix cmd_build., $(TARGETS)))
SETUP_ENV_COMMAND = "C:\\Program Files\\Microsoft Visual Studio 10.0\\Common7\\Tools\\vsvars32.bat"
WINDOWS_MAKEFILE = NMakefile.checker

build: $(BAT_TARGETS)

run: all

$(BAT_TARGETS):
	echo 'pushd . && $(SETUP_ENV_COMMAND) && popd && nmake /nologo /f $(WINDOWS_MAKEFILE) $(subst .bat,,$(subst cmd_build.,,$@))' > $@

$(TARGETS): % : cmd_build.%.bat
	cmd.exe /E:ON /V:ON /T:0E /C $<

bclean:
	-rm -f $(BAT_TARGETS) *~

pack:
	zip -r run_test_win.zip _test/ NMakefile.checker \
		GNUmakefile run_all.sh README
