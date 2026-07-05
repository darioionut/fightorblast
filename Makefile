# Name of the output executable file
EE_BIN = fight_or_blast.elf

# The object files that need to be compiled (main.c becomes main.o)
EE_OBJS = main.o

# Libraries to link. 
# -ldebug handles basic text output
# -lpad handles the controller ports
EE_LIBS = -ldebug -lpad

# Include standard PS2SDK build rules and environment settings
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
