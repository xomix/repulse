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

#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>

namespace util {

typedef float floating_t;
const static std::string VERSION_ = "0.0.1";
static const std::string BLANK = "";
static const util::floating_t ZERO = 0;
static const floating_t PI = acos(-1);
static const floating_t PI_HALF = PI / 2;
static const floating_t PI_DOUBLE = PI * 2;
static const size_t MAX_SOUNDS = 8;

enum SoundIdentifier {
	SOUND_01 = 0,
	SOUND_02,
	SOUND_03,
	SOUND_04,
	SOUND_05,
	SOUND_06,
	SOUND_07,
	SOUND_08
};

template<class T>
static inline bool almost_equal( const T& a, const T& b, const T& d = 1e-10 ) {
	return fabs( b - a ) < d;
}

template<class T>
static inline T adjust_value( const T& value, const T& min, const T& max ) {
	return value < min ? min : ( value > max ? max : value );
}

static inline int adjust_value( const int& value, const short& min, const short& max ) {
	return value < min ? min : ( value > max ? max : value );
}

static inline floating_t controller_to_decimal( unsigned char value, const floating_t& a, const floating_t& b ) {
	return ( ( b - a ) * ( value / 127. ) ) + a;
}

static inline floating_t controller_to_decimal_center( unsigned char value, const floating_t& a, const floating_t& b ) {
	return value == 63 ? ( ( b - a ) / 2. ) + a : controller_to_decimal( value, a, b );
}

static inline floating_t controller_to_range( const floating_t& value, const floating_t& a, const floating_t& b, const floating_t& max_range ) {
	return ( value / max_range ) * ( ( b - a ) / 2 );
}

static inline floating_t controller_to_percentage( unsigned char value ) {
	return controller_to_decimal( value, 0, 1 );
}

static inline bool controller_to_bool( unsigned char value ) {
	return value >= 64;
}

//MIDI_CONT_VOLUME                    /**< Controller: 07 Volume       0.    .. 2.   dB   Center                  */
//MIDI_CONT_STRETCH                   /**< Controller: 09 Stretch     -1.    .. +1.  %    Center                  */
//MIDI_CONT_TRANSPOSE                 /**< Controller: 14 Transpose   -48    .. +48  st   Center     Fijar        */
//MIDI_CONT_LINKED                    /**< Controller: 15 Linked       0     .. 1    bool                         */
//MIDI_CONT_SOUND_START               /**< Controller: 20 Start        0     .. 100  ms                           */
//MIDI_CONT_SOUND_START_SOFT          /**< Controller: 21  Soft Start  0     .. 1    bool            Invertir?    */
//MIDI_CONT_SOUND_TRANSPOSE           /**< Controller: 22 Transpose   -48    .. +48  st   Center                  */
//MIDI_CONT_SOUND_TRANSPOSE_VELOCITY  /**< Controller: 23  Velocity    0.    .. 1.   %                            */
//MIDI_CONT_SOUND_TRANSPOSE_RANDOM    /**< Controller: 24  Random      0.    .. 1.   %                            */
//MIDI_CONT_SOUND_STRETCH             /**< Controller: 25 Stretch     -1.    .. +1.  %    Center                  */
//MIDI_CONT_SOUND_STRETCH_VELOCITY    /**< Controller: 26  Velocity    0.    .. 1.   %                            */
//MIDI_CONT_SOUND_STRETCH_TYPE        /**< Controller: 27  Type        0     .. 1    enum [A,B...]                */
//MIDI_CONT_SOUND_OVER_DRIVE          /**< Controller: 28 Drive        0     .. 10   ???                          */
//MIDI_CONT_SOUND_OVER_DRIVE_ACTIVE   /**< Controller: 29  Saturation  0     .. 1    bool            Invertir?    */
//MIDI_CONT_SOUND_FREQUENCY_FREQUENCY /**< Controller: 48 Freq         30 Hz .. 18.5 kHz ???                      */
//MIDI_CONT_SOUND_FREQUENCY_ACTIVE    /**< Controller: 49  Filter      0     .. 1    bool                         */
//MIDI_CONT_SOUND_FREQUENCY_TYPE      /**< Controller: 50  Type        0     .. 4    enum [lp,hp,bp1,bp2,notch]   */
//MIDI_CONT_SOUND_FREQUENCY_VELOCITY  /**< Controller: 51  Velocity    0.    .. 1.   %                            */
//MIDI_CONT_SOUND_FREQUENCY_RANDOM    /**< Controller: 52  Random      0.    .. 1.   %                            */
//MIDI_CONT_SOUND_FREQUENCY_RESONANCE /**< Controller: 53 Resonance    0     .. 10   ???                          */
//MIDI_CONT_SOUND_DECAY               /**< Controller: 54 Decay        0     .. 10   s                            */
//MIDI_CONT_SOUND_DECAY_TYPE          /**< Controller: 55  Type        0     .. 2    enum [infinite,gate,trigger] */
//MIDI_CONT_SOUND_PANNING             /**< Controller: 56 Panning      -1.   .. -1.  % Center                     */
//MIDI_CONT_SOUND_PANNING_VELOCITY    /**< Controller: 57  Velocity    0.    .. 1.   %                            */
//MIDI_CONT_SOUND_PANNING_RANDOM      /**< Controller: 58  Random      0.    .. 1.   %                            */
//MIDI_CONT_SOUND_VOLUME              /**< Controller: 59 Volume       0.    .. 2.   dB Center                    */
//MIDI_CONT_SOUND_VOLUME_VELOCITY     /**< Controller: 60  Velocity    0.    .. 1.   %                            */
//MIDI_CONT_SOUND_MUTED               /**< Controller: 61 Mute         0     .. 1    bool                         */
//MIDI_CONT_SOUND_SOLOED = 62         /**< Controller: 62 Solo         0     .. 1    bool                         */

static const unsigned char MIDI_CONT_BASE_CHANNEL = 9; // 10
static const unsigned char MIDI_CONT_INIT = 8;
static const unsigned char MIDI_CONT_STRIDE = 14;
static const unsigned char MIDI_CONT_ENGINE_LINKED_C10 = 2;
static const unsigned char MIDI_CONT_ENGINE_TRANSPOSE_C10 = 4;
static const unsigned char MIDI_CONT_ENGINE_STRETCH_C10 = 5;
static const unsigned char MIDI_CONT_ENGINE_VOLUME_C10 = 7;
static const unsigned char MIDI_CONT_SOUND_START_C10 = 0;
static const unsigned char MIDI_CONT_SOUND_START_SOFT_C10 = 1;
static const unsigned char MIDI_CONT_SOUND_TRANSPOSE_C10 = 2;
static const unsigned char MIDI_CONT_SOUND_STRETCH_C10 = 3;
static const unsigned char MIDI_CONT_SOUND_OVER_DRIVE_C10 = 4;
static const unsigned char MIDI_CONT_SOUND_OVER_DRIVE_ACTIVE_C10 = 5;
static const unsigned char MIDI_CONT_SOUND_FILTER_FREQUENCY_C10 = 6;
static const unsigned char MIDI_CONT_SOUND_FILTER_ACTIVE_C10 = 7;
static const unsigned char MIDI_CONT_SOUND_FILTER_RESONANCE_C10 = 8;
static const unsigned char MIDI_CONT_SOUND_DECAY_C10 = 9;
static const unsigned char MIDI_CONT_SOUND_PANNING_C10 = 10;
static const unsigned char MIDI_CONT_SOUND_VOLUME_C10 = 11;
static const unsigned char MIDI_CONT_ENGINE_ALL_SOUND_OFF_C10 = 120;
static const unsigned char MIDI_CONT_ENGINE_ALL_CONTROLLERS_OFF_C10 = 121;
static const unsigned char MIDI_CONT_ENGINE_LOCAL_KEYBOARD_C10 = 122;
static const unsigned char MIDI_CONT_ENGINE_ALL_NOTES_OFF_C10 = 123;
static const unsigned char MIDI_CONT_ENGINE_OMNI_MODE_OFF_C10 = 124;
static const unsigned char MIDI_CONT_ENGINE_OMNI_MODE_ON_C10 = 125;
static const unsigned char MIDI_CONT_ENGINE_MONO_OPERATION_C10 = 126;
static const unsigned char MIDI_CONT_ENGINE_POLY_OPERATION_C10 = 127;
static const unsigned char MIDI_CONT_SOUND_TRANSPOSE_VELOCITY_C11 = 0;
static const unsigned char MIDI_CONT_SOUND_TRANSPOSE_RANDOM_C11 = 1;
static const unsigned char MIDI_CONT_SOUND_STRETCH_VELOCITY_C11 = 2;
static const unsigned char MIDI_CONT_SOUND_STRETCH_TYPE_C11 = 3;
static const unsigned char MIDI_CONT_SOUND_FILTER_TYPE_C11 = 4;
static const unsigned char MIDI_CONT_SOUND_FILTER_VELOCITY_C11 = 5;
static const unsigned char MIDI_CONT_SOUND_FILTER_RANDOM_C11 = 6;
static const unsigned char MIDI_CONT_SOUND_DECAY_TYPE_C11 = 7;
static const unsigned char MIDI_CONT_SOUND_PANNING_VELOCITY_C11 = 8;
static const unsigned char MIDI_CONT_SOUND_PANNING_RANDOM_C11 = 9;
static const unsigned char MIDI_CONT_SOUND_VOLUME_VELOCITY_C11 = 10;
static const unsigned char MIDI_CONT_SOUND_MUTED_C11 = 11;
static const unsigned char MIDI_CONT_SOUND_SOLOED_C11 = 12;

enum NoteMapType {
	NOTE_MAP_KEYBOARD = 0,
	NOTE_MAP_PADS
};

static const int MIDI_NOTE_MAP[][ MAX_SOUNDS ] = {
	{ 0, 2, 4, 5, 7, 9, 11, 12 },
	{ 0, 1, 2, 3, 4, 5,  6,  7 }
};

static const NoteMapType NOTE_MAP_DEF_TYPE = NOTE_MAP_KEYBOARD;

class MidiNoteOffset {
	unsigned char id;
	int base_note;
	int note;
	NoteMapType note_map;
protected:
	void update() {
		note = base_note + MIDI_NOTE_MAP[ note_map ][ id ];
	}
public:
	MidiNoteOffset() : id( 0 ), note_map( NOTE_MAP_DEF_TYPE ) {}
	virtual ~MidiNoteOffset() {}
	void set_id( const unsigned char& id ) {
		this->id = id;
		update();
	}
	void set_base_note( const unsigned char& base_note ) {
		this->base_note = base_note;
		update();
	}
	void set_note_map( const NoteMapType& note_map ) {
		this->note_map = note_map;
		update();
	}
	bool test( const int& note ) const {
		return this->note == note;
	}
};

class MidiControllerOffset {
	unsigned char id;
	unsigned char init;
	unsigned char stride;
	unsigned char base;
protected:
	void update() {
		base = ( id * stride ) + init;
	}
public:
	MidiControllerOffset() : id( 0 ), init( MIDI_CONT_INIT ),
		stride( MIDI_CONT_STRIDE ), base( init ) {}
	void set_id( const unsigned char& id ) {
		this->id = id;
		update();
	}
	virtual ~MidiControllerOffset() {}
	bool test( const unsigned char& controller, const unsigned char& offset ) const {
		return ( base + offset ) == controller;
	}
};

} // namespace util

#endif /* UTIL_H_ */
