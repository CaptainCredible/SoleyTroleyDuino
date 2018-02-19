/*
Name:		Sketch2.ino
Created:	2/5/2018 11:43:25 AM
Author:	Stoodio 2nd
*/

//#include "MIDIUSB.h"

const byte NOTEON = 0x09;
const byte NOTEOFF = 0x08;

#define powerLED 5
#define USBSwitch 16
#define recSwitch 3
#define shiftButton 14
const byte buttPins[5] = { 4,6,7,8,9 };
#define potA 0
#define potB 1
#define tempo 2
#define recSwitchCCNumber 42
#define tempoCC 43


// the setup function runs once when you press reset or power the board
int tempoVal = 120;
int potAVal = 0;
int potBVal = 0;
int desiredNoteLength = 50;

byte channelOffset = 0; // should be set to 5 * current channel
bool recSwitchState = false;
bool shiftButtState = false;
bool USBSwitchState = false;
bool buttStates[5] = { false,false,false, false, false };
bool buttBangs[5] = { false, false, false, false, false };
bool buttBongs[5] = { false, false, false, false, false };
bool oldButtState = false;
void setup() {
	Serial1.begin(19200);
	pinMode(powerLED, OUTPUT);
	pinMode(USBSwitch, INPUT);
	pinMode(shiftButton, INPUT_PULLUP);
	pinMode(recSwitch, INPUT);
	pinMode(potA, INPUT);
	pinMode(potB, INPUT);
	pinMode(tempo, INPUT);
	digitalWrite(powerLED, HIGH);
	for (byte i = 0; i < 5; i++) {
		pinMode(buttPins[i], INPUT_PULLUP);
	}
	//pinMode(1, OUTPUT);
}

void sendUSBMIDInoteOn(byte channel, byte pitch, byte velocity) {
	MIDIEvent noteOn = { 0x09, 0x90 | channel, pitch, velocity };
	MIDIUSB.write(noteOn);
	MIDIUSB.flush();
}

void sendUSBMIDInoteOff(byte channel, byte pitch, byte velocity) {
	MIDIEvent noteOff = { 0x08, 0x80 | channel, pitch, velocity };
	MIDIUSB.write(noteOff);
	MIDIUSB.flush();
}

void sendUSBMIDIcontrolChange(byte channel, byte control, byte value) {
	MIDIEvent event = { 0x0B, 0xB0 | channel, control, value };
	MIDIUSB.write(event);
	MIDIUSB.flush();
}

// the loop function runs over and over again until power down or reset
void loop() {
	getbuttValues();
	handleButts();
	getPotValues();
	handlePots();
	usbMidiProcessing();
	handlePowerLed();
	handleOnTimers();
	//debugPotValues();
	//pwrLedDebug(USBSwitchState);
	//debugButtNumbers();
}

byte currentChannel = 0;  //0 to 4
unsigned long onTimers[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //how long has this note been on
bool noteIsOn[25] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

void handlePots() {

}

void handleOnTimers() {
	for (byte i = 0; i < 25; i++) {
		if (noteIsOn[i]) {
			if (millis() - onTimers[i] > desiredNoteLength) {
				HandleNoteOff(i);
				sendUSBMIDInoteOff(1, i, 0);
			}
		}
	}
}
int incrementor = -255;
int powerLEDPWM = 0;
void handlePowerLed() {
	if (!USBSwitchState) {
		incrementor++;
		if (incrementor > 255) {
			incrementor = -255;
		}
		//Serial.println(incrementor);
		powerLEDPWM = abs(incrementor);
		analogWrite(powerLED, powerLEDPWM);
	}
}

void handleButts() {
	for (byte i = 0; i < 5; i++) {
		if (buttBangs[i]) {
			if (shiftButtState) {
				currentChannel = i;
				channelOffset = currentChannel * 5;
			}
			else {
				if (USBSwitchState) {
					sendUSBMIDInoteOn(1,i+channelOffset,127);
				}
				else {
					HandleNoteOn(i + channelOffset);
				}
			}
			
		}
		else if (buttBongs[i]) {
			HandleNoteOff(i + channelOffset);
			sendUSBMIDInoteOff(1, i+channelOffset, 0);
		}
	}

}


void usbMidiProcessing() {

	while (MIDIUSB.available() > 0) {
		MIDIEvent e = MIDIUSB.read();

		// IF NOTE ON WITH VELOCITY GREATER THAN ZERO
		if ((e.type == NOTEON) && (e.m3 > 0)) {
			//jitterfreq = 0;
			HandleNoteOn((e.m2 % 25));
		}
		// IF USB NOTE OFF
		else if (e.type == NOTEOFF) {
			HandleNoteOff((e.m2 % 25));
		}
		// IF NOTE ON W/ ZERO VELOCITY
		else if ((e.type == NOTEON) && (e.m3 == 0)) {
			HandleNoteOff((e.m2 % 25));
		}

	}

}

void HandleNoteOn(byte note) {  //noteOns are sent as positive values
	onTimers[note] = millis();
	noteIsOn[note] = true;
	Serial1.println(String(note));
	//Serial.println(String(note));
}

void HandleNoteOff(byte note) { //noteOffs are sent as negative values
	byte offsetNote = note + 1;
	Serial1.println(String(offsetNote*-1));
	noteIsOn[note] = false;
	//Serial.println(String(offsetNote*-1));
}
