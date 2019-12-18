
obj-m:= flush_tlb.o

KDIR := /usr/src/kernels/3.10.0-1062.9.1.el7.x86_64/
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

user_src = user.c
user_obj = user.o

user: $(user_obj)
	$(CC) $< -o $@

test_src = test.c
test_obj = test.o

test: $(test_obj)
	$(CC) $< -o $@

clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers *~ user test
