/*
 * LDR
 */


int readLDR(){
    int inputVal = analogRead (analogInLDR); // Analog Values 0 to 1023
    /*
     * inputVal dark 1023
     * inputval bright 135
     * inputVal intermediate light 300
     * convert to dark 1 light 100
     */
    int outputVal = map(inputVal, 135, 1023, 100, 1);
    Serial.print("LDR light"); 
    Serial.println(outputVal);
    return outputVal;
  }
