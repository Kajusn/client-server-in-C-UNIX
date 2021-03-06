COMPILER=gcc
COMPILER_ARGUMENTS=-Wall

SOURCE_FILES_CLIENT=client/*.c
SOURCE_FILES_SERVER=server/*.c

OUTPUT_FOLDER=build/
OUTPUT_FILE_CLIENT=client
OUTPUT_FILE_SERVER=server

build:
	rm -rf $(OUTPUT_FOLDER)
	mkdir $(OUTPUT_FOLDER)
	$(COMPILER) $(COMPILER_ARGUMENTS) $(SOURCE_FILES_CLIENT) -o $(OUTPUT_FOLDER)$(OUTPUT_FILE_CLIENT) 
	$(COMPILER) $(COMPILER_ARGUMENTS) $(SOURCE_FILES_SERVER) -o $(OUTPUT_FOLDER)$(OUTPUT_FILE_SERVER)

clean:
	rm -rf $(OUTPUT_FOLDER) 
