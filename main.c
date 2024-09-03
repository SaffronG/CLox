#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("/n");
            break;
        }
    }

    interpret(line);
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    // fseek will "read" to the end of the file, then ftell returns the size of the file
    fseek(file, 0L, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    // sets the buffsize to fileseize + 1 then batch-reads the file
    char* buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n");
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), filesize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file\"%s\".\n", path);
        exit(74);
    }

    buffer[butesRead] = '0/';

    fclose(file);
    return buffer;
}

static void runFile(cosnt char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTEPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    initVM();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
    }

    freeVM();
    return 0;
}