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

#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "../tinyxml/tinyxml.h"
#include "util.h"
#include "filtering.h"
#include "envelope.h"
#include "modulation.h"

namespace persistence {

namespace tag {
static const std::string REPULSE = "Repulse";
static const std::string WAVES = "Waves";
static const std::string WAVE = "Wave";
static const std::string PRESETS = "Presets";
static const std::string PRESET = "Preset";
static const std::string ENGINE = "Engine";
static const std::string SOUNDS = "Sounds";
static const std::string SOUND = "Sound";
}

namespace attr {
static const std::string VERSION_ = "version";
static const std::string NAME = "name";
static const std::string FILE = "file";
static const std::string LINKED = "linked";
static const std::string START = "start";
static const std::string START_SOFT = "startSoft";
static const std::string TRANSPOSE = "transpose";
static const std::string TRANSPOSE_VELOCITY = "transposeVelocity";
static const std::string TRANSPOSE_RANDOM = "transposeRandom";
static const std::string STRETCH = "stretch";
static const std::string STRETCH_VELOCITY = "stretchVelocity";
static const std::string STRETCH_TYPE = "stretchType";
static const std::string OVER_DRIVE = "overDrive";
static const std::string OVER_DRIVE_ACTIVE = "overDriveActive";
static const std::string FILTER_FREQUENCY = "filterFrequency";
static const std::string FILTER_ACTIVE = "filterActive";
static const std::string FILTER_TYPE = "filterType";
static const std::string FILTER_VELOCITY = "filterVelocity";
static const std::string FILTER_RANDOM = "filterRandom";
static const std::string FILTER_RESONANCE = "filterResonance";
static const std::string DECAY = "decay";
static const std::string DECAY_TYPE = "decayType";
static const std::string PANNING = "panning";
static const std::string PANNING_VELOCITY = "panningVelocity";
static const std::string PANNING_RANDOM = "panningRandom";
static const std::string VOLUME = "volume";
static const std::string VOLUME_VELOCITY = "volumeVelocity";
static const std::string MUTED = "muted";
static const std::string SOLOED = "soloed";
static const std::string SELECTED_PRESET = "selectedPreset";
static const std::string BASE_CHANNEL = "baseChannel";
static const std::string BASE_NOTE = "baseNote";
static const std::string LOCAL_KEYBOARD = "localKeyboard";
static const std::string ALTERNATE_WHEEL = "alternateWheel";
static const std::string OMNI = "omni";
static const std::string MONO = "mono";
static const std::string NOTE_MAP = "noteMap";
}

static inline std::string bool_to_xml( const bool& value ) {
	std::ostringstream o;
	o << std::boolalpha << value;
	return o.str();
}

static inline bool xml_to_bool( const std::string& value ) {
	bool ret;
	std::stringstream o;
	o << value;
	o >> std::boolalpha >> ret;
	return ret;
}

static inline std::string int_to_xml( const int& value ) {
	std::ostringstream o;
	o << value;
	return o.str();
}

static inline int xml_to_int( const std::string& value ) {
	int ret;
	std::stringstream o;
	o << value;
	o >> ret;
	return ret;
}

static inline std::string floating_to_xml( const util::floating_t& value ) {
	std::ostringstream o;
	o << std::fixed << value;
	return o.str();
}

static inline util::floating_t xml_to_floating( const std::string& value ) {
	util::floating_t ret;
	std::stringstream o;
	o << value;
	o >> ret;
	return ret;
}

static const std::string NOTE_MAP_KEYBOARD = "keyboard";
static const std::string NOTE_MAP_PADS = "pads";

static inline std::string note_map_type_to_xml( const util::NoteMapType& value ) {
	std::string ret = NOTE_MAP_KEYBOARD;
	switch ( value ) {
	case util::NOTE_MAP_KEYBOARD:
		ret = NOTE_MAP_KEYBOARD;
		break;
	case util::NOTE_MAP_PADS:
		ret = NOTE_MAP_PADS;
		break;
	}
	return ret;
}

static inline util::NoteMapType xml_to_note_map_type( const std::string& value ) {
	util::NoteMapType ret = util::NOTE_MAP_DEF_TYPE;
	if ( NOTE_MAP_KEYBOARD == value ) {
		ret = util::NOTE_MAP_KEYBOARD;
	} else if ( NOTE_MAP_PADS == value ) {
		ret = util::NOTE_MAP_PADS;
	}
	return ret;
}

static const std::string DECAY_TYPE_INFINITE = "infinite";
static const std::string DECAY_TYPE_TRIGGER = "trigger";
static const std::string DECAY_TYPE_GATE = "gate";

static inline std::string decay_type_to_xml( const envelope::DecayType& value ) {
	std::string ret = DECAY_TYPE_INFINITE;
	switch ( value ) {
	case envelope::DECAY_TYPE_INFINITE:
		ret = DECAY_TYPE_INFINITE;
		break;
	case envelope::DECAY_TYPE_TRIGGER:
		ret = DECAY_TYPE_TRIGGER;
		break;
	case envelope::DECAY_TYPE_GATE:
		ret = DECAY_TYPE_GATE;
		break;
	}
	return ret;
}

static inline envelope::DecayType xml_to_decay_type( const std::string& value ) {
	envelope::DecayType ret = envelope::DECAY_DEF_TYPE;
	if ( DECAY_TYPE_INFINITE == value ) {
		ret = envelope::DECAY_TYPE_INFINITE;
	} else if ( DECAY_TYPE_TRIGGER == value ) {
		ret = envelope::DECAY_TYPE_TRIGGER;
	} else if ( DECAY_TYPE_GATE == value ) {
		ret = envelope::DECAY_TYPE_GATE;
	}
	return ret;
}

static const std::string FREQUENCY_FILTER_TYPE_LOW_PASS = "lp";
static const std::string FREQUENCY_FILTER_TYPE_HIGH_PASS = "hp";
static const std::string FREQUENCY_FILTER_TYPE_BAND_PASS_1 = "bp1";
static const std::string FREQUENCY_FILTER_TYPE_BAND_PASS_2 = "bp2";
static const std::string FREQUENCY_FILTER_TYPE_NOTCH = "notch";

static inline std::string filter_type_to_xml( const filtering::FrequencyFilterType& value ) {
	std::string ret = FREQUENCY_FILTER_TYPE_LOW_PASS;
	switch ( value ) {
	case filtering::FREQUENCY_FILTER_TYPE_LOW_PASS:
		ret = FREQUENCY_FILTER_TYPE_LOW_PASS;
		break;
	case filtering::FREQUENCY_FILTER_TYPE_HIGH_PASS:
		ret = FREQUENCY_FILTER_TYPE_HIGH_PASS;
		break;
	case filtering::FREQUENCY_FILTER_TYPE_BAND_PASS_1:
		ret = FREQUENCY_FILTER_TYPE_BAND_PASS_1;
		break;
	case filtering::FREQUENCY_FILTER_TYPE_BAND_PASS_2:
		ret = FREQUENCY_FILTER_TYPE_BAND_PASS_2;
		break;
	case filtering::FREQUENCY_FILTER_TYPE_NOTCH:
		ret = FREQUENCY_FILTER_TYPE_NOTCH;
		break;
	}
	return ret;
}

static inline filtering::FrequencyFilterType xml_to_filter_type( const std::string& value ) {
	filtering::FrequencyFilterType ret = filtering::FREQUENCY_FILTER_DEF_TYPE;
	if ( FREQUENCY_FILTER_TYPE_LOW_PASS == value ) {
		ret = filtering::FREQUENCY_FILTER_TYPE_LOW_PASS;
	} else if ( FREQUENCY_FILTER_TYPE_HIGH_PASS == value ) {
		ret = filtering::FREQUENCY_FILTER_TYPE_HIGH_PASS;
	} else if ( FREQUENCY_FILTER_TYPE_BAND_PASS_1 == value ) {
		ret = filtering::FREQUENCY_FILTER_TYPE_BAND_PASS_1;
	} else if ( FREQUENCY_FILTER_TYPE_BAND_PASS_2 == value ) {
		ret = filtering::FREQUENCY_FILTER_TYPE_BAND_PASS_2;
	} else if ( FREQUENCY_FILTER_TYPE_NOTCH == value ) {
		ret = filtering::FREQUENCY_FILTER_TYPE_NOTCH;
	}
	return ret;
}

static const std::string TIME_STRETCH_TYPE_AUTO = "auto";
static const std::string TIME_STRETCH_TYPE_SPEECH = "speech";
static const std::string TIME_STRETCH_TYPE_1 = "type1";
static const std::string TIME_STRETCH_TYPE_2 = "type2";

static inline std::string stretch_type_to_xml( const filtering::TimeStretchType& value ) {
	std::string ret = TIME_STRETCH_TYPE_AUTO;
	switch ( value ) {
	case filtering::TIME_STRETCH_TYPE_AUTO:
		ret = TIME_STRETCH_TYPE_AUTO;
		break;
	case filtering::TIME_STRETCH_TYPE_SPEECH:
		ret = TIME_STRETCH_TYPE_SPEECH;
		break;
	case filtering::TIME_STRETCH_TYPE_1:
		ret = TIME_STRETCH_TYPE_1;
		break;
	case filtering::TIME_STRETCH_TYPE_2:
		ret = TIME_STRETCH_TYPE_2;
		break;
	}
	return ret;
}

static inline filtering::TimeStretchType xml_to_stretch_type( const std::string& value ) {
	filtering::TimeStretchType ret = filtering::TIME_STRETCH_DEF_TYPE;
	if ( TIME_STRETCH_TYPE_AUTO == value ) {
		ret = filtering::TIME_STRETCH_TYPE_AUTO;
	} else if ( TIME_STRETCH_TYPE_SPEECH == value ) {
		ret = filtering::TIME_STRETCH_TYPE_SPEECH;
	} else if ( TIME_STRETCH_TYPE_1 == value ) {
		ret = filtering::TIME_STRETCH_TYPE_1;
	} else if ( TIME_STRETCH_TYPE_2 == value ) {
		ret = filtering::TIME_STRETCH_TYPE_2;
	}
	return ret;
}

class Serializable {
public:
	Serializable() {}
	virtual ~Serializable() {}
	virtual void deserialize( TiXmlElement& element ) = 0;
	virtual const TiXmlElement& serialize( TiXmlElement& element ) const = 0;
	virtual void to_stream( std::ostringstream& o ) const = 0;
	virtual std::string to_string() const {
		std::ostringstream o;
		to_stream( o );
		return o.str();
	}
};

const static std::string BLANK = "";

template <class T>
class ElementVector : public std::vector<T>, public Serializable {
protected:
	virtual const std::string& get_parent_tag() const { return BLANK; }
	virtual const std::string& get_child_tag() const { return BLANK; }
public:
	ElementVector() : std::vector<T>() {}
	virtual ~ElementVector() {}
	void deserialize( TiXmlElement& element ) {
		std::vector<T>::clear();
		T o;
		TiXmlNode* child = 0;
		while ( ( child = element.IterateChildren( get_child_tag(), child ) ) ) {
			if ( child->ToElement() ) {
				o.deserialize( *child->ToElement() );
				push_back( o );
			}
		}
	}
	const TiXmlElement& serialize( TiXmlElement& element ) const {
		typename std::vector<T>::const_iterator it;
		for ( it = std::vector<T>::begin(); it != std::vector<T>::end(); ++it ) {
			TiXmlElement child( get_child_tag() );
			element.InsertEndChild( it->serialize( child ) );
		}
		return element;
	}
	void to_stream( std::ostringstream& o ) const {
		o << "[" << get_parent_tag() << "]" << std::endl;
		typename std::vector<T>::const_iterator it;
		for ( it = std::vector<T>::begin(); it != std::vector<T>::end(); ++it ) {
			it->to_stream( o );
		}
	}
};

class Sound : public Serializable {
    util::floating_t start;
    bool start_soft;
    util::floating_t transpose;
    util::floating_t transpose_velocity;
    util::floating_t transpose_random;
    util::floating_t stretch;
    util::floating_t stretch_velocity;
    filtering::TimeStretchType stretch_type;
    util::floating_t over_drive;
    bool over_drive_active;
    util::floating_t filter_frequency;
    bool filter_active;
    filtering::FrequencyFilterType filter_type;
    util::floating_t filter_velocity;
    util::floating_t filter_random;
    util::floating_t filter_resonance;
    util::floating_t decay;
    envelope::DecayType decay_type;
    util::floating_t panning;
    util::floating_t panning_velocity;
    util::floating_t panning_random;
    util::floating_t volume;
    util::floating_t volume_velocity;
    bool muted;
    bool soloed;
public:
	Sound() :
	    start( filtering::WAVE_DEF_START_TIME ),
	    start_soft( envelope::DEF_SOFT_START ),
	    transpose( filtering::TUNER_DEF_TRANSPOSE ),
	    transpose_velocity( modulation::DEF_VELOCITY ),
	    transpose_random( modulation::DEF_RANDOM ),
    	stretch( filtering::TIME_STRETCH_DEF_STRETCH ),
    	stretch_velocity( modulation::DEF_VELOCITY ),
    	stretch_type( filtering::TIME_STRETCH_DEF_TYPE ),
    	over_drive( filtering::OVER_DRIVE_DEF_DRIVE ),
		over_drive_active( filtering::OVER_DRIVE_DEF_ACTIVE ),
	    filter_frequency( filtering::FREQUENCY_DEF_FREQUENCY ),
	    filter_active( filtering::FREQUENCY_DEF_ACTIVE ),
	    filter_type( filtering::FREQUENCY_FILTER_DEF_TYPE ),
	    filter_velocity( modulation::DEF_VELOCITY ),
	    filter_random( modulation::DEF_RANDOM ),
	    filter_resonance( filtering::FREQUENCY_DEF_ACTIVE ),
	    decay( envelope::DEF_DECAY ),
	    decay_type( envelope::DECAY_DEF_TYPE ),
	    panning( filtering::PANNER_DEF_PANNING ),
	    panning_velocity( modulation::DEF_VELOCITY ),
	    panning_random( modulation::DEF_RANDOM ),
	    volume( filtering::GAIN_DEF_VOLUME ),
	    volume_velocity( modulation::DEF_VELOCITY ),
	    muted( false ),
	    soloed( false ) {}
	virtual ~Sound() {}
	void set_start( const util::floating_t& start ) {
	    this->start = start;
	}
	const util::floating_t& get_start() const {
	    return start;
	}
	void set_start_soft( const bool& start_soft ) {
	    this->start_soft = start_soft;
	}
	const bool& is_start_soft() const {
	    return start_soft;
	}
	void set_transpose( const util::floating_t& transpose ) {
	    this->transpose = transpose;
	}
	const util::floating_t& get_transpose() const {
	    return transpose;
	}
	void set_transpose_velocity( const util::floating_t& transpose_velocity ) {
	    this->transpose_velocity = transpose_velocity;
	}
	const util::floating_t& get_transpose_velocity() const {
	    return transpose_velocity;
	}
	void set_transpose_random( const util::floating_t& transpose_random ) {
	    this->transpose_random = transpose_random;
	}
	const util::floating_t& get_transpose_random() const {
	    return transpose_random;
	}
	void set_stretch( const util::floating_t& stretch ) {
	    this->stretch = stretch;
	}
	const util::floating_t& get_stretch() const {
	    return stretch;
	}
	void set_stretch_velocity( const util::floating_t& stretch_velocity ) {
	    this->stretch_velocity = stretch_velocity;
	}
	const util::floating_t& get_stretch_velocity() const {
	    return stretch_velocity;
	}
	void set_stretch_type( const filtering::TimeStretchType& stretch_type ) {
	    this->stretch_type = stretch_type;
	}
	const filtering::TimeStretchType& get_stretch_type() const {
	    return stretch_type;
	}
	void set_over_drive( const util::floating_t& over_drive ) {
	    this->over_drive = over_drive;
	}
	const util::floating_t& get_over_drive() const {
	    return over_drive;
	}
	void set_over_drive_active( const bool& over_drive_active ) {
	    this->over_drive_active = over_drive_active;
	}
	const bool& is_over_drive_active() const {
	    return over_drive_active;
	}
	void set_filter_frequency( const util::floating_t& filter_frequency ) {
	    this->filter_frequency = filter_frequency;
	}
	const util::floating_t& get_filter_frequency() const {
	    return filter_frequency;
	}
	void set_filter_active( const bool& filter_active ) {
	    this->filter_active = filter_active;
	}
	const bool& is_filter_active() const {
	    return filter_active;
	}
	void set_filter_type( const filtering::FrequencyFilterType& filter_type ) {
	    this->filter_type = filter_type;
	}
	const filtering::FrequencyFilterType& get_filter_type() const {
	    return filter_type;
	}
	void set_filter_velocity( const util::floating_t& filter_velocity ) {
	    this->filter_velocity = filter_velocity;
	}
	const util::floating_t& get_filter_velocity() const {
	    return filter_velocity;
	}
	void set_filter_random( const util::floating_t& filter_random ) {
	    this->filter_random = filter_random;
	}
	const util::floating_t& get_filter_random() const {
	    return filter_random;
	}
	void set_filter_resonance( const util::floating_t& filter_resonance ) {
	    this->filter_resonance = filter_resonance;
	}
	const util::floating_t& get_filter_resonance() const {
	    return filter_resonance;
	}
	void set_decay( const util::floating_t& decay ) {
	    this->decay = decay;
	}
	const util::floating_t& get_decay() const {
	    return decay;
	}
	void set_decay_type( const envelope::DecayType& decay_type ) {
	    this->decay_type = decay_type;
	}
	const envelope::DecayType& get_decay_type() const {
	    return decay_type;
	}
	void set_panning( const util::floating_t& panning ) {
	    this->panning = panning;
	}
	const util::floating_t& get_panning() const {
	    return panning;
	}
	void set_panning_velocity( const util::floating_t& panning_velocity ) {
	    this->panning_velocity = panning_velocity;
	}
	const util::floating_t& get_panning_velocity() const {
	    return panning_velocity;
	}
	void set_panning_random( const util::floating_t& panning_random ) {
	    this->panning_random = panning_random;
	}
	const util::floating_t& get_panning_random() const {
	    return panning_random;
	}
	void set_volume( const util::floating_t& volume ) {
	    this->volume = volume;
	}
	const util::floating_t& get_volume() const {
	    return volume;
	}
	void set_volume_velocity( const util::floating_t& volume_velocity ) {
	    this->volume_velocity = volume_velocity;
	}
	const util::floating_t& get_volume_velocity() const {
	    return volume_velocity;
	}
	void set_muted( const bool& muted ) {
	    this->muted = muted;
	}
	const bool& is_muted() const {
	    return muted;
	}
	void set_soloed( const bool& soloed ) {
	    this->soloed = soloed;
	}
	const bool& is_soloed() const {
	    return soloed;
	}
	void deserialize( TiXmlElement& element );
	const TiXmlElement& serialize( TiXmlElement& element ) const;
	void to_stream( std::ostringstream& o ) const;
};

void Sound::deserialize( TiXmlElement& element ) {
	if ( element.Attribute( attr::START ) ) {
		set_start( xml_to_floating( *element.Attribute( attr::START ) ) );
	}
	if ( element.Attribute( attr::START_SOFT ) ) {
		set_start_soft( xml_to_bool( *element.Attribute( attr::START_SOFT ) ) );
	}
	if ( element.Attribute( attr::TRANSPOSE ) ) {
		set_transpose( xml_to_floating( *element.Attribute( attr::TRANSPOSE ) ) );
	}
	if ( element.Attribute( attr::TRANSPOSE_VELOCITY ) ) {
		set_transpose_velocity( xml_to_floating( *element.Attribute( attr::TRANSPOSE_VELOCITY ) ) );
	}
	if ( element.Attribute( attr::TRANSPOSE_RANDOM ) ) {
		set_transpose_random( xml_to_floating( *element.Attribute( attr::TRANSPOSE_RANDOM ) ) );
	}
	if ( element.Attribute( attr::STRETCH ) ) {
		set_stretch( xml_to_floating( *element.Attribute( attr::STRETCH ) ) );
	}
	if ( element.Attribute( attr::STRETCH_VELOCITY ) ) {
		set_stretch_velocity( xml_to_floating( *element.Attribute( attr::STRETCH_VELOCITY ) ) );
	}
	if ( element.Attribute( attr::STRETCH_TYPE ) ) {
		set_stretch_type( xml_to_stretch_type( *element.Attribute( attr::STRETCH_TYPE ) ) );
	}
	if ( element.Attribute( attr::OVER_DRIVE ) ) {
		set_over_drive( xml_to_floating( *element.Attribute( attr::OVER_DRIVE ) ) );
	}
	if ( element.Attribute( attr::OVER_DRIVE_ACTIVE ) ) {
		set_over_drive_active( xml_to_bool( *element.Attribute( attr::OVER_DRIVE_ACTIVE ) ) );
	}
	if ( element.Attribute( attr::FILTER_FREQUENCY ) ) {
		set_filter_frequency( xml_to_floating( *element.Attribute( attr::FILTER_FREQUENCY ) ) );
	}
	if ( element.Attribute( attr::FILTER_ACTIVE ) ) {
		set_filter_active( xml_to_bool( *element.Attribute( attr::FILTER_ACTIVE ) ) );
	}
	if ( element.Attribute( attr::FILTER_TYPE ) ) {
		set_filter_type( xml_to_filter_type( *element.Attribute( attr::FILTER_TYPE ) ) );
	}
	if ( element.Attribute( attr::FILTER_VELOCITY ) ) {
		set_filter_velocity( xml_to_floating( *element.Attribute( attr::FILTER_VELOCITY ) ) );
	}
	if ( element.Attribute( attr::FILTER_RANDOM ) ) {
		set_filter_random( xml_to_floating( *element.Attribute( attr::FILTER_RANDOM ) ) );
	}
	if ( element.Attribute( attr::FILTER_RESONANCE ) ) {
		set_filter_resonance( xml_to_floating( *element.Attribute( attr::FILTER_RESONANCE ) ) );
	}
	if ( element.Attribute( attr::DECAY ) ) {
		set_decay( xml_to_floating( *element.Attribute( attr::DECAY ) ) );
	}
	if ( element.Attribute( attr::DECAY_TYPE ) ) {
		set_decay_type( xml_to_decay_type( *element.Attribute( attr::DECAY_TYPE ) ) );
	}
	if ( element.Attribute( attr::PANNING ) ) {
		set_panning( xml_to_floating( *element.Attribute( attr::PANNING ) ) );
	}
	if ( element.Attribute( attr::PANNING_VELOCITY ) ) {
		set_panning_velocity( xml_to_floating( *element.Attribute( attr::PANNING_VELOCITY ) ) );
	}
	if ( element.Attribute( attr::PANNING_RANDOM ) ) {
		set_panning_random( xml_to_floating( *element.Attribute( attr::PANNING_RANDOM ) ) );
	}
	if ( element.Attribute( attr::VOLUME ) ) {
		set_volume( xml_to_floating( *element.Attribute( attr::VOLUME ) ) );
	}
	if ( element.Attribute( attr::VOLUME_VELOCITY ) ) {
		set_volume_velocity( xml_to_floating( *element.Attribute( attr::VOLUME_VELOCITY ) ) );
	}
	if ( element.Attribute( attr::MUTED ) ) {
		set_muted( xml_to_bool( *element.Attribute( attr::MUTED ) ) );
	}
	if ( element.Attribute( attr::SOLOED ) ) {
		set_soloed( xml_to_bool( *element.Attribute( attr::SOLOED ) ) );
	}
}

const TiXmlElement& Sound::serialize( TiXmlElement& element ) const {
	element.SetAttribute( attr::START, floating_to_xml( get_start() ) );
	element.SetAttribute( attr::START_SOFT, bool_to_xml( is_start_soft() ) );
	element.SetAttribute( attr::TRANSPOSE, floating_to_xml( get_transpose() ) );
	element.SetAttribute( attr::TRANSPOSE_VELOCITY, floating_to_xml( get_transpose_velocity() ) );
	element.SetAttribute( attr::TRANSPOSE_RANDOM, floating_to_xml( get_transpose_random() ) );
	element.SetAttribute( attr::STRETCH, floating_to_xml( get_stretch() ) );
	element.SetAttribute( attr::STRETCH_VELOCITY, floating_to_xml( get_stretch_velocity() ) );
	element.SetAttribute( attr::STRETCH_TYPE, stretch_type_to_xml( get_stretch_type() ) );
	element.SetAttribute( attr::OVER_DRIVE, floating_to_xml( get_over_drive() ) );
	element.SetAttribute( attr::OVER_DRIVE_ACTIVE, bool_to_xml( is_over_drive_active() ) );
	element.SetAttribute( attr::FILTER_FREQUENCY, floating_to_xml( get_filter_frequency() ) );
	element.SetAttribute( attr::FILTER_ACTIVE, bool_to_xml( is_filter_active() ) );
	element.SetAttribute( attr::FILTER_TYPE, filter_type_to_xml( get_filter_type() ) );
	element.SetAttribute( attr::FILTER_VELOCITY, floating_to_xml( get_filter_velocity() ) );
	element.SetAttribute( attr::FILTER_RANDOM, floating_to_xml( get_filter_random() ) );
	element.SetAttribute( attr::FILTER_RESONANCE, floating_to_xml( get_filter_resonance() ) );
	element.SetAttribute( attr::DECAY, floating_to_xml( get_decay() ) );
	element.SetAttribute( attr::DECAY_TYPE, decay_type_to_xml( get_decay_type() ) );
	element.SetAttribute( attr::PANNING, floating_to_xml( get_panning() ) );
	element.SetAttribute( attr::PANNING_VELOCITY, floating_to_xml( get_panning_velocity() ) );
	element.SetAttribute( attr::PANNING_RANDOM, floating_to_xml( get_panning_random() ) );
	element.SetAttribute( attr::VOLUME, floating_to_xml( get_volume() ) );
	element.SetAttribute( attr::VOLUME_VELOCITY, floating_to_xml( get_volume_velocity() ) );
	element.SetAttribute( attr::MUTED, bool_to_xml( is_muted() ) );
	element.SetAttribute( attr::SOLOED, bool_to_xml( is_soloed() ) );
    return element;
}

void Sound::to_stream( std::ostringstream& o ) const {
	o << "[" << tag::SOUND << "]" << std::endl;
	o << " start: " << std::fixed << get_start() << std::endl;
	o << " start_soft: " << std::boolalpha << is_start_soft() << std::endl;
	o << " transpose: " << std::fixed << get_transpose() << std::endl;
	o << " transpose_velocity: " << std::fixed << get_transpose_velocity() << std::endl;
	o << " transpose_random: " << std::fixed << get_transpose_random() << std::endl;
	o << " stretch: " << std::fixed << get_stretch() << std::endl;
	o << " stretch_velocity: " << std::fixed << get_stretch_velocity() << std::endl;
	o << " stretch_type: " << std::fixed << get_stretch_type() << std::endl;
	o << " over_drive: " << std::fixed << get_over_drive() << std::endl;
	o << " over_drive_active: " << std::boolalpha << is_over_drive_active() << std::endl;
	o << " filter_frequency: " << std::fixed << get_filter_frequency() << std::endl;
	o << " filter_active: " << std::boolalpha << is_filter_active() << std::endl;
	o << " filter_type: " << filter_type_to_xml( get_filter_type() ) << std::endl;
	o << " filter_velocity: " << std::fixed << get_filter_velocity() << std::endl;
	o << " filter_random: " << std::fixed << get_filter_random() << std::endl;
	o << " filter_resonance: " << std::fixed << get_filter_resonance() << std::endl;
	o << " decay: " << std::fixed << get_decay() << std::endl;
	o << " decay_type: " << decay_type_to_xml( get_decay_type() ) << std::endl;
	o << " panning: " << std::fixed << get_panning() << std::endl;
	o << " panning_velocity: " << std::fixed << get_panning_velocity() << std::endl;
	o << " panning_random: " << std::fixed << get_panning_random() << std::endl;
	o << " volume: " << std::fixed << get_volume() << std::endl;
	o << " volume_velocity: " << std::fixed << get_volume_velocity() << std::endl;
	o << " muted: " << std::boolalpha << is_muted() << std::endl;
	o << " soloed: " << std::boolalpha << is_soloed() << std::endl;
}

class Sounds : public ElementVector<Sound> {
protected:
	const std::string& get_parent_tag() const { return tag::SOUNDS; }
	const std::string& get_child_tag() const { return tag::SOUND; }
public:
	Sounds() : ElementVector<Sound>() {
		for ( size_t i = size(); i < util::MAX_SOUNDS; ++i ) {
			push_back( Sound() );
		}
	}
	virtual ~Sounds() {}
	void deserialize( TiXmlElement& element ) {
		ElementVector<Sound>::deserialize( element );
		for ( size_t i = size(); i < util::MAX_SOUNDS; ++i ) {
			push_back( Sound() );
		}
	}
};

class Engine : public Serializable {
	util::floating_t volume;
	util::floating_t stretch;
	util::floating_t transpose;
	bool linked;
	int base_channel;
	int base_note;
	bool local_keyboard;
	bool alternate_wheel;
	bool omni;
	bool mono;
	util::NoteMapType note_map;
public:
	Engine() :
		volume( filtering::GAIN_DEF_VOLUME ),
		stretch( filtering::TIME_STRETCH_DEF_STRETCH ),
		transpose( filtering::TUNER_DEF_TRANSPOSE ),
		linked( false ),
		base_channel( 10 ),
		base_note( midi::MIDDLE_C ),
		local_keyboard( true ),
		alternate_wheel( false ),
		omni( true ),
		mono( false ),
		note_map( util::NOTE_MAP_DEF_TYPE ) {}
	virtual ~Engine() {}
	void set_note_map( const util::NoteMapType& note_map ) {
	    this->note_map = note_map;
	}
	const util::NoteMapType& get_note_map() const {
	    return note_map;
	}
	void set_volume( const util::floating_t& volume ) {
	    this->volume = volume;
	}
	const util::floating_t& get_volume() const {
	    return volume;
	}
	void set_stretch( const util::floating_t& stretch ) {
	    this->stretch = stretch;
	}
	const util::floating_t& get_stretch() const {
	    return stretch;
	}
	void set_transpose( const util::floating_t& transpose ) {
	    this->transpose = transpose;
	}
	const util::floating_t& get_transpose() const {
	    return transpose;
	}
	void set_linked( const bool& linked ) {
	    this->linked = linked;
	}
	const bool& is_linked() const {
	    return linked;
	}
	void set_base_channel( const int& base_channel ) {
	    this->base_channel = base_channel;
	}
	const int& get_base_channel() const {
	    return base_channel;
	}
	void set_base_note( const int& base_note ) {
	    this->base_note = base_note;
	}
	const int& get_base_note() const {
	    return base_note;
	}
	void set_local_keyboard( const bool& local_keyboard ) {
	    this->local_keyboard = local_keyboard;
	}
	const bool& is_local_keyboard() const {
	    return local_keyboard;
	}
	void set_alternate_wheel( const bool& alternate_wheel ) {
	    this->alternate_wheel = alternate_wheel;
	}
	const bool& is_alternate_wheel() const {
	    return alternate_wheel;
	}
	void set_omni( const bool& omni ) {
	    this->omni = omni;
	}
	const bool& is_omni() const {
	    return omni;
	}
	void set_mono( const bool& mono ) {
	    this->mono = mono;
	}
	const bool& is_mono() const {
	    return mono;
	}
	void deserialize( TiXmlElement& element ) {
		if ( element.Attribute( attr::VOLUME ) ) {
			set_volume( xml_to_floating( *element.Attribute( attr::VOLUME ) ) );
		}
		if ( element.Attribute( attr::STRETCH ) ) {
			set_stretch( xml_to_floating( *element.Attribute( attr::STRETCH ) ) );
		}
		if ( element.Attribute( attr::TRANSPOSE ) ) {
			set_transpose( xml_to_floating( *element.Attribute( attr::TRANSPOSE ) ) );
		}
		if ( element.Attribute( attr::LINKED ) ) {
			set_linked( xml_to_bool( *element.Attribute( attr::LINKED ) ) );
		}
		if ( element.Attribute( attr::BASE_CHANNEL ) ) {
			set_base_channel( xml_to_int( *element.Attribute( attr::BASE_CHANNEL ) ) );
		}
		if ( element.Attribute( attr::BASE_NOTE ) ) {
			set_base_note( xml_to_int( *element.Attribute( attr::BASE_NOTE ) ) );
		}
		if ( element.Attribute( attr::LOCAL_KEYBOARD ) ) {
			set_local_keyboard( xml_to_bool( *element.Attribute( attr::LOCAL_KEYBOARD ) ) );
		}
		if ( element.Attribute( attr::ALTERNATE_WHEEL ) ) {
			set_alternate_wheel( xml_to_bool( *element.Attribute( attr::ALTERNATE_WHEEL ) ) );
		}
		if ( element.Attribute( attr::OMNI ) ) {
			set_omni( xml_to_bool( *element.Attribute( attr::OMNI ) ) );
		}
		if ( element.Attribute( attr::MONO ) ) {
			set_mono( xml_to_bool( *element.Attribute( attr::MONO ) ) );
		}
		if ( element.Attribute( attr::NOTE_MAP ) ) {
			set_note_map( xml_to_note_map_type( *element.Attribute( attr::NOTE_MAP ) ) );
		}
	}
	const TiXmlElement& serialize( TiXmlElement& element ) const {
		element.SetAttribute( attr::VOLUME, floating_to_xml( get_volume() ) );
		element.SetAttribute( attr::STRETCH, floating_to_xml( get_stretch() ) );
		element.SetAttribute( attr::TRANSPOSE, floating_to_xml( get_transpose() ) );
		element.SetAttribute( attr::LINKED, bool_to_xml( is_linked() ) );
		element.SetAttribute( attr::BASE_CHANNEL, int_to_xml( get_base_channel() ) );
		element.SetAttribute( attr::BASE_NOTE, int_to_xml( get_base_note() ) );
		element.SetAttribute( attr::LOCAL_KEYBOARD, bool_to_xml( is_local_keyboard() ) );
		element.SetAttribute( attr::ALTERNATE_WHEEL, bool_to_xml( is_alternate_wheel() ) );
		element.SetAttribute( attr::OMNI, bool_to_xml( is_omni() ) );
		element.SetAttribute( attr::MONO, bool_to_xml( is_mono() ) );
		element.SetAttribute( attr::NOTE_MAP, note_map_type_to_xml( get_note_map() ) );
		return element;
	}
	void to_stream( std::ostringstream& o ) const {
		o << "[" << tag::ENGINE << "]" << std::endl;
		o << " volume: " << std::fixed << get_volume() << std::endl;
		o << " stretch: " << std::fixed << get_stretch() << std::endl;
		o << " transpose: " << std::fixed << get_transpose() << std::endl;
		o << " linked: " << std::boolalpha << is_linked() << std::endl;
		o << " base_channel: " << get_base_channel() << std::endl;
		o << " base_note: " << get_base_note() << std::endl;
		o << " local_keyboard: " << std::boolalpha << is_local_keyboard() << std::endl;
		o << " alternate_wheel: " << std::boolalpha << is_alternate_wheel() << std::endl;
		o << " omni: " << std::boolalpha << is_omni() << std::endl;
		o << " mono: " << std::boolalpha << is_mono() << std::endl;
		o << " note map: " << note_map_type_to_xml( get_note_map() ) << std::endl;
	}
};

class Preset : public Serializable {
	std::string name;
	Engine engine;
	Sounds sounds;
public:
	Preset() {}
	Preset( const std::string& name ) : name( name ) {}
	virtual ~Preset() {}
	void set_name( const std::string& name ) {
		this->name = name;
	}
	const std::string& get_name() const {
		return name;
	}
	Engine& get_engine() {
		return engine;
	}
	const Engine& get_engine() const {
		return engine;
	}
	Sounds& get_sounds() {
		return sounds;
	}
	const Sounds& get_sounds() const {
		return sounds;
	}
	void deserialize( TiXmlElement& element ) {
		if ( element.Attribute( attr::NAME ) ) {
			set_name( *element.Attribute( attr::NAME ) );
		}
		TiXmlNode* child = 0;
		while ( ( child = element.IterateChildren( tag::ENGINE, child ) ) ) {
			if ( child->ToElement() ) {
				get_engine().deserialize( *child->ToElement() );
				break;
			}
		}
		child = 0;
		while ( ( child = element.IterateChildren( tag::SOUNDS, child ) ) ) {
			if ( child->ToElement() ) {
				get_sounds().deserialize( *child->ToElement() );
				break;
			}
		}
	}
	const TiXmlElement& serialize( TiXmlElement& element ) const {
		element.SetAttribute( attr::NAME, get_name() );
		TiXmlElement engine( tag::ENGINE );
		element.InsertEndChild( get_engine().serialize( engine ) );
		TiXmlElement sounds( tag::SOUNDS );
		element.InsertEndChild( get_sounds().serialize( sounds ) );
		return element;
	}
	void to_stream( std::ostringstream& o ) const {
		o << "[" << tag::PRESET << "]" << std::endl;
		get_engine().to_stream( o );
		get_sounds().to_stream( o );
	}
};

static const std::string PRESET_DEFAULT_NAME = "Default";

class Presets : public ElementVector<Preset> {
protected:
	const std::string& get_parent_tag() const { return tag::PRESETS; }
	const std::string& get_child_tag() const { return tag::PRESET; }
public:
	Presets() : ElementVector<Preset>() {
		if ( empty() ) {
			push_back( Preset() );
			front().set_name( PRESET_DEFAULT_NAME );
		}
	}
	virtual ~Presets() {}
	void deserialize( TiXmlElement& element ) {
		ElementVector<Preset>::deserialize( element );
		if ( empty() ) {
			push_back( Preset() );
			front().set_name( PRESET_DEFAULT_NAME );
		}
	}
};

class Wave {
	std::string file;
public:
	Wave() {}
	virtual ~Wave() {}
	void set_file( const std::string& file ) {
		this->file = file;
	}
	const std::string& get_file() const {
		return file;
	}
	void deserialize( TiXmlElement& element ) {
		if ( element.Attribute( attr::FILE ) ) {
			set_file( *element.Attribute( attr::FILE ) );
		}
	}
	const TiXmlElement& serialize( TiXmlElement& element ) const {
		element.SetAttribute( attr::FILE, get_file() );
		return element;
	}
	void to_stream( std::ostringstream& o ) const {
		o << "[" << tag::WAVE << "]" << std::endl;
		o << " file: " << get_file() << std::endl;
	}
};

class Waves : public ElementVector<Wave> {
protected:
	const std::string& get_parent_tag() const { return tag::WAVES; }
	const std::string& get_child_tag() const { return tag::WAVE; }
public:
	Waves() : ElementVector<Wave>() {
		for ( size_t i = size(); i < util::MAX_SOUNDS; ++i ) {
			push_back( Wave() );
		}
	}
	virtual ~Waves() {}
	void deserialize( TiXmlElement& element ) {
		ElementVector<Wave>::deserialize( element );
		for ( size_t i = size(); i < util::MAX_SOUNDS; ++i ) {
			push_back( Wave() );
		}
	}
};

class Repulse : public Serializable {
	Waves waves;
	Presets presets;
    int selected_preset;
    std::string version;
public:
	Repulse() : selected_preset( 0 ), version( util::VERSION_ ) {}
	virtual ~Repulse() {}
	Waves& get_waves() {
		return waves;
	}
	void set_version( const std::string& version ) {
	    this->version = version;
	}
	const std::string& get_version() const {
	    return version;
	}
	void set_selected_preset( const int& selected_preset ) {
	    this->selected_preset = selected_preset;
	}
	const int& get_selected_preset() const {
	    return selected_preset;
	}
	const Waves& get_waves() const {
		return waves;
	}
	Presets& get_presets() {
		return presets;
	}
	const Presets& get_presets() const {
		return presets;
	}
	void deserialize( TiXmlElement& element ) {
		if ( element.Attribute( attr::SELECTED_PRESET ) ) {
			set_selected_preset( xml_to_int( *element.Attribute( attr::SELECTED_PRESET ) ) );
		}
		if ( element.Attribute( attr::VERSION_ ) ) {
			set_version( *element.Attribute( attr::VERSION_ ) );
		}
		TiXmlNode* child = 0;
		while ( ( child = element.IterateChildren( tag::WAVES, child ) ) ) {
			if ( child->ToElement() ) {
				get_waves().deserialize( *child->ToElement() );
				break;
			}
		}
		child = 0;
		while ( ( child = element.IterateChildren( tag::PRESETS, child ) ) ) {
			if ( child->ToElement() ) {
				get_presets().deserialize( *child->ToElement() );
				break;
			}
		}
	}
	const TiXmlElement& serialize( TiXmlElement& element ) const {
		element.SetAttribute( attr::VERSION_, get_version() );
		element.SetAttribute( attr::SELECTED_PRESET, int_to_xml( get_selected_preset() ) );
		TiXmlElement waves( tag::WAVES );
		element.InsertEndChild( get_waves().serialize( waves ) );
		TiXmlElement presets( tag::PRESETS );
		element.InsertEndChild( get_presets().serialize( presets ) );
		return element;
	}
	void to_stream( std::ostringstream& o ) const {
		o << "[" << tag::REPULSE << "]" << std::endl;
		o << " version: " << get_version() << std::endl;
		o << " selected_preset: " << get_selected_preset() << std::endl;
		get_waves().to_stream( o );
		get_presets().to_stream( o );
	}
};

template <class T>
class Document {
	std::string file;
	T root;
protected:
	virtual const std::string& get_tag_name() const { return BLANK; }
public:
	Document() {}
	Document( const std::string& file ) : file( file ) {}
	virtual ~Document() {}
	void set_file( const std::string& file ) { this->file = file; }
	const std::string& get_file() const { return file; }
	void load() {
		TiXmlDocument doc;
		if ( doc.LoadFile( get_file() ) ) {
			get_root().deserialize( *doc.RootElement() );
		}
	}
	void save() const {
		TiXmlDocument doc;
		TiXmlElement root( get_tag_name() );
		doc.InsertEndChild( get_root().serialize( root ) );
		doc.SaveFile( get_file() );
	}
	const T& get_root() const { return root; }
	T& get_root() { return root; }
};

class RepulseDocument : public Document<Repulse> {
protected:
	virtual const std::string& get_tag_name() const { return tag::REPULSE; }
public:
	RepulseDocument() : Document<Repulse>() {}
	RepulseDocument( const std::string& file ) : Document<Repulse>( file ) {}
	virtual ~RepulseDocument() {}
};

} // namespace persistence

#endif /* PERSISTENCE_H_ */
