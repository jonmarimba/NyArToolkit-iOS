NAME_LIBRARY=libnyartoolkit.a
include makefile.env

SOURCES= \
	$(shell ls $(DIR_SOURCE)/core/*.cpp) $(shell ls $(DIR_SOURCE)/utils/*.cpp)

OBJECTS= \
	$(addprefix $(DIR_OBJECT)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

#	$(DIR_OBJECT)/$(patsubst %.cpp,%.o,$(shell ls $(DIR_SOURCE)/core/*.cpp)) \
#	$(DIR_OBJECT)/$(patsubst %.cpp,%.o,$(shell ls $(DIR_SOURCE)/utils/*.cpp))

all:$(DIR_OUTPUT)/$(NAME_LIBRARY)



$(DIR_OUTPUT)/$(NAME_LIBRARY):$(OBJECTS)
	ar rcu $(DIR_OUTPUT)/$(NAME_LIBRARY) $^

$(OBJECTS):
	cd $(DIR_SOURCE)/core;make objects
	cd $(DIR_SOURCE)/utils;make objects

clean:
	rm -f $(DIR_OBJECT)/*.o $(DIR_OUTPUT)/$(NAME_LIBRARY)

