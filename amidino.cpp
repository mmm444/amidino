#include <amidino.h>

/// @file amidino.cpp

// off, on, nat, cc, pc, cp, pw
static const uint8_t VOICE_MSG_LENGTHS[] = {2, 2, 2, 2, 1, 1, 2, 0};

MidiParser::MidiParser(const uint8_t channel) {
  for (int i = 0; i < 8; i++) {
    mRtHandlers[i] = 0;
    mVoiceHandlers[i] = 0;
  }
  mCmd = 0;
  mChannel = channel;
}

void MidiParser::setChannel(const uint8_t channel) {
  mChannel = channel;
  mCmd = 0;
}

void MidiParser::parse(const uint8_t in) {
  // is it a system message?
  if ((in & 0xf0) == 0xf0) {
    if ((in & 0xf8) == 0xf8) { // system realtime
      const uint8_t idx = in & 0x07;
      if (mRtHandlers[idx]) {
        mRtHandlers[idx]();
        return;
      }
    } else { // system common
      mCmd = 0; // jsut break the running status
    }
  } else if (in & 0x80) { // is it a voice message?
    // is it for us?
    if (mChannel == MIDI_OMNI || (in & 0x0f) == mChannel) {
      mCmd = in;
      mParsCnt = VOICE_MSG_LENGTHS[(in >> 4) - 8];
    } else {
      mCmd = 0; // break the running status
    }
  } else if (mCmd) { // parameter byte for a known command
    mParsCnt--;
    mPars[mParsCnt] = in;
  }
  
  if (mParsCnt == 0 && mCmd) { // all parameters received
    // only voice messages are handled here
    const uint8_t idx = (mCmd >> 4) - 8;
    if (mVoiceHandlers[idx]) {
      if (VOICE_MSG_LENGTHS[idx] == 1) {
        ((midiMsgHandler2) mVoiceHandlers[idx])(mCmd & 0x0f, mPars[0]);
      } else { // 2
        ((midiMsgHandler3) mVoiceHandlers[idx])(mCmd & 0x0f, mPars[1], mPars[0]);
      }
    }
    mParsCnt = VOICE_MSG_LENGTHS[idx];
  }
}

void FakePoly::noteOn(uint8_t note, uint8_t vel) {
  if (vel > 0) { // note on
    if (mSize < N) { // have space in stack?
      mNoteStack[mSize] = note;
      mVelStack[mSize] = vel;
      mSize++;
      
      mHandler(note, vel);
    }
  } else { // note off
    // look for note on the stack
    uint8_t j = N;
    for (uint8_t i = 0; i < mSize; ++i) {
      if (mNoteStack[i] == note) {
        j = i;
        break;
      }
    }
    if (j < N) { // found?
      // remove it from the stack
      mSize--;
      for (uint8_t i = j; i < mSize; ++i) {
        mNoteStack[i] = mNoteStack[i+1];
        mVelStack[i] = mVelStack[i+1];
      }

      if (mSize == 0) { // no notes on stack -> silence
        mHandler(1, 0);
      } else {
        if (j == mSize) { // last note removed
          mHandler(mNoteStack[j - 1], mVelStack[j - 1]);
        }
      }
    }
  }
}

void FakePoly::reset() {
  mSize = 0;
  mHandler(1, 0);
}
