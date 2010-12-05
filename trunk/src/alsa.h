/**
 * This file is part of repulse.
 * (c) 2010 and onwards Juan Carlos Rodrigo Garcia.
 *
 * repulse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * repulse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with repulse.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ALSA_H_
#define ALSA_H_

#include <cassert>
#include <alsa/asoundlib.h>

namespace alsa {

class Sequencer {
	snd_seq_t* handle;
public:
	Sequencer( const std::string& name ) {
		assert( snd_seq_open( &handle, "default", SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK ) >= 0 );
		assert( snd_seq_set_client_name( handle, name.c_str() ) == 0 );
	}
	virtual ~Sequencer() {
		snd_seq_close( handle );
	}
	snd_seq_t* get_handle() const { return handle; }
};

class MidiPort {
	Sequencer* sequencer;
protected:
	Sequencer* get_sequencer() const { return sequencer; }
public:
	MidiPort( Sequencer* sequencer ) : sequencer( sequencer ) {}
	virtual ~MidiPort() {}
};

class MidiInputListener;

typedef std::set<MidiInputListener*> MidiInputListenerSet;

class IMidiInput : public MidiPort {
public:
	IMidiInput( Sequencer* sequencer ) : MidiPort( sequencer ) {}
	~IMidiInput() {}
	virtual void add_listener( MidiInputListener* listener ) {}
	virtual void remove_listener( MidiInputListener* listener ) {}
};

class MidiInputListener {
public:
	MidiInputListener() {}
	virtual ~MidiInputListener() {}
	virtual void on_note_off( IMidiInput* input, const midi::NoteOff* event ) {}
	virtual void on_note_on( IMidiInput* input, const midi::NoteOn* event ) {}
	virtual void on_after_touch( IMidiInput* input, const midi::AfterTouch* event ) {}
	virtual void on_controller( IMidiInput* input, const midi::Controller* event ) {}
	virtual void on_program_change( IMidiInput* input, const midi::ProgramChange* event ) {}
	virtual void on_channel_pressure( IMidiInput* input, const midi::ChannelPressure* event ) {}
	virtual void on_pitch_wheel( IMidiInput* input, const midi::PitchWheel* event ) {}
};

class MidiInput : public IMidiInput {
	MidiInputListenerSet listeners;
	int port_id;
protected:
	void fire_note_off( const midi::NoteOff* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_note_off( this, event );
		}
	}
	void fire_note_on( const midi::NoteOn* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_note_on( this, event );
		}
	}
	void fire_after_touch( const midi::AfterTouch* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_after_touch( this, event );
		}
	}
	void fire_controller( const midi::Controller* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_controller( this, event );
		}
	}
	void fire_program_change( const midi::ProgramChange* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_program_change( this, event );
		}
	}
	void fire_channel_pressure( const midi::ChannelPressure* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_channel_pressure( this, event );
		}
	}
	void fire_pitch_wheel( const midi::PitchWheel* event ) {
		MidiInputListenerSet::const_iterator it;
		for ( it = listeners.begin(); it != listeners.end(); ++it ) {
			(*it)->on_pitch_wheel( this, event );
		}
	}
public:
	MidiInput( Sequencer* sequencer, const std::string& name ) :
		IMidiInput( sequencer ),
		port_id( snd_seq_create_simple_port(
				get_sequencer()->get_handle(), name.c_str(),
				SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE | SND_SEQ_PORT_CAP_READ,
				SND_SEQ_PORT_TYPE_APPLICATION ) ) {
		assert( port_id >= 0 );
	}
    ~MidiInput() {
    	snd_seq_delete_simple_port( get_sequencer()->get_handle(), port_id );
    }
	void add_listener( MidiInputListener* listener ) {
		listeners.insert( listener );
	}
	void remove_listener( MidiInputListener* listener ) {
		listeners.erase( listener );
	}
    void next() {
    	midi::Event event;
    	while ( snd_seq_event_input_pending( get_sequencer()->get_handle(), 1 ) > 0 ) {
    		snd_seq_event_input( get_sequencer()->get_handle(), event.get_event_ex() );
			switch ( event.get_type() ) {
			case midi::Event::NOTE_OFF:
				fire_note_off( event.as_note_off() );
				break;
			case midi::Event::NOTE_ON:
				fire_note_on( event.as_note_on() );
				break;
			case midi::Event::AFTER_TOUCH:
				fire_after_touch( event.as_after_touch() );
				break;
			case midi::Event::CONTROLLER:
				fire_controller( event.as_controller() );
				break;
			case midi::Event::PROGRAM_CHANGE:
				fire_program_change( event.as_program_change() );
				break;
			case midi::Event::CHANNEL_PRESSURE:
				fire_channel_pressure( event.as_channel_pressure() );
				break;
			case midi::Event::PITCH_WHEEL:
				fire_pitch_wheel( event.as_pitch_wheel() );
				break;
			}
			snd_seq_free_event( event.get_event() );
    	}
    }
};

}

#endif /* ALSA_H_ */
