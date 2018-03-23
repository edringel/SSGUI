int objToIndex(int objnum){
	for (int i = 0; i <SSG.numobj;i++){
		if (SSG.SSGObj[i]->UId == objnum)
			return i;
	}
	return -1;
}