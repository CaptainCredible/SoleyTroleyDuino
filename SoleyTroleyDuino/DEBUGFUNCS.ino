
void debugButtNumbers() {
	for (byte i = 0; i < 5; i++) {
		if (!digitalRead(buttPins[i])) {
			//Serial.println(i);
		}
	}
}

void pwrLedDebug(byte boolToDebug) {
	//digitalWrite(powerLED, !digitalRead(pinToDebug));
	digitalWrite(powerLED, boolToDebug);
}

void debugPotValues() {
	Serial.print("potA = ");
	Serial.print(analogRead(potA));
	Serial.print("   potB = ");
	Serial.print(analogRead(potB));
	Serial.print("   tempo = ");
	Serial.println(analogRead(tempo));

}