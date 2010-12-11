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

#ifndef REPULSE_H_
#define REPULSE_H_

#include <set>
#include <sstream>
#include "jack.h"
#include "alsa.h"
#include "midi.h"
#include "envelope.h"
#include "modulation.h"
#include "filtering.h"
#include "util.h"
#include "persistence.h"

namespace repulse {

class IEngine;

class EngineListener {
public:
	EngineListener() {}
	virtual ~EngineListener() {}
	virtual void on_stretch( IEngine* engine, const util::floating_t& stretch, const bool& fire = true ) {}
	virtual void on_volume( IEngine* engine, const util::floating_t& volume, const bool& fire = true ) {}
	virtual void on_transpose( IEngine* engine, const util::floating_t& transpose, const bool& fire = true ) {}
	virtual void on_note_map( IEngine* engine, const util::NoteMapType& note_map, const bool& fire = true ) {}
	virtual void on_base_note( IEngine* engine, const unsigned char& base_note, const bool& fire = true ) {}
};

typedef std::set<EngineListener*> EngineListenerSet;

class Sound;

class IEngine {
public:
	IEngine() {}
	virtual ~IEngine() {}
	virtual jack::Client* get_client() const { return 0; }
    virtual const std::string& get_name() const { return util::BLANK; }
	virtual void add_listener( EngineListener* engine_listener ) {}
	virtual void remove_listener( EngineListener* engine_listener ) {}
	virtual void add_midi_listener( alsa::MidiInputListener* midi_listener ) {}
	virtual void remove_midi_listener( alsa::MidiInputListener* midi_listener ) {}
    virtual void set_transpose_wheel( const util::floating_t& transpose_wheel, const bool& fire = true ) {}
    virtual const util::floating_t& get_transpose_wheel() const { return util::ZERO; }
    virtual void set_transpose_offset( const util::floating_t& transpose_offset, const bool& fire = true ) {}
    virtual const util::floating_t& get_transpose_offset() const { return util::ZERO; }
    virtual const util::floating_t get_transpose() const { return util::ZERO; }
    virtual void set_stretch_wheel( const util::floating_t& stretch_wheel, const bool& fire = true ) {}
    virtual const util::floating_t& get_stretch_wheel() const { return util::ZERO; }
    virtual void set_stretch_offset( const util::floating_t& stretch_offset, const bool& fire = true ) {}
    virtual const util::floating_t& get_stretch_offset() const { return util::ZERO; }
    virtual const util::floating_t get_stretch() const { return util::ZERO; }
    virtual void set_volume( const util::floating_t& volume, const bool& fire = true ) {}
	virtual const util::floating_t& get_volume() const { return util::ZERO; }
    virtual void set_linked( const bool& linked, const bool& fire = true ) {}
    virtual const bool is_linked() const { return false; }
    virtual Sound** get_sounds() { return 0; }
	virtual void solo( const util::SoundIdentifier& id, const bool& active, const bool& fire = true ) {}
	virtual bool is_soloed() const { return false; }
	virtual bool is_omni() const { return false; }
    virtual const unsigned char& get_base_channel() const { return util::MIDI_CONT_BASE_CHANNEL; }
    virtual void set_base_channel( const unsigned char& base_channel ) {}
    virtual const unsigned char& get_base_note() const { return util::MIDI_CONT_BASE_CHANNEL; }
    virtual void set_base_note( const unsigned char& base_note ) {};
    virtual const util::NoteMapType& get_note_map() const { return util::NOTE_MAP_DEF_TYPE; }
    virtual void set_note_map( const util::NoteMapType& note_map, const bool& fire = true ) {}
};

class Sound :
	public filtering::Filter,
	public jack::Listener,
	public alsa::MidiInputListener,
	public EngineListener  {
	util::MidiNoteOffset mno;
	util::MidiControllerOffset mco;
	jack::sample_t* buffer;
    util::floating_t sample;
	IEngine* engine;
    jack::AudioOutput* output;
    filtering::Wave* wave;
    filtering::TimeStretch* time_stretch;
    filtering::Tuner* tuner;
    filtering::OverDrive* over_drive;
    filtering::Frequency* frequency;
    envelope::Machine* envelope;
    filtering::Gain* gain;
    filtering::Panner panner;
    util::floating_t panning;
	util::floating_t decay_time;
    util::floating_t stretch;
    util::floating_t volume;
    util::floating_t transpose;
    util::floating_t filter_frequency;
    util::floating_t filter_resonance;
    util::SoundIdentifier id;
    bool muted;
    bool soloed;
    bool playing;
    modulation::Velocity volume_modulation;
    modulation::Velocity stretch_modulation;
    modulation::VelocityRandom transpose_modulation;
    modulation::VelocityRandom filter_frequency_modulation;
    modulation::VelocityRandom panning_modulation;
public:
    Sound( IEngine* engine, const std::string& name, const util::SoundIdentifier& id ) :
    	filtering::Filter( engine->get_client() ),
    	jack::Listener(),
    	alsa::MidiInputListener(),
       	EngineListener(),
    	buffer( 0 ),
       	sample( 0 ),
       	engine( engine ),
    	output( new jack::AudioOutput( engine->get_client(), name ) ),
    	wave( new filtering::Wave( engine->get_client() ) ),
    	time_stretch( new filtering::TimeStretch( engine->get_client(), wave ) ),
    	tuner( new filtering::Tuner( engine->get_client(), time_stretch ) ),
    	over_drive( new filtering::OverDrive( engine->get_client() ) ),
    	frequency( new filtering::Frequency( engine->get_client() ) ),
    	envelope( new envelope::Machine( engine->get_client() ) ),
    	gain( new filtering::Gain( engine->get_client() ) ),
    	panning( filtering::PANNER_DEF_PANNING ),
    	decay_time( envelope::DEF_DECAY ),
    	stretch( filtering::TIME_STRETCH_DEF_STRETCH ),
    	volume( filtering::GAIN_DEF_VOLUME ),
    	transpose( filtering::TUNER_DEF_TRANSPOSE ),
    	filter_frequency( filtering::FREQUENCY_DEF_FREQUENCY ),
    	filter_resonance( filtering::FREQUENCY_DEF_RESONANCE ),
    	id( id ),
    	muted( false ),
    	soloed( false ),
    	playing( true ) {
    	mno.set_id( id );
    	mno.set_base_note( engine->get_base_note() );
    	mno.set_note_map( engine->get_note_map() );
    	mco.set_id( id );
        transpose_modulation.set_range(
        		filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE );
        filter_frequency_modulation.set_range(
        		filtering::FREQUENCY_MIN_FREQUENCY, filtering::FREQUENCY_MAX_FREQUENCY( engine->get_client() ) );
        panning_modulation.set_range(
        		filtering::PANNER_MIN_PANNING, filtering::PANNER_MAX_PANNING );
    	engine->add_listener( this );
    	engine->add_midi_listener( this );
    	engine->get_client()->add_jack_listener( this );
    }
    ~Sound() {
        engine->get_client()->remove_jack_listener( this );
    	engine->remove_midi_listener( this );
    	engine->remove_listener( this );
    	delete output;
        delete wave;
        delete tuner;
        delete over_drive;
        delete frequency;
        delete envelope;
        delete gain;
    }
    void save_preset( persistence::Sound& sound ) const {
	    sound.set_start( get_start_time() );
	    sound.set_start_soft( is_start_soft() );
	    sound.set_transpose( get_transpose() );
	    sound.set_transpose_velocity( get_transpose_velocity() );
	    sound.set_transpose_random( get_transpose_random() );
	    sound.set_stretch( get_stretch() );
	    sound.set_stretch_velocity( get_stretch_velocity() );
	    sound.set_stretch_type( get_stretch_type() );
	    sound.set_over_drive( get_over_drive_drive() );
	    sound.set_over_drive_active( is_over_drive_active() );
	    sound.set_filter_frequency( get_filter_frequency() );
	    sound.set_filter_active( is_filter_active() );
	    sound.set_filter_type( get_filter_type() );
	    sound.set_filter_velocity( get_filter_frequency_velocity() );
	    sound.set_filter_random( get_filter_frequency_random() );
	    sound.set_filter_resonance( get_filter_resonance() );
	    sound.set_decay( get_decay_time() );
	    sound.set_decay_type( get_decay_type() );
	    sound.set_panning( get_panning() );
	    sound.set_panning_velocity( get_panning_velocity() );
	    sound.set_panning_random( get_panning_random() );
	    sound.set_volume( get_volume() );
	    sound.set_volume_velocity( get_volume_velocity() );
	    sound.set_muted( is_muted() );
	    sound.set_soloed( is_soloed() );
    }
    void recall_preset( persistence::Sound& sound, const bool& fire = false ) {
	    set_start_time( sound.get_start(), fire );
	    set_start_soft( sound.is_start_soft(), fire );
	    set_transpose( sound.get_transpose(), fire );
	    set_transpose_velocity( sound.get_transpose_velocity(), fire );
	    set_transpose_random( sound.get_transpose_random(), fire );
	    set_stretch( sound.get_stretch(), fire );
	    set_stretch_velocity( sound.get_stretch_velocity(), fire );
	    set_stretch_type( sound.get_stretch_type(), fire );
	    set_over_drive_drive( sound.get_over_drive(), fire );
	    set_over_drive_active( sound.is_over_drive_active(), fire );
	    set_filter_frequency( sound.get_filter_frequency(), fire );
	    set_filter_active( sound.is_filter_active(), fire );
	    set_filter_type( sound.get_filter_type(), fire );
	    set_filter_frequency_velocity( sound.get_filter_velocity(), fire );
	    set_filter_frequency_random( sound.get_filter_random(), fire );
	    set_filter_resonance( sound.get_filter_resonance(), fire );
	    set_decay_time( sound.get_decay(), fire );
	    set_decay_type( sound.get_decay_type(), fire );
	    set_panning( sound.get_panning(), fire );
	    set_panning_velocity( sound.get_panning_velocity(), fire );
	    set_panning_random( sound.get_panning_random(), fire );
	    set_volume( sound.get_volume(), fire );
	    set_volume_velocity( sound.get_volume_velocity(), fire );
	    set_muted( sound.is_muted(), fire );
	    set_soloed( sound.is_soloed(), fire );
    }
    bool is_playing() const {
    	return playing;
    }
    IEngine* get_engine() const {
    	return engine;
    }
    const util::SoundIdentifier& get_id() const {
    	return id;
    }
    void set_muted( const bool& muted, const bool& fire = true ) {
    	this->muted = muted;
    }
    const bool& is_muted() const {
    	return muted;
    }
    void set_soloed( const bool& soloed, const bool& fire = true ) {
    	this->soloed = soloed;
    }
    const bool& is_soloed() const {
    	return soloed;
    }
	void set_volume_velocity( const util::floating_t& volume_velocity, const bool& fire = true ) {
		volume_modulation.set_velocity( volume_velocity );
	}
	const util::floating_t& get_volume_velocity() const {
		return volume_modulation.get_velocity();
	}
	void set_stretch_velocity( const util::floating_t& stretch_velocity, const bool& fire = true ) {
		stretch_modulation.set_velocity( stretch_velocity );
	}
	const util::floating_t& get_stretch_velocity() const {
		return stretch_modulation.get_velocity();
	}
	void set_transpose_velocity( const util::floating_t& transpose_velocity, const bool& fire = true ) {
		transpose_modulation.set_velocity( transpose_velocity );
	}
	const util::floating_t& get_transpose_velocity() const {
		return transpose_modulation.get_velocity();
	}
	void set_transpose_random( const util::floating_t& transpose_random, const bool& fire = true ) {
		transpose_modulation.set_random( transpose_random );
	}
	const util::floating_t& get_transpose_random() const {
		return transpose_modulation.get_random();
	}
	void set_filter_frequency_velocity( const util::floating_t& filter_frequency_velocity, const bool& fire = true ) {
		filter_frequency_modulation.set_velocity( filter_frequency_velocity );
	}
	const util::floating_t& get_filter_frequency_velocity() const {
		return filter_frequency_modulation.get_velocity();
	}
	void set_filter_frequency_random( const util::floating_t& filter_frequency_random, const bool& fire = true ) {
		filter_frequency_modulation.set_random( filter_frequency_random );
	}
	const util::floating_t& get_filter_frequency_random() const {
		return filter_frequency_modulation.get_random();
	}
	void set_panning_velocity( const util::floating_t& panning_velocity, const bool& fire = true ) {
		panning_modulation.set_velocity( panning_velocity );
	}
	const util::floating_t& get_panning_velocity() const {
		return panning_modulation.get_velocity();
	}
	void set_panning_random( const util::floating_t& panning_random, const bool& fire = true ) {
		panning_modulation.set_random( panning_random );
	}
	const util::floating_t& get_panning_random() const {
		return panning_modulation.get_random();
	}
    void acquire() {
    	buffer = output->get_buffer();
    }
    jack::sample_t* get_buffer() const {
    	return buffer;
    }
    void set_start_soft( const bool& start_soft, const bool& fire = true ) {
    	envelope->set_start_soft( start_soft );
    }
    const bool& is_start_soft() const {
    	return envelope->is_start_soft();
    }
    void set_decay_time( const util::floating_t& decay_time, const bool& fire = true ) {
    	envelope->set_decay_time( decay_time );
    }
    const util::floating_t& get_decay_time() const {
    	return envelope->get_decay_time();
    }
    void set_decay_type( const envelope::DecayType& decay_type, const bool& fire = true ) {
    	envelope->set_decay_type( decay_type );
    }
    const envelope::DecayType& get_decay_type() const {
    	return envelope->get_decay_type();
    }
    void set_over_drive_active( const bool& over_drive_active, const bool& fire = true ) {
    	over_drive->set_active( over_drive_active );
    }
    const bool& is_over_drive_active() const {
    	return over_drive->is_active();
    }
    void set_over_drive_drive( const util::floating_t& over_drive_drive, const bool& fire = true ) {
    	over_drive->set_drive( over_drive_drive );
    }
    const util::floating_t& get_over_drive_drive() const {
    	return over_drive->get_drive();
    }
    void set_filter_type( const filtering::FrequencyFilterType& filter_type, const bool& fire = true ) {
    	if ( filter_type != frequency->get_filter_type() ) {
    		frequency->set_filter_type( filter_type );
    		frequency->set_resonance( filter_resonance );
    		frequency->set_frequency( filter_frequency );
    	}
    }
    const filtering::FrequencyFilterType& get_filter_type() const {
    	return frequency->get_filter_type();
    }
	void set_filter_frequency( const util::floating_t& filter_frequency, const bool& fire = true ) {
		this->filter_frequency = util::adjust_value( filter_frequency,
				filtering::FREQUENCY_MIN_FREQUENCY, filtering::FREQUENCY_MAX_FREQUENCY( engine->get_client() ) );
		frequency->set_frequency( filter_frequency_modulation.modulate( this->filter_frequency ) );
	}
	const util::floating_t& get_filter_frequency() const {
		return filter_frequency;
	}
	void set_filter_resonance( const util::floating_t& filter_resonance, const bool& fire = true ) {
		this->filter_resonance = util::adjust_value( filter_resonance,
				filtering::FREQUENCY_MIN_RESONANCE, filtering::FREQUENCY_MAX_RESONANCE );
		frequency->set_resonance( filter_resonance );
	}
	const util::floating_t& get_filter_resonance() const {
		return frequency->get_resonance();
	}
	void set_filter_active( const bool& active, const bool& fire = true ) {
		frequency->set_active( active );
	}
	const bool& is_filter_active() const {
		return frequency->is_active();
	}
	void set_volume( const util::floating_t& volume, const bool& fire = true ) {
		this->volume = util::adjust_value( volume, filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME );
		gain->set_volume( volume_modulation.modulate( util::adjust_value( this->volume * engine->get_volume(),
				filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME ) ) );
	}
	const util::floating_t& get_volume() const {
		return volume;
	}
    void set_transpose( const util::floating_t& transpose, const bool& fire = true ) {
    	this->transpose = util::adjust_value( transpose, filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE );
    	tuner->set_transpose( transpose_modulation.modulate(
    			util::adjust_value( this->transpose + engine->get_transpose(),
    					filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE ) ) );
    }
    const util::floating_t& get_transpose() const {
    	return transpose;
    }
    void set_stretch( const util::floating_t& stretch, const bool& fire = true ) {
		this->stretch = util::adjust_value( stretch,
				filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH );
		time_stretch->set_stretch( stretch_modulation.modulate( this->stretch * engine->get_stretch() ) );
	}
	const util::floating_t& get_stretch() const {
		return stretch;
	}
	void set_stretch_type( const filtering::TimeStretchType& stretch_type, const bool& fire = true ) {
		time_stretch->set_type( stretch_type );
	}
	const filtering::TimeStretchType& get_stretch_type() const {
		return time_stretch->get_type();
	}
    void set_start_time( const util::floating_t& start_time, const bool& fire = true ) {
        wave->set_start_time( start_time );
    }
    const util::floating_t& get_start_time() const {
    	return wave->get_start_time();
    }
    void set_panning( const util::floating_t& panning, const bool& fire = true ) {
        this->panning = util::adjust_value( panning,
        		filtering::PANNER_MIN_PANNING, filtering::PANNER_MAX_PANNING );
        panner.set_panning( panning_modulation.modulate( this->panning ) );
    }
    const util::floating_t& get_panning() const {
    	return panning;
    }
	const util::floating_t& get_mix_left() const {
		return panner.get_mix_left();
    }
	const util::floating_t& get_mix_right() const {
		return panner.get_mix_right();
	}
    virtual void note_on( unsigned char velocity ) {
    	util::floating_t tmp;
    	// Modulate.
        volume_modulation.note_on( velocity );
        stretch_modulation.note_on( velocity );
        transpose_modulation.note_on( velocity );
        filter_frequency_modulation.note_on( velocity );
        panning_modulation.note_on( velocity );
        // Apply values.
        tmp = volume_modulation.modulate( util::adjust_value(
				volume * engine->get_volume(),
				filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME ) );
        if ( tmp != gain->get_volume() ) {
        	gain->set_volume( tmp );
        }
        tmp = stretch_modulation.modulate( util::adjust_value(
				stretch * engine->get_stretch(),
				filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH ) );
        if ( tmp != time_stretch->get_stretch() ) {
        	time_stretch->set_stretch( tmp );
        }
        tmp = transpose_modulation.modulate(
        		util::adjust_value( transpose + engine->get_transpose(),
        		filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE ) );
		if ( tmp != tuner->get_transpose() ) {
			tuner->set_transpose( tmp );
		}
        tmp = filter_frequency_modulation.modulate( filter_frequency );
        if ( tmp != frequency->get_frequency() ) {
        	frequency->set_frequency( tmp );
        }
        tmp = panning_modulation.modulate( panning );
        if ( tmp != panner.get_panning() ) {
        	panner.set_panning( tmp );
        }
		// Start the note.
    	tuner->reset();
    	envelope->note_on();
    	playing = true;
    }
    void note_off() {
    	envelope->note_off();
    }
    void silence() {
    	envelope->silence();
    }
    void filter( jack::sample_t* samples ) {
		if ( is_muted() || ( engine->is_soloed() && !is_soloed() )
				|| tuner->is_finished() || envelope->is_finished() ) {
			if ( playing ) {
				memset( samples, 0, get_client()->get_data_size() );
				playing = false;
			}
		} else {
			tuner->filter( samples );
			over_drive->filter( samples );
			frequency->filter( samples );
			envelope->filter( samples );
			gain->filter( samples );
		}
	}
    void set_file_name( const std::string& file_name ) {
    	wave->set_file_name( file_name );
    }
    const std::string& get_file_name() const {
    	return wave->get_file_name();
    }
    void load() {
    	wave->load();
    	tuner->reset();
    	envelope->silence();
    }
    ///////////////////////////////////////////////////////////////
    void on_sample_rate( jack::Client* client ) {
    	// TODO: implement
    	//load();
        //set_start_time( get_start_time(), false );
    }
    void on_buffer_size( jack::Client* client ) {
    	// TODO: implement
    }
    ///////////////////////////////////////////////////////////////
	void on_stretch( IEngine* engine, const util::floating_t& stretch, const bool& fire = true ) {
		time_stretch->set_stretch( stretch_modulation.modulate( util::adjust_value( this->stretch * stretch,
				filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH ) ) );
	}
	void on_volume( IEngine* engine, const util::floating_t& volume, const bool& fire = true ) {
		gain->set_volume( volume_modulation.modulate( util::adjust_value( this->volume * volume,
				filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME ) ) );
	}
	void on_transpose( IEngine* engine, const util::floating_t& transpose, const bool& fire = true ) {
		tuner->set_transpose( transpose_modulation.modulate( util::adjust_value(
				this->transpose + transpose, filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE ) ) );
	}
	virtual void on_note_map( IEngine* engine, const util::NoteMapType& note_map, const bool& fire = true ) {
		mno.set_note_map( note_map );
	}
	virtual void on_base_note( IEngine* engine, const unsigned char& base_note, const bool& fire = true ) {
		mno.set_base_note( base_note );
	}
	///////////////////////////////////////////////////////////////
	void on_note_off( alsa::IMidiInput* input, const midi::NoteOff* event ) {
		if ( engine->is_omni() || event->get_channel() == engine->get_base_channel() ) {
			if ( mno.test( event->get_note() ) ) {
				note_off();
			}
		}
	}
	void on_note_on( alsa::IMidiInput* input, const midi::NoteOn* event ) {
		if ( engine->is_omni() || event->get_channel() == engine->get_base_channel() ) {
			if ( mno.test( event->get_note() ) ) {
				if ( event->is_note_off() ) {
					note_off();
				} else {
					note_on( event->get_velocity() );
				}
			}
		}
	}
	void on_after_touch( alsa::IMidiInput* input, const midi::AfterTouch* event ) {
		// Not used
	}
	void on_controller( alsa::IMidiInput* input, const midi::Controller* event ) {
		if ( event->get_channel() == engine->get_base_channel() ) {
			if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_START_C10 ) ) {
				set_start_time( util::controller_to_decimal( event->get_value(),
						filtering::WAVE_MIN_START_TIME, filtering::WAVE_MAX_START_TIME ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_START_SOFT_C10 ) ) {
				set_start_soft( util::controller_to_bool( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_TRANSPOSE_C10 ) ) {
				set_transpose( util::controller_to_decimal_center( event->get_value(),
						filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_STRETCH_C10 ) ) {
				set_stretch( util::controller_to_decimal_center( event->get_value(),
						filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_OVER_DRIVE_C10 ) ) {
				set_over_drive_drive( util::controller_to_decimal( event->get_value(),
						filtering::OVER_DRIVE_MIN_DRIVE, filtering::OVER_DRIVE_MAX_DRIVE ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_FREQUENCY_C10 ) ) {
				set_filter_frequency( util::controller_to_decimal( event->get_value(),
						filtering::FREQUENCY_MIN_FREQUENCY,
						filtering::FREQUENCY_MAX_FREQUENCY( engine->get_client() ) ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_RESONANCE_C10 ) ) {
				set_filter_resonance( util::controller_to_decimal( event->get_value(),
						filtering::FREQUENCY_MIN_RESONANCE, filtering::FREQUENCY_MAX_RESONANCE ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_DECAY_C10 ) ) {
				set_decay_time( util::controller_to_decimal( event->get_value(),
						envelope::MIN_DECAY, envelope::MAX_DECAY ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_PANNING_C10 ) ) {
				set_panning( util::controller_to_decimal_center( event->get_value(),
						filtering::PANNER_MIN_PANNING, filtering::PANNER_MAX_PANNING ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_VOLUME_C10 ) ) {
				set_volume( util::controller_to_decimal_center( event->get_value(),
						filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_ACTIVE_C10 ) ) {
				set_filter_active( util::controller_to_bool( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_OVER_DRIVE_ACTIVE_C10 ) ) {
				set_over_drive_active( util::controller_to_bool( event->get_value() ), false );
			}

		} else if ( event->get_channel() == engine->get_base_channel() + 1 ) {
			if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_TRANSPOSE_VELOCITY_C11 ) ) {
				set_transpose_velocity( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_STRETCH_TYPE_C11 ) ) {
				set_stretch_type( filtering::TimeStretch::controller_to_stretch_type( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_TYPE_C11 ) ) {
				set_filter_type( filtering::Frequency::controller_to_filter_type( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_TRANSPOSE_RANDOM_C11 ) ) {
				set_transpose_random( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_STRETCH_VELOCITY_C11 ) ) {
				set_stretch_velocity( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_VELOCITY_C11 ) ) {
				set_filter_frequency_velocity( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_FILTER_RANDOM_C11 ) ) {
				set_filter_frequency_random( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_PANNING_VELOCITY_C11 ) ) {
				set_panning_velocity( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_PANNING_RANDOM_C11 ) ) {
				set_panning_random( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_VOLUME_VELOCITY_C11 ) ) {
				set_volume_velocity( util::controller_to_percentage( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_DECAY_TYPE_C11 ) ) {
				set_decay_type( envelope::Machine::controller_to_decay_type( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_MUTED_C11 ) ) {
				set_muted( util::controller_to_bool( event->get_value() ), false );
			} else if ( mco.test( event->get_controller(), util::MIDI_CONT_SOUND_SOLOED_C11 ) ) {
				engine->solo( get_id(), util::controller_to_bool( event->get_value() ), false );
			}
		}
	}
};

class LinkedSound : public Sound {
	Sound* linked;
public:
	LinkedSound( IEngine* engine, const std::string& name, const util::SoundIdentifier& id ) :
		Sound( engine, name, id ) {}
	~LinkedSound() {}
	void set_linked( Sound* linked ) {
		this->linked = linked;
	}
	Sound* get_linked() const {
		return linked;
	}
    void note_on( unsigned char velocity ) {
    	Sound::note_on( velocity );
    	if ( get_engine()->is_linked() ) {
    		get_linked()->silence();
    	}
    }
};

class Engine : public IEngine, public jack::Listener, public alsa::MidiInputListener {
	EngineListenerSet listeners;
	jack::Client* client;
    Sound* sounds[ util::MAX_SOUNDS ];
    persistence::RepulseDocument document;
    jack::AudioOutput* output_left;
    jack::AudioOutput* output_right;
    jack::sample_t* buffer_right;
    jack::sample_t* buffer_left;
    alsa::Sequencer* sequencer;
    alsa::MidiInput* midi_input;
    util::floating_t stretch_offset;
    util::floating_t stretch_wheel;
    util::floating_t volume;
    util::floating_t transpose_offset;
    util::floating_t transpose_wheel;
    //////////////////////////////////
    bool linked;
    size_t soloed;
    int selected_preset;
	unsigned char base_channel;
	unsigned char base_note;
	bool local_keyboard;
	bool alternate_wheel;
	bool omni;
	bool mono;
	util::NoteMapType note_map;
protected:
    void fire_stretch( const util::floating_t& stretch, const bool& fire = true ) {
    	EngineListenerSet::const_iterator it;
    	for ( it = listeners.begin(); it != listeners.end(); it++ ) {
    		(*it)->on_stretch( this, stretch, fire );
    	}
    }
    void fire_transpose( const util::floating_t& transpose, const bool& fire = true ) {
    	EngineListenerSet::const_iterator it;
    	for ( it = listeners.begin(); it != listeners.end(); it++ ) {
    		(*it)->on_transpose( this, transpose, fire );
    	}
    }
    void fire_volume( const util::floating_t& volume, const bool& fire = true ) {
    	EngineListenerSet::const_iterator it;
    	for ( it = listeners.begin(); it != listeners.end(); it++ ) {
    		(*it)->on_volume( this, volume, fire );
    	}
    }
    void fire_note_map( const util::NoteMapType& note_map, const bool& fire = true ) {
    	EngineListenerSet::const_iterator it;
    	for ( it = listeners.begin(); it != listeners.end(); it++ ) {
    		(*it)->on_note_map( this, note_map, fire );
    	}
    }
    void fire_base_note( const unsigned char& base_note, const bool& fire = true ) {
    	EngineListenerSet::const_iterator it;
    	for ( it = listeners.begin(); it != listeners.end(); it++ ) {
    		(*it)->on_base_note( this, base_note, fire );
    	}
    }
public:
    Engine( const std::string& name ) :
    	IEngine(),
    	jack::Listener(),
    	alsa::MidiInputListener(),
		client( new jack::Client( name ) ),
        output_left( new jack::AudioOutput( client, "out-L" ) ),
        output_right( new jack::AudioOutput( client, "out-R" ) ),
        sequencer( new alsa::Sequencer( name ) ),
        midi_input( new alsa::MidiInput( sequencer, name ) ),
		stretch_offset( filtering::TIME_STRETCH_DEF_STRETCH ),
		stretch_wheel( 0 ),
		volume( filtering::GAIN_DEF_VOLUME ),
		transpose_offset( filtering::TUNER_DEF_TRANSPOSE ),
		transpose_wheel( 0 ),
		linked( false ),
		soloed( 0 ),
		selected_preset( 0 ),
		base_channel( util::MIDI_CONT_BASE_CHANNEL ),
		base_note( midi::MIDDLE_C ),
		local_keyboard( false ),
		alternate_wheel( false ),
		omni( true ),
		mono( false ),
		note_map( util::NOTE_MAP_DEF_TYPE ) {
        std::ostringstream o;
        for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
        	o << "out" << ( i + 1 );
        	if ( i < 6 ) {
        		sounds[i] = new Sound( this, o.str(), (util::SoundIdentifier)i );
        	} else {
        		sounds[i] = new LinkedSound( this, o.str(), (util::SoundIdentifier)i );
        	}
        	o.str("");
        }
        ((LinkedSound*)sounds[ util::SOUND_07 ])->set_linked( sounds[ util::SOUND_08 ] );
        ((LinkedSound*)sounds[ util::SOUND_08 ])->set_linked( sounds[ util::SOUND_07 ] );
        client->activate();
        buffer_right = output_right->get_buffer();
        buffer_left = output_left->get_buffer();
		for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
			sounds[i]->acquire();
		}
        client->add_jack_listener( this );
		midi_input->add_listener( this );
        auto_connect();
    }
    ~Engine() {
    	client->deactivate();
        client->remove_jack_listener( this );
		midi_input->remove_listener( this );
        for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
            delete sounds[i];
        }
        delete midi_input;
        delete sequencer;
        delete output_left;
        delete output_right;
        delete client;
    }
    //////////////////////////////////////////////////////
    const util::NoteMapType& get_note_map() const {
    	return note_map;
    }
	void set_note_map( const util::NoteMapType& note_map, const bool& fire = true ) {
		this->note_map = note_map;
		fire_note_map( note_map, fire );
	}
    const std::string& get_name() const {
    	return client->get_name();
    }
    void set_linked( const bool& linked, const bool& fire = true ) {
    	this->linked = linked;
    }
    const bool is_linked() const {
    	return linked;
    }
    const unsigned char& get_base_channel() const {
    	return base_channel;
    }
    void set_base_channel( const unsigned char& base_channel, const bool& fire = true ) {
    	this->base_channel = base_channel;
    }
    const unsigned char& get_base_note() const {
    	return base_note;
    }
    void set_base_note( const unsigned char& base_note, const bool& fire = true ) {
    	this->base_note = base_note;
    	fire_base_note( base_note );
    }
    const bool is_local_keyboard() const {
    	return local_keyboard;
    }
    void set_local_keyboard( const bool& local_keyboard, const bool& fire = true ) {
    	this->local_keyboard = local_keyboard;
    }
    const bool is_alternate_wheel() const {
    	return alternate_wheel;
    }
    void set_alternate_wheel( const bool& alternate_wheel, const bool& fire = true ) {
    	this->alternate_wheel = alternate_wheel;
    }
    bool is_omni() const {
    	return omni;
    }
    void set_omni( const bool& omni, const bool& fire = true ) {
    	this->omni = omni;
    }
    const bool is_mono() const {
    	return mono;
    }
    void set_mono( const bool& mono, const bool& fire = true ) {
    	this->mono = mono;
    }
    ////////////////////////////////////////////////////////
    const persistence::RepulseDocument& get_document() {
    	return document;
    }
    void set_document_file( const std::string& document_file ) {
    	document.set_file( document_file );
    }
    void load_document() {
    	document.load();
    }
    void save_document() {
    	document.save();
    }
    void load_waves() {
    	size_t i;
    	persistence::Waves::const_iterator it;
    	for ( it = document.get_root().get_waves().begin();
    			it != document.get_root().get_waves().end() && i < util::MAX_SOUNDS; ++it, ++i ) {
            get_sounds()[i]->set_file_name( it->get_file() );
            get_sounds()[i]->load();
    	}
    }
    void load_repulse() {
    	set_selected_preset( document.get_root().get_selected_preset() );
    	recall_preset( get_selected_preset() );
    }
    void save_repulse() {
    	document.get_root().set_selected_preset( get_selected_preset() );
		save_document();
    }
    void next_preset() {
    	selected_preset = ( selected_preset + 1 ) % document.get_root().get_presets().size();
    }
    void previous_preset() {
    	selected_preset--;
		if ( selected_preset < 0 ) {
			selected_preset = document.get_root().get_presets().size() - 1;
		}
    }
    size_t create_preset() {
    	std::ostringstream o;
    	o << "Preset " << std::hex << time( 0 );
    	return create_preset( o.str() );
    }
    size_t create_preset( const std::string& name ) {
    	document.get_root().get_presets().push_back( persistence::Preset( name ) );
    	save_document();
    	return document.get_root().get_presets().size() - 1;
    }
    void delete_preset( const size_t& id ) {
    	if ( id < document.get_root().get_presets().size() ) {
    		document.get_root().get_presets().erase( document.get_root().get_presets().begin() + id );
    		save_document();
    	}
    }
    void rename_preset( const size_t& id, const std::string& name ) {
    	if ( id < document.get_root().get_presets().size() ) {
    		document.get_root().get_presets()[ id ].set_name( name );
    		save_document();
    	}
    }
    void save_preset( const size_t& id ) {
    	if ( id < document.get_root().get_presets().size() ) {
			persistence::Preset& preset = document.get_root().get_presets()[ id ];
			preset.get_engine().set_volume( get_volume() );
			preset.get_engine().set_stretch( get_stretch_offset() );
			preset.get_engine().set_transpose( get_transpose_offset() );
			preset.get_engine().set_linked( is_linked() );
			document.get_root().set_selected_preset( get_selected_preset() );
			preset.get_engine().set_base_channel( get_base_channel() + 1 );
			preset.get_engine().set_base_note( get_base_note() );
			preset.get_engine().set_local_keyboard( is_local_keyboard() );
			preset.get_engine().set_alternate_wheel( is_alternate_wheel() );
			preset.get_engine().set_omni( is_omni() );
			preset.get_engine().set_mono( is_mono() );
			preset.get_engine().set_note_map( get_note_map() );
			size_t i = 0;
			persistence::Sounds::iterator it;
			for ( it = preset.get_sounds().begin(); it != preset.get_sounds().end() && i < util::MAX_SOUNDS; ++it, ++i ) {
				sounds[ i ]->save_preset( *it );
			}
			save_document();
    	}
    }
    void recall_preset( const size_t& id, const bool& fire = false ) {
    	if ( id < document.get_root().get_presets().size() ) {
    		persistence::Preset& preset = document.get_root().get_presets()[ id ];
			set_volume( preset.get_engine().get_volume(), fire );
			set_stretch_offset( preset.get_engine().get_stretch(), fire );
			set_transpose_offset( preset.get_engine().get_transpose(), fire );
			set_linked( preset.get_engine().is_linked(), fire );
			set_base_channel( preset.get_engine().get_base_channel() - 1, fire );
			set_base_note( preset.get_engine().get_base_note(), fire );
			set_local_keyboard( preset.get_engine().is_local_keyboard(), fire );
			set_alternate_wheel( preset.get_engine().is_alternate_wheel(), fire );
			set_omni( preset.get_engine().is_omni(), fire );
			set_mono( preset.get_engine().is_mono(), fire );
			set_note_map( preset.get_engine().get_note_map(), fire );
			size_t i = 0;
			persistence::Sounds::iterator it;
			for ( it = preset.get_sounds().begin(); it != preset.get_sounds().end() && i < util::MAX_SOUNDS; ++it, ++i ) {
				sounds[ i ]->recall_preset( *it, fire );
			}
			set_selected_preset( id, fire );
    	}
    }
	const int& get_selected_preset() {
		return selected_preset;
	}
	void set_selected_preset( const int& selected_preset, const bool& fire = true ) {
		this->selected_preset = selected_preset;
	}
	const std::string& get_selected_preset_name() {
		return get_document().get_root().get_presets()[ get_selected_preset() ].get_name();
	}
    virtual jack::Client* get_client() const {
		return client;
	}
    void add_listener( EngineListener* listener ) {
		listeners.insert( listener );
	}
	void remove_listener( EngineListener* listener ) {
		listeners.erase( listener );
	}
	void add_midi_listener( alsa::MidiInputListener* midi_listener ) {
		midi_input->add_listener( midi_listener );
	}
	void remove_midi_listener( alsa::MidiInputListener* midi_listener ) {
		midi_input->remove_listener( midi_listener );
	}
	void solo( const util::SoundIdentifier& id, const bool& active, const bool& fire = true ) {
		Sound* sound = sounds[ id ];
		if ( active && !sound->is_soloed() ) {
			soloed++;
		} else if ( !active && sound->is_soloed() ) {
			soloed--;
		}
		sound->set_soloed( active, fire );
	}
	bool is_soloed() const {
		return soloed > 0;
	}
	void set_volume( const util::floating_t& volume, const bool& fire = true ) {
		if ( this->volume != volume ) {
			this->volume = util::adjust_value( volume,
					filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME );
			fire_volume( this->volume, fire );
		}
	}
	const util::floating_t& get_volume() const {
		return volume;
	}
    void set_transpose_wheel( const util::floating_t& transpose_wheel, const bool& fire = true ) {
    	if ( this->transpose_wheel != transpose_wheel ) {
    		this->transpose_wheel = util::adjust_value( transpose_wheel,
    				filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE );;
    		fire_transpose( get_transpose(), fire );
    	}
    }
    const util::floating_t& get_transpose_wheel() const {
    	return transpose_wheel;
    }
    void set_transpose_offset( const util::floating_t& transpose_offset, const bool& fire = true ) {
    	if ( this->transpose_offset != transpose_offset ) {
    		this->transpose_offset = util::adjust_value( transpose_offset,
    				filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE );;
    		fire_transpose( get_transpose(), fire );
    	}
    }
    const util::floating_t& get_transpose_offset() const {
    	return transpose_offset;
    }
    const util::floating_t get_transpose() const {
    	return transpose_offset + transpose_wheel;
    }
    void set_stretch_wheel( const util::floating_t& stretch_wheel, const bool& fire = true ) {
    	if ( this->stretch_wheel != stretch_wheel ) {
    		this->stretch_wheel = util::adjust_value( stretch_wheel,
    				filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE );;
    		fire_stretch( get_stretch(), fire );
    	}
    }
    const util::floating_t& get_stretch_wheel() const {
    	return stretch_wheel;
    }
    void set_stretch_offset( const util::floating_t& stretch_offset, const bool& fire = true ) {
    	if ( this->stretch_offset != stretch_offset ) {
    		this->stretch_offset = util::adjust_value( stretch_offset,
    				filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH );;
    		fire_stretch( get_stretch(), fire );
    	}
    }
    const util::floating_t& get_stretch_offset() const {
    	return stretch_offset;
    }
    const util::floating_t get_stretch() const {
    	return stretch_offset + stretch_wheel;
    }
    Sound** get_sounds() {
    	return sounds;
    }
	void on_process( jack::Client* client ) {
        size_t i;
        Sound* sound;
        // Mixdown
        midi_input->next();
        if ( is_mono() ) {
			for ( i = 0; i < util::MAX_SOUNDS; ++i ) {
				sound = sounds[i];
				sound->filter( sound->get_buffer() );
			}
        } else {
            jack::sample_t* buffer_sound;
			memset( buffer_left, 0, client->get_data_size() );
			memset( buffer_right, 0, client->get_data_size() );
			jack_nframes_t buffer_size = client->get_buffer_size();
			for ( i = 0; i < util::MAX_SOUNDS; ++i ) {
				sound = sounds[i];
				buffer_sound = sound->get_buffer();
				sound->filter( buffer_sound );
				for ( jack_nframes_t j = 0; j < buffer_size; ++j ) {
					buffer_right[j] += buffer_sound[j] * sound->get_mix_right();
					buffer_left[j] += buffer_sound[j] * sound->get_mix_left();
				}
			}
        }
	}
	void all_sound_off() {
		for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
			sounds[i]->note_off();
			sounds[i]->silence();
		}
	}
	void all_controllers_off() {
		persistence::Sound defaults;
		for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
			sounds[i]->recall_preset( defaults );
		}
	}
	void all_notes_off() {
		for ( size_t i = 0; i < util::MAX_SOUNDS; ++i ) {
			sounds[i]->silence();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void on_controller( alsa::IMidiInput* input, const midi::Controller* event ) {
		if ( is_omni() || event->get_channel() == get_base_channel() ) {
			if ( event->get_controller() == util::MIDI_CONT_ENGINE_LINKED_C10 ) {
				set_linked( util::controller_to_bool( event->get_value() ), false );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_TRANSPOSE_C10 ) {
				set_transpose_offset( util::controller_to_decimal_center( event->get_value(),
						filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE ), false );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_STRETCH_C10 ) {
				set_stretch_offset( util::controller_to_decimal_center( event->get_value(),
						filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH ), false );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_VOLUME_C10 ) {
				set_volume( util::controller_to_decimal_center( event->get_value(),
						filtering::GAIN_MIN_VOLUME, filtering::GAIN_MAX_VOLUME ), false );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_ALL_SOUND_OFF_C10 ) {
				all_sound_off();
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_ALL_CONTROLLERS_OFF_C10 ) {
				all_controllers_off();
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_LOCAL_KEYBOARD_C10 ) {
				set_local_keyboard( util::controller_to_bool( event->get_value() ) );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_ALL_NOTES_OFF_C10 ) {
				all_notes_off();
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_OMNI_MODE_OFF_C10 ) {
				set_omni( !util::controller_to_bool( event->get_value() ) );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_OMNI_MODE_ON_C10 ) {
				set_omni( util::controller_to_bool( event->get_value() ) );
			} else if ( event->get_controller() == util::MIDI_CONT_ENGINE_MONO_OPERATION_C10 ) {
				set_mono( util::controller_to_bool( event->get_value() ) );
			}
		}
	}
	void on_program_change( alsa::IMidiInput* input, const midi::ProgramChange* event ) {
		if ( is_omni() || event->get_channel() == get_base_channel() ) {
			recall_preset( event->get_program(), false );
		}
	}
	void on_channel_pressure( alsa::IMidiInput* input, const midi::ChannelPressure* event ) {
		// Not used
	}
	void on_pitch_wheel( alsa::IMidiInput* input, const midi::PitchWheel* event ) {
		if ( is_omni() || event->get_channel() == get_base_channel() ) {
			if ( is_alternate_wheel() ) {
				set_stretch_wheel( util::controller_to_range( event->get_pitch(),
						filtering::TIME_STRETCH_MIN_STRETCH, filtering::TIME_STRETCH_MAX_STRETCH, midi::MAX_PITCH ), false );
			} else {
				set_transpose_wheel( util::controller_to_range( event->get_pitch(),
						filtering::TUNER_MIN_TRANSPOSE, filtering::TUNER_MAX_TRANSPOSE, midi::MAX_PITCH ), false );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void on_sample_rate( jack::Client* client ) {
		// TODO: implement
	}
	void on_buffer_size( jack::Client* client ) {
		// TODO: implement
	}
	void on_shutdown( jack::Client* client ) {
		// TODO: implement
	}
	//////////////////////////////////////////////////////////////////////////
    void auto_connect() {
    	const char** ports;
    	if ( ( ports = jack_get_ports( client->get_jack_client(), 0, 0, JackPortIsPhysical | JackPortIsInput ) ) != 0 ) {
			int i = 0;
			while ( ports[i] != 0 && i < 2 ) {
				switch ( i ) {
				case 0:
					jack_connect( client->get_jack_client(), jack_port_name( output_left->get_jack_port() ), ports[i] );
					break;
				case 1:
					jack_connect( client->get_jack_client(), jack_port_name( output_right->get_jack_port() ), ports[i] );
					break;
				}
				i++;
			}
    	}
    }
};

} // namespace repulse

#endif /* REPULSE_H_ */
