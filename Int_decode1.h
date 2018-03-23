// helper function
int decodeInt(String *buildString, char delimiter) {
	String scratch = "";
	char nextchar;
	int i= 0; //iterator
	const char *ConvertBuffer;
	//get rid of whitespace, if present
	buildString->trim();
	//grab the next character
	do {
	nextchar = buildString->charAt(i);
	if (nextchar == delimiter){
		buildString-> remove(0, i+1);
		break;
	}//if
	scratch.concat(nextchar);
	i++;
	} while (nextchar != delimiter);
	//scratch will contain string to be converted
	//buildString will be shortened and ready for the next pass
	ConvertBuffer = scratch.c_str();
	return (uint16_t)strtol(ConvertBuffer, NULL, 0);
}