CC=gcc
CFLAGS=

SDIR=./src
TDIR=./test
IDIR=./inc
ODIR=./output

EXEC=$(ODIR)/a
SRCS= $(wildcard $(SDIR)/*.c)
TSTS= $(wildcard $(TDIR)/*.c)
INCS= -I$(IDIR)
OBJS= $(SRCS:.c=.o)
TOBJS= $(TSTS:.c=.o)
OUT_OBJS= $(subst $(SDIR),$(ODIR),$(OBJS))
TEST_OBJS= $(subst $(TDIR),$(ODIR),$(TOBJS))

$(EXEC):$(OUT_OBJS) $(TEST_OBJS)
	$(CC) $(OUT_OBJS) $(TEST_OBJS) -o $(EXEC)

$(ODIR)/%.o:$(TDIR)/%.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f *.o $(ODIR)/*.o
