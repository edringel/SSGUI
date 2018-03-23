//helper function


boolean inRect(uint16_t testX, uint16_t testY,uint16_t  tlx,uint16_t  tly,uint16_t  brx,uint16_t  bry) {
//	String debugString= String(t->x1) + "  " + String(t->x2) + "  " + String(t->y1) + "  "+ String( t->y2);
//	Serial.println(debugString);
	if ((testX > tlx) && (testX < brx) && (testY > tly) && (testY < bry))
			return true;
		else return false;
}