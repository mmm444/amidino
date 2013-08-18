#ifndef AMIDINO_H
#define AMIDINO_H

/**
 * @file amidino.h
 * A library for processing MIDI input on Arduino.
 */

#include <inttypes.h>

/// Handler type for messages with no parametrs.
typedef void (*midiMsgHandler0)(void);
/// Handler type for messages with 1 parametr.
typedef void (*midiMsgHandler1)(uint8_t);
/// Handler type for messages with 2 parametrs.
typedef void (*midiMsgHandler2)(uint8_t, uint8_t);
/// Handler type for messages with 3 parametrs.
typedef void (*midiMsgHandler3)(uint8_t, uint8_t, uint8_t);

/// Special input channel meaning _all channels_
#define MIDI_OMNI 16

/**
 * Parser of MIDI byte stream.
 *
 * The parser accepts the incoming bytes of a MIDI stream using the #parse(uint8_t) method and
 * uses callbacks (handlers) for dispatching the parsed messages to the user. Not all handlers
 * have to be registered.
 *
 * The parser supports all voice and system realtime messages. System common messages are not supported and
 * are correctly ignored. Running status is supported.
 * 
 * The parser can be set to process voice messages on a specific channel only.
 */
class MidiParser {
public:
  /**
   * Constructor.
   * @param channel the MIDI channel (zero based) to restrict parsing for or MIDI_OMNI for parsing on all channels
   */
  MidiParser(const uint8_t channel = MIDI_OMNI);
  
  /**
   * Set the MIDI channel to restrict parsing for.
   * The current partially received message is discarded.
   *
   * @param channel the MIDI channel (zero based) to restrict parsing for or MIDI_OMNI for parsing on all channels
   */
  void setChannel(const uint8_t channel);

  /**
   * Get the MIDI channel the parser is restricted to.
   * @return channel the MIDI channel (zero based) or {@link MIDI_OMNI}
   */
  uint8_t getChannel() const { return mChannel; }
  
  /**
   * Parse a byte from an input. As a result some of the handlers may be called.
   */
  void parse(const uint8_t in);

  /**
   * Reset the parser. The current partially received message and running status are discarded.
   */
  void reset() { mCmd = 0; }
  
  /**
   * @name Voice message handlers
   * Use the following handler prototypes:
   * ~~~{.c}
   * void handleNoteOff(uint8_t ch, uint8_t note, uint8_t vel);
   * void handleNoteOn(uint8_t ch, uint8_t note, uint8_t vel);
   * void handleAfterTouch(uint8_t ch, uint8_t note, uint8_t pres);
   * void handleControlChange(uint8_t ch, uint8_t ctl, uint8_t val);
   * void handleProgramChange(uint8_t ch, uint8_t prog);
   * void handleChannelPressure(uint8_t ch, uint8_t pres);
   * void handlePitchWheel(uint8_t ch, uint8_t val1, uint8_t val2);
   * ~~~
   * The handlers can also be NULL.
   */
  //@{
  void setNoteOffHandler(midiMsgHandler3 handler) { mVoiceHandlers[0] = (void*) handler; }
  void setNoteOnHandler(midiMsgHandler3 handler) { mVoiceHandlers[1] = (void*) handler; }
  void setAfterTouchHandler(midiMsgHandler3 handler) { mVoiceHandlers[2] = (void*) handler; }
  void setControlChangeHandler(midiMsgHandler3 handler) { mVoiceHandlers[3] = (void*) handler; }
  void setProgramChangeHandler(midiMsgHandler2 handler) { mVoiceHandlers[4] = (void*) handler; }
  void setChannelPressureHandler(midiMsgHandler2 handler) { mVoiceHandlers[5] = (void*) handler; }
  /// @see #convertPitch(uint8_t val1, uint8_t val2)
  void setPitchWheelHandler(midiMsgHandler3 handler) { mVoiceHandlers[6] = (void*) handler; }
  //@}
  
  /** @name Realtime message handlers
   * All the handlers have 0 parameters so use a handler like this:
   * ~~~{.c}
   * void handleStart();
   * ~~~
   * The handlers can also be NULL.
   */
  //@{
  void setClockHandler(midiMsgHandler0 handler) { mRtHandlers[0] = handler; }
  void setTickHandler(midiMsgHandler0 handler) { mRtHandlers[1] = handler; }
  void setStartHandler(midiMsgHandler0 handler) { mRtHandlers[2] = handler; }
  void setContinueHandler(midiMsgHandler0 handler) { mRtHandlers[3] = handler; }
  void setStopHandler(midiMsgHandler0 handler) { mRtHandlers[4] = handler; }
  void setActiveSenseHandler(midiMsgHandler0 handler) { mRtHandlers[6] = handler; }
  void setResetHandler(midiMsgHandler0 handler) { mRtHandlers[7] = handler; }
  //@}

  /** 
   * Utility method for pitch wheel handlers. It converts the handler `val1` and `val2` byte parameters
   * into a combined vaule between -8192 and 8191 where 0 means no pitch shift.
   * @see setPitchWheelHandler()
   */
  static int16_t convertPitch(uint8_t val1, uint8_t val2) { return ((int16_t) val1 | (val2 << 7)) - 0x2000; } 
protected:
  uint8_t mChannel; ///< channel to receive on (0-15) or MIDI_OMNI to receive on all channels

  uint8_t mCmd; ///< last command received
  uint8_t mParsCnt; ///< number of parameters remaining to parse
  uint8_t mPars[2]; ///< parameter values

  midiMsgHandler0 mRtHandlers[8];
  void* mVoiceHandlers[8];
};

/**
 * Encapsulation of the "the still-held last-pressed note plays now" algorithm.
 * Allows a monophonic instrument to receive MIDI messages from a polyphonic controller.
 *
 * You should register one callback that tells you what note at what velocity
 * should be playing now. Then you have to pass the the Note On and Note Off messages
 * from the MIDI parser to the noteOn() method.
 */
class FakePoly {
public:
  FakePoly() : mSize(0) {}

  /**
   * Register the handler. The handler receives note (midi) and its velocity
   * that should play at the moment.
   */
  void setHandler(midiMsgHandler2 handler) { mHandler = handler; }

  /**
   * Notify FakePoly about the incomming Note On or Note Off message. Note Offs
   * should pass `vel=0`. This method invokes the callback when appropriate.
   */
  void noteOn(uint8_t note, uint8_t vel);

  /**
   * Reset the state of fake poly. The handler is notiefied that it should stop playing
   * any sound.
   */
  void reset();
protected:
  static const uint8_t N = 16;

  midiMsgHandler2 mHandler;
  uint8_t mNoteStack[N];
  uint8_t mVelStack[N];
  uint8_t mSize;
};

#endif