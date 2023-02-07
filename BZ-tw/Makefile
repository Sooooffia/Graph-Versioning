all:
	mkdir -p tmp
	mkdir -p bin
	g++ -std=c++11 -o2 tmp/td-validate td-validate.cpp
	dmcs -optimize -define:SEQUENTIAL -out:bin/BZTreewidth-DP.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;LIMIT_SEPARATOR -out:bin/BZTreewidth-DP-LS.exe BZTreewidth/*.cs
	dmcs -optimize -out:bin/PARALLEL-BZTreewidth-DP.exe BZTreewidth/*.cs
	dmcs -optimize -define:LIMIT_SEPARATOR -out:bin/PARALLEL-BZTreewidth-DP-LS.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS -out:bin/BZTreewidth-DFS.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR -out:bin/BZTreewidth-DFS-LS.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE -out:bin/BZTreewidth-DFS-SV.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE\;DFS_MMW -out:bin/BZTreewidth-DFS-SV-MMW.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE\;DFS_MMW_LIMITED -out:bin/BZTreewidth-DFS-SV-MMWLIMIT.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE\;DFS_MMW\;DFS_EDGEADDITION\;DFS_ALMOSTSIMPLICIALRULE -out:bin/BZTreewidth-DFS-SV-ASV-EADD-MMW.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE\;DFS_MMW\;DFS_ALMOSTSIMPLICIALRULE -out:bin/BZTreewidth-DFS-SV-ASV-MMW.exe BZTreewidth/*.cs
	dmcs -optimize -define:SEQUENTIAL\;USE_DFS\;LIMIT_SEPARATOR\;DFS_SIMPLICIALRULE\;DFS_MMW_LIMITED\;DFS_ALMOSTSIMPLICIALRULE -out:bin/BZTreewidth-DFS-SV-ASV-MMWLIMIT.exe BZTreewidth/*.cs
	
	
.SUFFIXES: .gr .test
	
testfiles = $(wildcard ./BZTreewidth/instances/alltests/*.gr)
test_run = $(testfiles:.gr=.test)
	
test: $(test_run)
		cat tmp/validation.txt
	
.gr.test:
	./bin/BZTreewidth-DP.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DP-LS.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/PARALLEL-BZTreewidth-DP.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/PARALLEL-BZTreewidth-DP-LS.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-LS.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV-ASV-EADD-MMW.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV-ASV-MMW.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV-ASV-MMWLIMIT.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV-MMW.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt
	./bin/BZTreewidth-DFS-SV-MMWLIMIT.exe < $< > tmp/tmp.td
	./tmp/td-validate $< tmp/tmp.td 2>> tmp/validation.txt