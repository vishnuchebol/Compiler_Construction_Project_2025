parser: parser.o lexer.o driver.o keyword_table.o stackADT.o
	gcc parser.o lexer.o driver.o keyword_table.o stackADT.o -o stage1exe

lexer: lexer.o keyword_table.o driver.o
	gcc lexer.o keyword_table.o driver.o -o myprogram

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -w -c lexer.c

keyword_table.o: keyword_table.c keyword_table.h keyword_tableDef.h
	 gcc -w -c keyword_table.c

parser.o: parser.c parser.h parserDef.h lexer.h stackADT.h
	gcc -w -c parser.c

stackADT.o : stackADT.c stackADT.h stackADTDef.h
	gcc -w -c stackADT.c

driver.o: driver.c parser.h lexer.h
	 gcc -w -c driver.c