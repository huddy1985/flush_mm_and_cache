
obj-m:= flush_tlb.o

KDIR := /lib/modules/$(shell uname -r)/build
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
