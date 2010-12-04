/**
 * This file is part of rePulse.
 * (c) 2010 and onwards Juan Carlos Rodrigo Garcia.
 *
 * rePulse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rePulse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rePulse.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIDI_H_
#define MIDI_H_

#include <set>
#include <vector>
#include <cstring>
#include <alsa/asoundlib.h>
#include "util.h"

namespace midi {

static unsigned char  MIDDLE_C    =  60;
static unsigned char  MIN_VALUE   =  0;
static unsigned char  MAX_VALUE   =  0x7F;
static unsigned char  MIN_CHANNEL =  0;
static unsigned char  MAX_CHANNEL =  0xF;
static          short MIN_PITCH   = -8192;
static          short MAX_PITCH   =  8191;

class NoteOff;
class NoteOn;
class AfterTouch;
class Controller;
class ProgramChange;
class ChannelPressure;
class PitchWheel;

class Event {
public:
	enum Type {
		NOTE_OFF = SND_SEQ_EVENT_NOTEOFF,
		NOTE_ON = SND_SEQ_EVENT_NOTEON,
		AFTER_TOUCH = SND_SEQ_EVENT_KEYPRESS,
		CONTROLLER = SND_SEQ_EVENT_CONTROLLER,
		PROGRAM_CHANGE = SND_SEQ_EVENT_PGMCHANGE,
		CHANNEL_PRESSURE = SND_SEQ_EVENT_CHANPRESS,
		PITCH_WHEEL = SND_SEQ_EVENT_PITCHBEND
	};
private:
	snd_seq_event_t* event;
public:
	Event() {}
	virtual ~Event() {}
	snd_seq_event_t* get_event() const { return event; }
	snd_seq_event_t** get_event_ex() { return &event; }
	Type get_type() const { return (Type)event->type; }
	NoteOff* as_note_off() const { return (NoteOff*)this; }
	NoteOn* as_note_on() const { return (NoteOn*)this; }
	AfterTouch* as_after_touch() const { return (AfterTouch*)this; }
	Controller* as_controller() const { return (Controller*)this; }
	ProgramChange* as_program_change() const { return (ProgramChange*)this; }
	ChannelPressure* as_channel_pressure() const { return (ChannelPressure*)this; }
	PitchWheel* as_pitch_wheel() const { return (PitchWheel*)this; }
};

class VoiceEvent : public Event {
protected:
	VoiceEvent() : Event() {}
public:
	~VoiceEvent() {}
	void set_channel( unsigned char channel ) {
		get_event()->data.control.channel = util::adjust_value( channel, MIN_CHANNEL, MAX_CHANNEL );
	}
	unsigned char get_channel() const {
		return get_event()->data.control.channel;
	}
};

class KeyEvent : public VoiceEvent {
protected:
	KeyEvent() : VoiceEvent() {}
public:
	~KeyEvent() {}
	void set_note( unsigned char note ) {
		get_event()->data.note.note = util::adjust_value( note, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_note() const {
		return get_event()->data.note.note;
	}
};

class NoteOff : public KeyEvent {
public:
	NoteOff() : KeyEvent() {}
	~NoteOff() {}
	void set_velocity( unsigned char velocity ) {
		get_event()->data.note.velocity = util::adjust_value( velocity, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_velocity() const {
		return get_event()->data.note.velocity;
	}
	bool is_no_velocity() const {
		return get_event()->data.note.velocity == 0x40;
	}
};

class NoteOn : public KeyEvent {
public:
	NoteOn() : KeyEvent() {}
	~NoteOn() {}
	void set_velocity( unsigned char velocity ) {
		get_event()->data.note.velocity = util::adjust_value( velocity, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_velocity() const {
		return get_event()->data.note.velocity;
	}
	bool is_no_velocity() const {
		return get_event()->data.note.velocity == 0x40;
	}
	bool is_note_off() const {
		return get_event()->data.note.velocity == 0;
	}
};

class AfterTouch : public KeyEvent {
public:
	AfterTouch() : KeyEvent() {}
	~AfterTouch() {}
	void set_value( unsigned char value ) {
		get_event()->data.note.velocity = util::adjust_value( value, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_value() const {
		return get_event()->data.note.velocity;
	}
};

class Controller : public VoiceEvent {
public:
	Controller() : VoiceEvent() {}
	~Controller() {}
	void set_controller( unsigned char controller ) {
		get_event()->data.control.param = util::adjust_value( controller, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_controller() const {
		return get_event()->data.control.param;
	}
	void set_value( unsigned char value ) {
		get_event()->data.control.value = util::adjust_value( value, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_value() const {
		return get_event()->data.control.value;
	}
};

class ProgramChange : public VoiceEvent {
public:
	ProgramChange() : VoiceEvent() {}
	~ProgramChange() {}
	void set_program( unsigned char program ) {
		get_event()->data.control.param = util::adjust_value( program, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_program() const {
		return get_event()->data.control.param;
	}
};

class ChannelPressure : public VoiceEvent {
public:
	ChannelPressure() : VoiceEvent() {}
	~ChannelPressure() {}
	void set_pressure( unsigned char pressure ) {
		get_event()->data.control.param = util::adjust_value( pressure, MIN_VALUE, MAX_VALUE );
	}
	unsigned char get_pressure() const {
		return get_event()->data.control.param;
	}
};

class PitchWheel : public VoiceEvent {
public:
	PitchWheel() : VoiceEvent() {}
	~PitchWheel() {}
	void set_pitch( const int& pitch ) {
		get_event()->data.control.value = util::adjust_value( pitch, MIN_PITCH, MAX_PITCH );
	}
	int get_pitch() const {
		return get_event()->data.control.value;
	}
};

} // namespace midi

#endif /* MIDI_H_ */
