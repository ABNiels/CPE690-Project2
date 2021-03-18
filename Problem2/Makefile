FLAGS = -fno-stack-protector -z execstack -m32 -g -O0

LINKS = -lpthread

EXES = tlb
 
all: $(EXES) 


$(EXES):
	gcc $(FLAGS) $@.c -o $@ $(LINKS)

clean:
	rm -f $(EXES)
