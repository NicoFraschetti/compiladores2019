
#ifndef OFFSET_GENERATOR_C
#define OFFSET_GENERATOR_C

int offset = 0;

int param_offset = 0;

int getOffSet(){
    offset+=-8;
    return offset;
}

int offSet(){
	return offset;
}

void resetOffSet(int num){
	offset = num;
}

int getParamOffSet(){
	param_offset+=-8;
	return param_offset;
}

int paramOffSet(){
	return param_offset;
}

void resetParamOffSet(){
	param_offset = 0;
}

#endif