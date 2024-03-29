CC=arm-linux-gnueabi-gcc
#CC=gcc

scriptDir=$(shell pwd)
objectDir=bin
srcDir=src
headDir=include

CFLAGS=-Wall -g -DDEBUG -I$(scriptDir)/$(headDir)
EFLAGS=-lpthread -lm

srcExt=.c
headerExt=.h

launcher=client.out

directories=$(shell find $(srcDir) -type d | sed '1d')
headDirectories=$(shell find $(headDir) -type d | sed '1d')

srcFile=$(foreach dir, $(directories), $(wildcard $(dir)/*$(srcExt)))

headers=$(foreach dir, $(headDirectories), $(wildcard $(scriptDir)/$(dir)/*$(headerExt)))

objects=$(foreach file, $(srcFile:$(srcExt)=.o), $(objectDir)/$(notdir $(file)))

all: $(launcher)

install:
	scp $(launcher) root@192.168.10.10:/home/root/AlexisEtBastien

$(launcher): compilation
	$(CC) -o $(launcher) $(objects) $(EFLAGS)

compilation:
	@mkdir $(objectDir) -p
	@for dir in $(directories); do \
		if [ ! `ls $$dir | grep $(srcExt) | wc -l` -eq 0 ]; then \
			if [ ! -e $$dir/Makefile ]; then \
					cp Makefile_type $$dir/Makefile; \
			fi; \
			make --no-print-directory -C $$dir objectDir="$(scriptDir)/$(objectDir)" headers="$(headers)" CC=$(CC) CFLAGS="$(CFLAGS)" srcExt="$(srcExt)" headerExt="$(headerExt)";	\
		fi; \
	done

clean_all: clean
	rm -f $(launcher)

clean:
	rm -Rf $(objectDir)

clean_error:
	@for dir in $(directories); do \
		rm -f $$dir/*.o; \
	done

clean_Makefile:
	@for dir in $(directories); do	\
		rm -f $$dir/Makefile;	\
	done
