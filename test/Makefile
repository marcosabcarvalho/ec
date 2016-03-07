#This Makefile will build the arduino sketch as a
#terminal program

TARGET=ec
ARLIBS = Float64 Math64
XOBJ=Float64ops.o softfloat.o

#Don't edit below
###################################################

ARLIBDIR=../../libraries
ODIR=obj
SDIR=stubs

LIBS=-lstdc++

ARLARG=$(patsubst %,-I$(ARLIBDIR)/%,$(ARLIBS))
ARLOBJ=$(patsubst %,%.o,$(ARLIBS))

INCLUDE=$(ARLARG) -I$(SDIR)

ARSRC=$(wildcard ../*.ino)
ARXSRC=$(wildcard ../*.cpp)
AROBJ=$(patsubst ../%.ino,%.o,$(ARSRC))
AROBJ+=$(patsubst ../%.cpp,%.o,$(ARXSRC))

_OBJS=Arduino.o $(AROBJ) $(ARLOBJ) $(XOBJ)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
GCC=gcc

VPATH=$(patsubst %,$(ARLIBDIR)/%:,$(ARLIBS))$(SDIR):../

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc $(OBJS) -o $@ $(LIBS)

$(ODIR)/%.o:	%.cpp
	$(GCC) -c $< -o $@ $(INCLUDE)

$(ODIR)/%.o:	%.cxx
	$(GCC) -x c++ -c $< -o $@ $(INCLUDE)

$(ODIR)/%.o:	%.ino
	$(GCC) -x c++ -c $< -o $@ $(INCLUDE)

$(ODIR)/Arduino.o:	Arduino.cxx

clean:
	rm -f $(TARGET) *.o *~ *.o $(ODIR)/*.o $(SDIR)/*~
#	echo $(VPARG)

backup:
	rm -f ../$(TARGET).tgz
	tar cvzf ../$(TARGET).tgz ../ec2

