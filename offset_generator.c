#ifndef OFFSET_GENERATOR_C
#define OFFSET_GENERATOR_C

int offset = -8;

int getOffSet(){
    int old_offset = offset;
    offset+=-8;
    return old_offset;
}

#endif