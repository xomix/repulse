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

#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include "jack.h"
#include "filtering.h"
#include "util.h"

namespace envelope {

enum DecayType {
	DECAY_TYPE_INFINITE = 0,
	DECAY_TYPE_TRIGGER,
	DECAY_TYPE_GATE
};

static const DecayType        DECAY_DEF_TYPE = DECAY_TYPE_INFINITE;
static const DecayType        DECAY_LAST_TYPE = DECAY_TYPE_GATE;

static const util::floating_t MIN_ATTACK = 0;
static const util::floating_t MAX_ATTACK = 0.001;
static const util::floating_t DEF_ATTACK = MIN_ATTACK;
static const util::floating_t MIN_DECAY  = 0;
static const util::floating_t MAX_DECAY  = 2; // TODO: The length is important.
static const util::floating_t DEF_DECAY  = MAX_DECAY;
static const bool             DEF_SOFT_START = false;

class Machine;

class State {
public:
	State() {}
	virtual ~State() {}
	virtual State* note_on( Machine* machine ) { return this; }
	virtual State* note_off( Machine* machine ) { return this; }
	virtual State* silence( Machine* machine ) { return this; }
	virtual State* filter( Machine* machine, jack::sample_t* samples ) { return this; }
};

class Machine : public filtering::Filter {
private:
	State* state;
	bool off;
	bool start_soft;
	DecayType decay_type;
	util::floating_t attack_time;
	util::floating_t decay_time;
	jack_nframes_t attack_samples;
	jack_nframes_t decay_samples;
	jack_nframes_t offset;
	util::floating_t slope;
	util::floating_t curve;
	util::floating_t amplitude;
public:
	Machine( jack::Client* client ) :
		filtering::Filter( client ), off( false ), start_soft( DEF_SOFT_START ),
		decay_type( DECAY_DEF_TYPE ), offset( 0 ),
		slope( 0 ), curve( 0 ), amplitude( 0 ) {
		set_attack_time( DEF_ATTACK );
		set_decay_time( DEF_DECAY );
		start();
	}
	~Machine() {}
	static DecayType controller_to_decay_type( unsigned char value ) {
		return (DecayType)( value / ( 128. / (util::floating_t)( DECAY_LAST_TYPE + 1 ) ) );
	}
	void start();
	jack::Client* get_client() const { return Filter::get_client(); }
    void set_attack_time( const util::floating_t& attack_time ) {
    	this->attack_time = util::adjust_value( attack_time, MIN_ATTACK, MAX_ATTACK );
    	attack_samples = get_client()->time_to_frames( this->attack_time );
    }
    const util::floating_t& get_attack_time() const { return attack_time; }
	void set_decay_time( const util::floating_t& decay_time ) {
    	this->decay_time = util::adjust_value( decay_time, MIN_DECAY, MAX_DECAY );
    	decay_samples = get_client()->time_to_frames( this->decay_time );
    }
	const util::floating_t& get_decay_time() const { return decay_time; }
	const jack_nframes_t& get_attack_samples() const { return attack_samples; }
	const jack_nframes_t& get_decay_samples() const { return decay_samples; }
	void set_off( const bool& off ) { this->off = off; }
	const bool& is_off() const { return off; }
	void set_start_soft( const bool& start_soft ) { this->start_soft = start_soft; }
	const bool& is_start_soft() const { return start_soft; }
	void set_decay_type( const DecayType& decay_type ) { this->decay_type = decay_type; }
	const DecayType& get_decay_type() const { return decay_type; }
	void set_offset( const jack_nframes_t& offset ) { this->offset = offset; }
	const jack_nframes_t& get_offset() const { return offset; }
	void note_on() { state = state->note_on( this ); }
	void note_off() { state = state->note_off( this ); }
	void silence() { state = state->silence( this ); }
	void filter( jack::sample_t* samples ) { state = state->filter( this, samples ); }
	bool is_finished() const;
	const util::floating_t& get_slope() const { return slope; }
	void set_slope( const util::floating_t& slope ) { this->slope = slope; }
	const util::floating_t& get_curve() const { return curve; }
	const util::floating_t& get_amplitude() const { return amplitude; }
	void set_amplitude( const util::floating_t& amplitude ) { this->amplitude = amplitude; }
	void calculate( const util::floating_t& duration_samples ) {
	    util::floating_t rdur = 1.0 / ( duration_samples * 2. );
	    util::floating_t rdur2 = rdur * rdur;
	    slope = 4.0 * ( rdur - rdur2 );
	    curve = -8.0 * rdur2;
	}
	void calculate_attack() {
		calculate( get_attack_samples() );
		amplitude = 0;
	}
	void calculate_decay() {
		calculate( get_decay_samples() );
	    slope += get_decay_samples() * curve;
		amplitude = 1;
	}
};

class StateOff : public State {
protected:
	StateOff() : State() {}
public:
	~StateOff() {}
	static State* get_instance() {
		static StateOff instance;
		return &instance;
	}
	State* note_on( Machine* machine );
	State* filter( Machine* machine, jack::sample_t* samples ) {
		memset( samples, 0, machine->get_client()->get_data_size() );
		return this;
	}
};

class StateDecay : public State {
protected:
	StateDecay() : State() {}
public:
	~StateDecay() {}
	static State* get_instance() {
		static StateDecay instance;
		return &instance;
	}
	State* note_on( Machine* machine ) {
		return StateOff::get_instance()->note_on( machine );
	}
	virtual State* silence( Machine* machine ) {
		return StateOff::get_instance();
	}
	State* filter( Machine* machine, jack::sample_t* samples ) {
		State* state = this;
		jack_nframes_t offset = machine->get_offset();
		jack_nframes_t buffer_size = machine->get_client()->get_buffer_size();
		jack_nframes_t decay_samples = machine->get_decay_samples();
		jack_nframes_t i;
		for ( i = 0; i < buffer_size && offset < decay_samples; ++i, ++offset ) {
			samples[i] *= machine->get_amplitude();
			machine->set_amplitude( machine->get_amplitude() + machine->get_slope() );
			machine->set_slope( machine->get_slope() + machine->get_curve() );
		}
		if ( offset >= decay_samples ) {
			if ( i < buffer_size ) {
				memset( samples + i, 0, ( buffer_size - i ) * sizeof( jack::sample_t ) );
			}
			machine->set_offset( 0 );
			state = StateOff::get_instance();
		} else {
			machine->set_offset( offset );
		}
		return state;
	}
};

class StateSustain : public State {
protected:
	StateSustain() : State() {}
public:
	~StateSustain() {}
	static State* get_instance() {
		static StateSustain instance;
		return &instance;
	}
	State* note_on( Machine* machine ) {
		return StateOff::get_instance()->note_on( machine );
	}
	State* note_off( Machine* machine ) {
		State* state = this;
		if ( DECAY_TYPE_INFINITE != machine->get_decay_type() ) {
			machine->calculate_decay();
			state = StateDecay::get_instance();
		}
		return state;
	}
	virtual State* silence( Machine* machine ) {
		return StateOff::get_instance();
	}
};

class StateAttack : public State {
protected:
	StateAttack() : State() {}
public:
	~StateAttack() {}
	static State* get_instance() {
		static StateAttack instance;
		return &instance;
	}
	State* note_on( Machine* machine ) {
		return StateOff::get_instance()->note_on( machine );
	}
	virtual State* silence( Machine* machine ) {
		return StateOff::get_instance();
	}
	State* filter( Machine* machine, jack::sample_t* samples ) {
		State* state = this;
		jack_nframes_t offset = machine->get_offset();
		jack_nframes_t buffer_size = machine->get_client()->get_buffer_size();
		jack_nframes_t attack_samples = machine->get_attack_samples();
		for ( jack_nframes_t i = 0; i < buffer_size && offset < attack_samples; ++i, ++offset ) {
			samples[i] *= machine->get_amplitude();
			machine->set_amplitude( machine->get_amplitude() + machine->get_slope() );
			machine->set_slope( machine->get_slope() + machine->get_curve() );
		}
		if ( offset >= attack_samples ) {
			machine->set_offset( 0 );
			if ( DECAY_TYPE_TRIGGER == machine->get_decay_type() ) {
				machine->calculate_decay();
				state = StateDecay::get_instance();
			} else {
				state = StateSustain::get_instance();
			}
		} else {
			machine->set_offset( offset );
		}
		return state;
	}
};

inline State* StateOff::note_on( Machine* machine ) {
	State* state = this;
	machine->set_off( false );
	machine->set_offset( 0 );
	if ( machine->is_start_soft() ) {
		machine->calculate_attack();
		state = StateAttack::get_instance();
	} else if ( DECAY_TYPE_TRIGGER == machine->get_decay_type() ) {
		machine->calculate_decay();
		state = StateDecay::get_instance();
	} else {
		state = StateSustain::get_instance();
	}
	return state;
}

inline void Machine::start() { state = StateOff::get_instance(); }

bool Machine::is_finished() const { return state == StateOff::get_instance(); }

} // namespace envelope

#endif /* ENVELOPE_H_ */
