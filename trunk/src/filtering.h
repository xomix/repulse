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

#ifndef FILTERING_H_
#define FILTERING_H_

#include <cassert>
#include <samplerate.h>
#include <sndfile.hh>
#include <FIFOSampleBuffer.h>
#include <TDStretch.h>
#include "jack.h"
#include "util.h"

namespace filtering {

class Filter {
	jack::Client* client;
	bool active;
protected:
	jack::Client* get_client() const { return client; }
public:
	Filter( jack::Client* client ) :
		client( client ), active( false ) {}
	virtual ~Filter() {}
	void set_active( const bool& active ) { this->active = active; }
	const bool& is_active() const { return active; }
	virtual void filter( jack::sample_t* samples ) {}
};

static const util::floating_t OVER_DRIVE_MIN_DRIVE  = 1;
static const util::floating_t OVER_DRIVE_MAX_DRIVE  = 10;
static const util::floating_t OVER_DRIVE_DEF_DRIVE = OVER_DRIVE_MIN_DRIVE;
static const util::floating_t OVER_DRIVE_DEF_DC = -0.001;
static const util::floating_t OVER_DRIVE_DEF_ACTIVE = false;

class OverDrive : public Filter {
	util::floating_t drive;
	util::floating_t dc;
public:
	OverDrive( jack::Client* client ) :
		Filter( client ), drive( OVER_DRIVE_DEF_DRIVE ), dc( OVER_DRIVE_DEF_DC ) {
		set_active( OVER_DRIVE_DEF_ACTIVE );
	}
	~OverDrive() {}
	void set_drive( const util::floating_t& drive ) {
		this->drive = util::adjust_value( drive, OVER_DRIVE_MIN_DRIVE, OVER_DRIVE_MAX_DRIVE );
	}
	const util::floating_t& get_drive() const { return drive; }
	void set_dc( const util::floating_t& dc ) { this->dc = dc; }
	const util::floating_t& get_dc() const { return dc; }
	void filter( jack::sample_t* samples ) {
		if ( is_active() ) {
			util::floating_t sample;
			jack_nframes_t buffer_size = get_client()->get_buffer_size();
			for ( jack_nframes_t i = 0; i < buffer_size; ++i ) {
				sample = samples[i];
				sample *= get_drive();
				sample += get_dc();
				if ( sample <= -1. ) {
					sample = -2. / 3.;
				} else if ( sample >= 1. ) {
					sample = 2. / 3.;
				} else {
					sample = sample - ( ( sample * sample * sample ) / 3. );
				}
				samples[i] = sample;
			}
		}
	}
};

enum FrequencyFilterType {
	FREQUENCY_FILTER_TYPE_LOW_PASS = 0,
	FREQUENCY_FILTER_TYPE_HIGH_PASS,
	FREQUENCY_FILTER_TYPE_BAND_PASS_1,
	FREQUENCY_FILTER_TYPE_BAND_PASS_2,
	FREQUENCY_FILTER_TYPE_NOTCH
};

static const  FrequencyFilterType FREQUENCY_FILTER_DEF_TYPE = FREQUENCY_FILTER_TYPE_LOW_PASS;
static const  FrequencyFilterType FREQUENCY_FILTER_LAST_TYPE = FREQUENCY_FILTER_TYPE_NOTCH;

static const  util::floating_t    FREQUENCY_MIN_FREQUENCY = 30.;
static inline util::floating_t    FREQUENCY_MAX_FREQUENCY( const jack::Client* client ) { return client->get_sample_rate() / 2.; }
static const  util::floating_t    FREQUENCY_DEF_FREQUENCY = FREQUENCY_MIN_FREQUENCY;
static const  util::floating_t    FREQUENCY_MIN_RESONANCE = 0.5;
static const  util::floating_t    FREQUENCY_MAX_RESONANCE = 1;
static const  util::floating_t    FREQUENCY_DEF_RESONANCE = FREQUENCY_MAX_RESONANCE;
static const  bool                FREQUENCY_DEF_ACTIVE = false;
static const  unsigned char       FREQUENCY_MEMORY_SIZE = 3;

class FrequencyStrategy {
	util::floating_t frequency; // TODO: review this type
	util::floating_t resonance;
	bool dirty;
protected:
	FrequencyStrategy() :
		frequency( FREQUENCY_DEF_FREQUENCY ),
		resonance( FREQUENCY_DEF_RESONANCE ),
		dirty( false ) {}
public:
	util::floating_t w0;
	util::floating_t a0;
	util::floating_t alpha;
	util::floating_t b0_a0;
	util::floating_t b1_a0;
	util::floating_t b2_a0;
	util::floating_t a1_a0;
	util::floating_t a2_a0;
	virtual ~FrequencyStrategy() {}
	void set_frequency( const util::floating_t& frequency ) {
		if ( this->frequency != frequency ) {
			this->frequency = frequency;
			dirty = true;
		}
	}
	const util::floating_t& get_frequency() const {
		return frequency;
	}
	void set_resonance( const util::floating_t& resonance ) {
		if ( this->resonance != resonance ) {
			this->resonance = resonance;
			dirty = true;
		}
	}
	const util::floating_t& get_resonance() const {
		return resonance;
	}
	const bool& is_dirty() {
		return dirty;
	}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		dirty = false;
		// w0 = 2*pi*f0/Fs
		w0 = 2. * util::PI * ( get_frequency() / (util::floating_t)sample_rate );
		// alpha = sin(w0)/(2*Q)
		alpha = sin(w0) / ( 2. * get_resonance() );
		// a0 =   1 + alpha
		a0 = 1. + alpha;
	}
};

class LowPass : public FrequencyStrategy {
public:
	LowPass() : FrequencyStrategy() {}
	~LowPass() {}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		FrequencyStrategy::compute( sample_rate );
		// LPF: H(s) = 1 / (s^2 + s/Q + 1)
		// a1 =  -2*cos(w0)
		a1_a0 = ( -2. * cos(w0) ) / a0;
		// a2 =   1 - alpha
		a2_a0 = ( 1. - alpha ) / a0;
		// b0 =  (1 - cos(w0))/2
		b0_a0 = ( ( 1. - cos(w0) ) / 2. ) / a0;
		// b1 =   1 - cos(w0)
		b1_a0 = ( 1. - cos(w0) ) / a0;
		// b2 =  (1 - cos(w0))/2
		b2_a0 = b0_a0;
	}
};

class HighPass : public FrequencyStrategy {
public:
	HighPass() : FrequencyStrategy() {}
	~HighPass() {}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		FrequencyStrategy::compute( sample_rate );
		// HPF: H(s) = s^2 / (s^2 + s/Q + 1)
		// a1 =  -2*cos(w0)
		a1_a0 = ( -2. * cos(w0) ) / a0;
		// a2 =   1 - alpha
		a2_a0 = ( 1. - alpha ) / a0;
		// b0 =  (1 + cos(w0))/2
		b0_a0 = ( ( 1. + cos(w0) ) / 2. ) / a0;
		// b1 = -(1 + cos(w0))
		b1_a0 = ( -( 1. + cos(w0) ) ) / a0;
		// b2 =  (1 + cos(w0))/2
		b2_a0 = ( ( 1. + cos(w0) ) / 2. ) / a0;
	}

};

class BandPass1 : public FrequencyStrategy {
public:
	BandPass1() : FrequencyStrategy() {}
	~BandPass1() {}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		FrequencyStrategy::compute( sample_rate );
		// BPF: H(s) = s / (s^2 + s/Q + 1)
		//      (constant skirt gain, peak gain = Q)
		// a1 =  -2*cos(w0)
		a1_a0 = ( -2. * cos(w0) ) / a0;
		// a2 =   1 - alpha
		a2_a0 = ( 1. - alpha ) / a0;
		// b0 =   sin(w0)/2  =   Q*alpha
		b0_a0 = ( get_resonance() * alpha ) / a0;
		// b1 =   0
		b1_a0 = 0.;
		// b2 =  -sin(w0)/2  =  -Q*alpha
		b2_a0 = ( -get_resonance() * alpha ) / a0;
	}
};

class BandPass2 : public FrequencyStrategy {
public:
	BandPass2() : FrequencyStrategy() {}
	~BandPass2() {}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		FrequencyStrategy::compute( sample_rate );
		// BPF: H(s) = (s/Q) / (s^2 + s/Q + 1)
		//      (constant 0 dB peak gain)
		// a1 =  -2*cos(w0)
		a1_a0 = ( -2. * cos(w0) ) / a0;
		// a2 =   1 - alpha
		a2_a0 = ( 1. - alpha ) / a0;
		// b0 =   alpha
		b0_a0 = alpha / a0;
		// b1 =   0
		b1_a0 = 0.;
		// b2 =  -alpha
		b2_a0 = -alpha / a0;
	}

};

class Notch : public FrequencyStrategy {
public:
	Notch() : FrequencyStrategy() {}
	~Notch() {}
	virtual void compute( const jack_nframes_t& sample_rate ) {
		FrequencyStrategy::compute( sample_rate );
		// notch: H(s) = (s^2 + 1) / (s^2 + s/Q + 1)
		// a1 =  -2*cos(w0)
		a1_a0 = ( -2. * cos(w0) ) / a0;
		// a2 =   1 - alpha
		a2_a0 = ( 1. - alpha ) / a0;
		// b0 =   1
		b0_a0 = 1.;
		// b1 =  -2*cos(w0)
		b1_a0 = ( -2. * cos(w0) ) / a0;
		// b2 =   1
		b2_a0 = 1.;
	}
};

class Frequency : public Filter {
private:
	FrequencyStrategy* strategies[ FREQUENCY_FILTER_LAST_TYPE + 1 ];
	FrequencyFilterType filter_type;
	FrequencyStrategy* strategy;
	jack_nframes_t memory_offset;
	// ... ... ...
	// 012 012 012
	// PLc cPL LcP
	// ^    ^    ^
	util::floating_t y[ FREQUENCY_MEMORY_SIZE ];
	util::floating_t x[ FREQUENCY_MEMORY_SIZE ];
protected:
	void compute() {
		if ( strategy->is_dirty() ) {
			strategy->compute( get_client()->get_sample_rate() );
		}
	}
	void compute_all() {
		for ( size_t i = FREQUENCY_FILTER_TYPE_LOW_PASS; i <= FREQUENCY_FILTER_TYPE_NOTCH; i++ ) {
			strategies[ i ]->compute( get_client()->get_sample_rate() );
		}
	}
public:
	Frequency( jack::Client* client ) :
		Filter( client ), filter_type( FREQUENCY_FILTER_TYPE_LOW_PASS ), memory_offset( 0 ) {
		set_active( FREQUENCY_DEF_ACTIVE );
		strategies[ FREQUENCY_FILTER_TYPE_LOW_PASS ] = new LowPass;
		strategies[ FREQUENCY_FILTER_TYPE_HIGH_PASS ] = new HighPass;
		strategies[ FREQUENCY_FILTER_TYPE_BAND_PASS_1 ] = new BandPass1;
		strategies[ FREQUENCY_FILTER_TYPE_BAND_PASS_2 ] = new BandPass2;
		strategies[ FREQUENCY_FILTER_TYPE_NOTCH ] = new Notch;
		strategy = strategies[ filter_type ];
		memset( x, 0, FREQUENCY_MEMORY_SIZE * sizeof( util::floating_t ) );
		memset( y, 0, FREQUENCY_MEMORY_SIZE * sizeof( util::floating_t ) );
		compute_all();
	}
	~Frequency() {
		for ( size_t i = FREQUENCY_FILTER_TYPE_LOW_PASS; i <= FREQUENCY_FILTER_TYPE_NOTCH; i++ ) {
			delete strategies[ i ];
		}
	}
	static FrequencyFilterType controller_to_filter_type( unsigned char value ) {
		return (FrequencyFilterType)( value / ( 128. / (util::floating_t)( FREQUENCY_FILTER_LAST_TYPE + 1 ) ) );
	}
    void set_filter_type( const FrequencyFilterType& filter_type ) {
    	this->filter_type = filter_type;
    	strategy = strategies[ filter_type ];
    }
    const FrequencyFilterType& get_filter_type() const {
    	return filter_type;
    }
	void set_frequency( const util::floating_t& frequency ) {
		strategy->set_frequency( util::adjust_value(
				frequency, FREQUENCY_MIN_FREQUENCY, FREQUENCY_MAX_FREQUENCY( get_client() ) ) );
		compute();
	}
	const util::floating_t& get_frequency() const {
		return strategy->get_frequency();
	}
	void set_resonance( const util::floating_t& resonance ) {
		strategy->set_resonance( util::adjust_value(
				resonance, FREQUENCY_MIN_RESONANCE, FREQUENCY_MAX_RESONANCE ) );
		compute();
	}
	const util::floating_t& get_resonance() const {
		return strategy->get_resonance();
	}
	void filter( jack::sample_t* samples ) {
		jack_nframes_t buffer_size = get_client()->get_buffer_size();
		unsigned char s_0, s_1, s_2;
		if ( is_active() ) {
			for ( jack_nframes_t i = 0; i < buffer_size; ++i, ++memory_offset ) {
				// y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
				//                     - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]
				s_0 = ( memory_offset + 2 ) % FREQUENCY_MEMORY_SIZE;
				s_1 = ( memory_offset + 1 ) % FREQUENCY_MEMORY_SIZE;
				s_2 = ( memory_offset     ) % FREQUENCY_MEMORY_SIZE;
				x[s_0] = samples[i];
				samples[i] = y[s_0] = strategy->b0_a0 * x[s_0] + strategy->b1_a0 * x[s_1] + strategy->b2_a0 * x[s_2]
														   - strategy->a1_a0 * y[s_1] - strategy->a2_a0 * y[s_2];
			}
		} else {
			buffer_size--;
			memory_offset += buffer_size;
			s_0 = ( memory_offset + 2 ) % FREQUENCY_MEMORY_SIZE;
			s_1 = ( memory_offset + 1 ) % FREQUENCY_MEMORY_SIZE;
			s_2 = ( memory_offset     ) % FREQUENCY_MEMORY_SIZE;
			x[s_0] = y[s_0] = samples[ buffer_size-- ];
			x[s_1] = y[s_1] = samples[ buffer_size-- ];
			x[s_2] = y[s_2] = samples[ buffer_size-- ];
			memory_offset++;
		}
	}
};

static const util::floating_t GAIN_MIN_VOLUME = 0;
static const util::floating_t GAIN_MAX_VOLUME = 4;
static const util::floating_t GAIN_DEF_VOLUME = 1;

class Gain : public Filter {
	util::floating_t volume;
public:
	Gain( jack::Client* client ) : Filter( client ), volume( GAIN_DEF_VOLUME ) {}
	~Gain() {}
	void set_volume( const util::floating_t& volume ) {
		this->volume = util::adjust_value( volume, GAIN_MIN_VOLUME, GAIN_MAX_VOLUME );
	}
	const util::floating_t& get_volume() const {
		return volume;
	}
	void filter( jack::sample_t* samples ) {
		jack_nframes_t buffer_size = get_client()->get_buffer_size();
		for ( jack_nframes_t i = 0; i < buffer_size; ++i ) {
			samples[i] *= volume;
		}
	}
};

class Generator : public Filter, public jack::BufferedSource {
public:
	Generator( jack::Client* client ) : Filter( client ) {}
	virtual ~Generator() {}
	void filter( jack::sample_t* samples ) {
		jack::sample_t* origin;
		jack_nframes_t received = receive( &origin );
		memcpy( samples, origin, received * sizeof( jack::sample_t ) );
		if ( received < get_client()->get_buffer_size() ) {
			memset( samples + received, 0,
					( get_client()->get_buffer_size() - received ) * sizeof( jack::sample_t ) );
		}
	}
};

static const int              WAVE_MAX_CHANNELS = 1;
static const util::floating_t WAVE_MIN_START_TIME = 0;
static const util::floating_t WAVE_MAX_START_TIME = 0.1;
static const util::floating_t WAVE_DEF_START_TIME = WAVE_MIN_START_TIME;

class Wave : public Generator {
    jack::sample_t* buffer;
    jack_nframes_t buffer_size;
    util::floating_t start_time;
    jack_nframes_t start_frame;
    std::string file_name;
    jack_nframes_t count;
    jack_nframes_t offset;
    jack_nframes_t sample_rate;
protected:
    void clear() { delete buffer; }
public:
    Wave( jack::Client* client ) :
    	Generator( client ),
    	buffer( 0 ), buffer_size( 0 ),
    	start_time( WAVE_DEF_START_TIME ), start_frame( 0 ),
    	count( 0 ), offset( 0 ), sample_rate( 0 ) {
    }
    virtual ~Wave() {
    	clear();
    }
    void load() {
        SndfileHandle handle( file_name );
        if ( SF_ERR_NO_ERROR == handle.error() && WAVE_MAX_CHANNELS == handle.channels() ) {
            clear();
            sample_rate = handle.samplerate();
			buffer = new jack::sample_t[ handle.frames() ];
			handle.read( buffer, handle.frames() );
			buffer_size = handle.frames();
        }
    }
    void set_start_time( const util::floating_t& start_time ) {
    	util::floating_t ti = util::adjust_value( start_time, WAVE_MIN_START_TIME, WAVE_MAX_START_TIME );
    	util::floating_t fs = get_client()->time_to_frames( ti );
    	if ( fs > buffer_size ) {
    		fs = buffer_size;
    		this->start_time = get_client()->frames_to_time( fs );
    	} else {
    		this->start_time = ti;
    	}
        start_frame = fs;
    }
    const util::floating_t& get_start_time() const {
    	return start_time;
    }
    void set_file_name( const std::string& file_name ) {
    	this->file_name = file_name;
    }
    const std::string& get_file_name() const {
    	return file_name;
    }
	bool is_finished() {
		return offset >= buffer_size;
	}
    jack_nframes_t get_sample_rate() {
		return sample_rate == 0 ? get_client()->get_sample_rate() : sample_rate;
	}
	void reset() {
		count = 0;
		offset = start_frame;
	}
	jack_nframes_t receive( jack::sample_t** samples ) {
		offset += count;
		count = 0;
		if ( offset < buffer_size ) {
			*samples = buffer + offset;
			count = offset + get_client()->get_buffer_size() > buffer_size
					? buffer_size - offset : get_client()->get_buffer_size();
		}
		return count;
	}
};

class TimeStretchPreset {
	int sequence;
	int window;
	int overlap;
public:
	TimeStretchPreset( const int& sequence, const int& window, const int& overlap ) :
		sequence( sequence ), window( window ), overlap( overlap ) {}
	virtual ~TimeStretchPreset() {}
	const int& get_sequence() const { return sequence; }
	const int& get_window() const { return window; }
	const int& get_overlap() const { return overlap; }
};

static TimeStretchPreset TIME_STRETCH_PRESETS[] = {
		TimeStretchPreset(
				USE_AUTO_SEQUENCE_LEN,
				USE_AUTO_SEEKWINDOW_LEN,
				DEFAULT_OVERLAP_MS ),   // Auto
		TimeStretchPreset( 40, 15, 8 ), // Speech
		TimeStretchPreset( 28, 14, 7 ), // ModeA
		TimeStretchPreset( 24, 12, 6 )  // ModeB
};

enum TimeStretchType {
	TIME_STRETCH_TYPE_AUTO = 0,
	TIME_STRETCH_TYPE_SPEECH,
	TIME_STRETCH_TYPE_1,
	TIME_STRETCH_TYPE_2
};

static const TimeStretchType  TIME_STRETCH_DEF_TYPE    = TIME_STRETCH_TYPE_AUTO;
static const TimeStretchType  TIME_STRETCH_LAST_TYPE   = TIME_STRETCH_TYPE_2;

static const util::floating_t TIME_STRETCH_MIN_STRETCH = 0;
static const util::floating_t TIME_STRETCH_MAX_STRETCH = 4;
static const util::floating_t TIME_STRETCH_DEF_STRETCH = 1;
static const util::floating_t TIME_STRETCH_NO_STRETCH  = TIME_STRETCH_DEF_STRETCH;
static const jack_nframes_t   TIME_STRETCH_MAX_FLUSH   = 8192;

class TimeStretch : public Generator {
private:
	soundtouch::TDStretch* time_stretch;
	jack::BufferedSource* source;
	util::floating_t stretch;
	jack_nframes_t count;
	TimeStretchType type;
	jack::sample_t* silence;
	jack_nframes_t flush_blocks;
	bool flushed;
public:
	TimeStretch( jack::Client* client, jack::BufferedSource* source ) :
		Generator( client ),
		time_stretch( soundtouch::TDStretch::newInstance() ), source( source ),
		stretch( 1 ), count( 0 ), type( TIME_STRETCH_DEF_TYPE ),
		silence( 0 ), flush_blocks( TIME_STRETCH_MAX_FLUSH / client->get_buffer_size() ),
		flushed( true ) {
		time_stretch->setChannels( WAVE_MAX_CHANNELS );
		time_stretch->enableQuickSeek( true );
		time_stretch->setTempo( stretch );
		silence = new jack::sample_t[ client->get_buffer_size() ];
		memset( silence, 0, client->get_data_size() );
	}
	virtual ~TimeStretch() {
		delete time_stretch;
		delete silence;
	}
	static TimeStretchType controller_to_stretch_type( unsigned char value ) {
		return (TimeStretchType)( value / ( 128. / (util::floating_t)( TIME_STRETCH_LAST_TYPE + 1 ) ) );
	}
	void set_type( const TimeStretchType& type ) {
		this->type = type;
	}
	const TimeStretchType& get_type() const {
		return type;
	}
	void set_stretch( const util::floating_t& stretch ) {
    	this->stretch = util::adjust_value( stretch, TIME_STRETCH_MIN_STRETCH, TIME_STRETCH_MAX_STRETCH );
    	time_stretch->setTempo( this->stretch );
	}
	const util::floating_t& get_stretch() const {
		return stretch;
	}
	bool is_finished() {
		return time_stretch->numSamples() <= 0;
	}
	jack_nframes_t get_sample_rate() {
		return source->get_sample_rate();
	}
	void reset() {
		flushed = false;
		count = 0;
		time_stretch->clear();
		source->reset();
		TimeStretchPreset& preset = TIME_STRETCH_PRESETS[ get_type() ];
		time_stretch->setParameters( source->get_sample_rate(),
				preset.get_sequence(), preset.get_window(), preset.get_overlap() );
	}
	jack_nframes_t receive( jack::sample_t** samples ) {
		jack_nframes_t ret = 0;
		if ( util::almost_equal( stretch, TIME_STRETCH_NO_STRETCH ) ) {
			count = 0;
			time_stretch->clear();
			ret = source->receive( samples );
		} else {
			jack::sample_t* origin;
			jack_nframes_t buffer_size = get_client()->get_buffer_size();
			time_stretch->receiveSamples( count );
			while ( time_stretch->numSamples() < buffer_size ) {
				ret = source->receive( &origin );
				if ( ret ) {
					time_stretch->putSamples( origin, ret );
				} else {
					break;
				}
			}
			if ( !( ret || flushed ) && ( time_stretch->numSamples() < buffer_size ) ) {
				// flush object
				jack_nframes_t last_count = time_stretch->numSamples();
				for ( jack_nframes_t i = 0; i < flush_blocks
					&& last_count == time_stretch->numSamples(); ++i ) {
					time_stretch->putSamples( silence, buffer_size );
				}
				flushed = true;
			}
			*samples = time_stretch->getOutput()->ptrBegin();
			ret = count = time_stretch->numSamples() > buffer_size
					? buffer_size : time_stretch->numSamples();
		}
		return ret;
	}
};

static const util::floating_t TUNER_MIN_TRANSPOSE = -48;
static const util::floating_t TUNER_MAX_TRANSPOSE = 48;
static const util::floating_t TUNER_DEF_TRANSPOSE = 0;
static const util::floating_t TUNER_NO_TRANSPOSE = 1;

class Tuner : public Generator {
	jack::BufferedSource* source;
	util::floating_t transpose;
	util::floating_t ratio;
	util::floating_t final_ratio;
	SRC_STATE* state;
	jack::sample_t* buffer;
	bool finished;
	static long callback( void* cb_data, jack::sample_t** data ) {
		return ((Tuner*)cb_data)->on_data( data );
	}
protected:
	long on_data( jack::sample_t** data ) {
		return source->receive( data );
	}
public:
	Tuner( jack::Client* client, jack::BufferedSource* source ) :
		Generator( client ),
		source( source ), transpose( TUNER_DEF_TRANSPOSE ),
		ratio( TUNER_NO_TRANSPOSE ), final_ratio( TUNER_NO_TRANSPOSE ),
		state( 0 ), finished( true ) {
		int error;
		state = src_callback_new( callback, SRC_ZERO_ORDER_HOLD, WAVE_MAX_CHANNELS, &error, this );
		buffer = new jack::sample_t[ client->get_buffer_size() ];
		memset( buffer, 0, client->get_buffer_size() );
		assert( state >= 0 );
		/*
          SRC_SINC_BEST_QUALITY       = 0,
          SRC_SINC_MEDIUM_QUALITY     = 1,
          SRC_SINC_FASTEST            = 2,
          SRC_ZERO_ORDER_HOLD         = 3,
          SRC_LINEAR                  = 4
		 */
	}
	~Tuner() {
		src_delete( state );
		delete buffer;
	}
    void set_transpose( const util::floating_t& transpose ) {
    	this->transpose = util::adjust_value( transpose, TUNER_MIN_TRANSPOSE, TUNER_MAX_TRANSPOSE );
    	ratio = pow( 2., -this->transpose / 12. );
    	final_ratio = ratio
    			* ( get_client()->get_sample_rate() / (util::floating_t)source->get_sample_rate() );
    	assert( src_is_valid_ratio( final_ratio ) );
		src_set_ratio( state, final_ratio );
    }
    const util::floating_t& get_transpose() const {
    	return transpose;
    }
	const util::floating_t& get_ratio() {
		return ratio;
	}
	jack_nframes_t get_sample_rate() {
		return get_client()->get_sample_rate();
	}
	bool is_finished() {
		return finished;
	}
	void reset() {
		finished = false;
		final_ratio = ratio
				* ( get_client()->get_sample_rate() / (util::floating_t)source->get_sample_rate() );
		src_reset( state );
		source->reset();
	}
	jack_nframes_t receive( jack::sample_t** samples ) {
		jack_nframes_t ret;
		if ( util::almost_equal( final_ratio, TUNER_NO_TRANSPOSE ) ) {
			ret = source->receive( samples );
		} else {
			ret = src_callback_read( state, final_ratio, get_client()->get_buffer_size(), buffer );
			*samples = buffer;
		}
		if ( ret <= 0 ) {
			finished = true;
		}
		return ret;
	}
};

static const util::floating_t PANNER_MIN_PANNING = -1;
static const util::floating_t PANNER_MAX_PANNING = 1;
static const util::floating_t PANNER_DEF_PANNING = 0;

class Panner {
	util::floating_t panning;
	util::floating_t mix_left;
	util::floating_t mix_right;
public:
	Panner() {
		set_panning( PANNER_DEF_PANNING );
	}
	virtual ~Panner() {}
	void set_panning( const util::floating_t& panning ) {
		this->panning = util::adjust_value( panning, PANNER_MIN_PANNING, PANNER_MAX_PANNING );
		mix_left = 1 - panning;
		mix_right = 1 + panning;
	}
	const util::floating_t& get_panning() const {
		return panning;
	}
	const util::floating_t& get_mix_left() const {
		return mix_left;
    }
	const util::floating_t& get_mix_right() const {
		return mix_right;
	}
};

} // namespace filtering

#endif /* FILTERING_H_ */
