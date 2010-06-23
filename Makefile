
all: build

build: clean setaml.c
	@mkdir dist;
	gcc -odist/setaml -Wall setaml.c

clean: 
	@if [ -d "dist" ]; then rm -rf "dist"; fi 	

install: dist 
	@if [ `id -u` -ne 0 ]; then \
		echo "Super-user priveliges required";\
		exit;\
	fi;\
	cp dist/setaml /usr/bin/setaml && \
	chown root:root /usr/bin/setaml && \
	chmod a=xs /usr/bin/setaml; \
	
