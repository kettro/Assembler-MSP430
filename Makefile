CC=gcc
CFLAGS=-Wall

SDIR=./src
TDIR=./test
IDIR=./inc
ODIR=./output

DIRGUARD=@mkdir -p $(@D)

EXEC=$(ODIR)/a
SRCS= $(wildcard $(SDIR)/*.c)
TSTS= $(wildcard $(TDIR)/*.c)
INCS= -I$(IDIR)
OBJS= $(SRCS:.c=.o)
TOBJS= $(TSTS:.c=.o)
OUT_OBJS= $(subst $(SDIR),$(ODIR),$(OBJS))
TEST_OBJS= $(subst $(TDIR),$(ODIR),$(TOBJS))

$(EXEC):$(OUT_OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(OUT_OBJS) $(TEST_OBJS) -o $(EXEC)

$(ODIR)/%.o:$(TDIR)/%.c
	$(DIRGUARD)
	$(CC) $(INCS) -c $< -o $@

$(ODIR)/%.o:$(SDIR)/%.c
	$(DIRGUARD)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f *.o *.lis *.s19 $(ODIR)/*.o
