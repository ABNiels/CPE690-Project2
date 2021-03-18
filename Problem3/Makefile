FLAGS = -fno-stack-protector -z execstack -m32 -g -O0

LINKS = -lpthread

EXES = visitor
 
all: $(EXES) 


$(EXES):
	gcc $(FLAGS) $@.c -o $@ $(LINKS)

clean:
	rm -f $(EXES)
