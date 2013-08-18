#include <amidino.h>
#include <iostream>
#include <fstream>

using namespace std;

void handleNoteOn(uint8_t ch, uint8_t note, uint8_t vel) {
	cout << "Note on ch=" << (int) ch << " note=" << (int) note << " vel=" << (int) vel << "\n";
}

void handleNoteOff(uint8_t ch, uint8_t note, uint8_t vel) {
	cout << "Note off ch=" << (int) ch << " note=" << (int) note << " vel=" << (int) vel << "\n";
}

void handleCc(uint8_t ch, uint8_t ctl, uint8_t val) {
	cout << "Control change ch=" << (int) ch << " ctl=" << (int) ctl << " val=" << (int) val << "\n";
}

void handleChannelPressure(uint8_t ch, uint8_t pres) {
	cout << "Channel pressure ch=" << (int) ch << " pres=" << (int) pres << "\n";
}

void handlePitchWheel(uint8_t ch, uint8_t val1, uint8_t val2) {
	cout << "Pitch wheel ch=" << (int) ch << " val=" << MidiParser::convertPitch(val1, val2) << "\n";
}

void handleStart() {
	cout << "RT Start\n"; 
}

void handleStop() {
	cout << "RT Stop\n"; 
}

int main(int argc, char** argv) {
	if (argc != 2) return 1;

	MidiParser m(MIDI_OMNI);
	m.setNoteOnHandler(handleNoteOn);
	m.setNoteOffHandler(handleNoteOff);
//	m.setControlChangeHandler(handleCc);
//	m.setChannelPressureHandler(handleChannelPressure);
	m.setPitchWheelHandler(handlePitchWheel);

	m.setStartHandler(handleStart);
	m.setStopHandler(handleStop);

	ifstream fs(argv[1]);
	int i;
	while (true) {
		fs >> i;
		if (!fs) break;
		m.parse((uint8_t) i);
	}

	return !fs.eof();
}
