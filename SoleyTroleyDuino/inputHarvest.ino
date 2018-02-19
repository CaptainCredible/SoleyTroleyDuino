void getPotValues() {
	potAVal = analogRead(potA);
	potBVal = analogRead(potB);
	byte tempTempoVal = analogRead(tempo) >> 3;
	if (tempoVal != tempTempoVal) {
		tempoVal = tempTempoVal;
		sendUSBMIDIcontrolChange(1, tempoCC, tempoVal);
		//Serial.println(tempoVal);
	}
	

}

void getbuttValues() {
	for (int i = 0; i < 5; i++) {
		
		if (!buttStates[i] && !digitalRead(buttPins[i])) { //if buttstate was off and buttpin is pulled down by button
			buttBangs[i] = true;						   // BANG!
		}
		else {
			buttBangs[i] = false;
		}

		if (buttStates[i] && digitalRead(buttPins[i])) { //if buttstate was on and buttpin is not pulled down by button
			buttBongs[i] = true;						   // BANG!
		}
		else {
			buttBongs[i] = false;
		}



		buttStates[i] = !digitalRead(buttPins[i]);
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
