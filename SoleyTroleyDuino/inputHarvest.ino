int hiPotAVal = 0;
int hiPotBVal = 0;

void handlePots() {
	int tempAVal = analogRead(potA) >> 2;
	if (hiPotAVal > tempAVal + 1 || hiPotAVal < tempAVal- 1) {
		hiPotAVal = tempAVal;
		potAVal = hiPotAVal >> 1;

		if (USBSwitchState) {
			sendUSBMIDIcontrolChange(1, potAcc, potAVal);
		}
		else {
			//Serial1.println(potAVal + 10000);
			//Serial.println(potAVal + 10000);
		}
	}

	int tempBVal = analogRead(potB) >> 2;
	if (hiPotBVal  > tempBVal + 1 || hiPotBVal < tempBVal - 1) {
		hiPotBVal = tempBVal;
		potBVal = hiPotBVal >> 1;
		if (USBSwitchState) {
			sendUSBMIDIcontrolChange(1, potBcc, potBVal);
		}
		else {
			//Serial1.println(potBVal + 20000);
			//Serial.println(potBVal + 20000);
		}
	}


	byte tempTempoVal = analogRead(tempo) >> 3;
	if (tempoVal != tempTempoVal) {
		tempoVal = tempTempoVal;
		if (USBSwitchState && recSwitchState) {

			sendUSBMIDIcontrolChange(1, tempoCC, tempoVal);
		}
	}




}

void getbuttValues() {
	for (int i = 0; i < 5; i++) {
		int tempButtState = !digitalRead(buttPins[i]);
		if (!buttStates[i] && tempButtState) { //if buttstate was off and buttpin is pulled down by button
			buttBangs[i] = true;						   // BANG!
		}
		else {
			buttBangs[i] = false;
		}

		if (buttStates[i] && !tempButtState) { //if buttstate was on and buttpin is not pulled down by button
			buttBongs[i] = true;						   // BANG!
		}
		else {
			buttBongs[i] = false;
		}
		buttStates[i] = tempButtState;
	}
	if (!USBSwitchState && !digitalRead(USBSwitch)) {
		digitalWrite(powerLED, HIGH);
	}
	USBSwitchState = !digitalRead(USBSwitch);
	shiftButtState = !digitalRead(shiftButton);
	if (recSwitchState != digitalRead(recSwitch)) {
		byte val = 127 * !recSwitchState;
		sendUSBMIDIcontrolChange(1, recSwitchCCNumber, val);
		//Serial.println(val);
	}
	recSwitchState = digitalRead(recSwitch);
}
