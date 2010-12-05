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

#ifndef UI_H_
#define UI_H_

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ncurses.h>
#include "repulse.h"
#include "persistence.h"

namespace ui {

class Window {
	WINDOW* window;
public:
	Window() : window( 0 ) {}
	Window( const int& width, const int& height, const int& x = 0, const int& y = 0 ) :
		window( subwin( stdscr, height, width, y, x ) ) {
		assert( window != 0 );
	}
	virtual ~Window() { delwin( window ); }
	WINDOW* get_window() const { return window; }
	virtual void draw() {
		//touchwin( window );
		wnoutrefresh( window );
	}
};

typedef std::vector<Window*> WindowVector;
typedef std::vector<std::string> StringVector;
typedef std::vector<StringVector> BlockVector;

static const int BUTTON_HEIGHT = 1;
static const int HELPER_WIDTH  = 10;
static const int SOUND_WIDTH   = 10;
static const int SOUND_HEIGHT  = 26;
static const int GRID_HEIGHT  = SOUND_HEIGHT + BUTTON_HEIGHT;
static const int HEADER_HEIGHT = 2;
static const int BUTTON_WIDTH = SOUND_WIDTH;
static const int PRESET_ROW = 28;
static const int PRESET_NAME_WIDTH = BUTTON_WIDTH * 6;
static const int MAX_COLUMNS = 8;
static const util::floating_t CLICK_DELAY = 1.;
static const util::floating_t FLASH_DELAY = 0.25;

enum ColorType {
	ROW_A_NAME = 1,
	ROW_A_VALUE,
	ROW_B_NAME,
	ROW_B_VALUE,
	HEADER_SOUND,
	HEADER_FILE,
	HEADER_TITLE,
	HEADER_VERSION,
	HEADER_PLAYING,
	CELL_EDITING,
	BUTTON_NORMAL,
	PRESET_NAME,
	BUTTON_ON,
	BUTTON_OFF,
	BUTTON_ROTATORY
};

class Color {
	WINDOW* window;
	ColorType color;
public:
	Color( WINDOW* window, const ColorType& color ) :
		window( window ), color( color ) {
		wattron( window, COLOR_PAIR( color ) );
	}
	virtual ~Color() {
		wattroff( window, COLOR_PAIR( color ) );
	}
};

template <unsigned long T>
class Attribute {
	WINDOW* window;
public:
	Attribute( WINDOW* window ) : window( window ) { wattron( window, T ); }
	virtual ~Attribute() { wattroff( window, T ); }
};

typedef Attribute<A_REVERSE> Reverse;
typedef Attribute<A_STANDOUT> StandOut;
typedef Attribute<A_BOLD> Bold;

static inline float get_decibel( const float& value ) {
	return 10 * log10( value );
}

static inline float get_frequency_value( const float& value ) {
	return value < 1000 ? value : value / 1000;
}

static inline std::string get_frequency_unit( const float& value ) {
	return value < 1000 ? "Hz" : "kHz";
}

static inline std::string to_width( const std::string& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::setw( width ) << value;
	return o.str();
}

static inline std::string to_filter_type( const filtering::FrequencyFilterType& filter_type, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << persistence::filter_type_to_xml( filter_type );
	return to_width( o.str(), width );
}

static inline std::string to_decay_type( const envelope::DecayType& decay_type, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << persistence::decay_type_to_xml( decay_type );
	return to_width( o.str(), width );
}

static inline std::string to_stretch_type( const filtering::TimeStretchType& stretch_type, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << persistence::stretch_type_to_xml( stretch_type );
	return to_width( o.str(), width );
}

static inline std::string to_percentage( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << ( value * 100 ) << " %%";
	return to_width( o.str(), width + 1 );
}

static inline std::string to_miliseconds( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << ( value * 1000 ) << " ms";
	return to_width( o.str(), width );
}

static inline std::string to_seconds( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << value << " s";
	return to_width( o.str(), width );
}

static inline std::string to_frequency( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << get_frequency_value( value ) << " " << get_frequency_unit( value );
	return to_width( o.str(), width );
}

static inline std::string to_resonance( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << ( ( 1 - value ) * 20 );
	return to_width( o.str(), width );
}

static inline std::string to_decibel( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << get_decibel( value ) << " dB";
	return to_width( o.str(), width );
}

static inline std::string to_semitones( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(0) << value << " st";
	return to_width( o.str(), width );
}

static inline std::string to_value( const util::floating_t& value, const size_t& width = SOUND_WIDTH ) {
	std::ostringstream o;
	o << std::fixed << std::setprecision(2) << value;
	return to_width( o.str(), width );
}

static inline std::string to_switch( const bool& value, const size_t& width = SOUND_WIDTH ) {
	return to_width( value ? "[o]" : "[ ]", width );
}

static inline std::string split_file_name( const std::string& file_name, const size_t& width = SOUND_WIDTH ) {
	size_t s = file_name.find_last_of('/');
	size_t e = file_name.find_last_of('.');
	size_t w = width - 1;
	if ( std::string::npos != e ) {
		w = e - s - 1;
		if ( w > ( width - 1 ) ) {
			w = width - 1;
		}
	}
	return file_name.substr( std::string::npos != s ? s + 1 : 0, w );
}

enum RowId {
	ROW_START = 0,
	ROW_SOFT,
	ROW_TRANSPOSE,
	ROW_TRANSPOSE_VELOCITY,
	ROW_TRANSPOSE_RANDOM,
	ROW_STRETCH,
	ROW_STRETCH_VELOCITY,
	ROW_STRETCH_TYPE,
	ROW_DRIVE,
	ROW_SAT,
	ROW_FREQ,
	ROW_FILTER,
	ROW_FILTER_TYPE,
	ROW_RES,
	ROW_FILTER_VELOCITY,
	ROW_FILTER_RANDOM,
	ROW_DECAY,
	ROW_TYPE,
	ROW_PAN,
	ROW_PAN_VELOCITY,
	ROW_PAN_RANDOM,
	ROW_VOLUME,
	ROW_VOLUME_VELOCITY,
	ROW_MUTED,
	ROW_SOLOED,
	ROW_LINKED
};

class SelectableRow {
	int selected_row;
public:
	SelectableRow() : selected_row( -1 ) {}
	virtual ~SelectableRow() {}
	virtual const int& get_selected_row() const {
		return selected_row;
	}
	virtual void set_selected_row( const int& selected_row ) {
		this->selected_row = selected_row;
	}
	virtual void move_down() {
		selected_row = ( selected_row + 1 ) % SOUND_HEIGHT;
	}
	virtual void move_up() {
		selected_row--;
		if ( selected_row < 0 ) {
			selected_row = SOUND_HEIGHT - 1;
		}
	}
	virtual void deselect_row() {
		selected_row = -1;
	}
};

enum EditMode {
	EDIT_UP,
	EDIT_DOWN,
	EDIT_UP_FINE,
	EDIT_DOWN_FINE
};

class Editable {
public:
	Editable() {}
	virtual ~Editable() {}
	virtual void edit_value( const EditMode& edit_mode ) {}
};

class Button : public Window {
	BlockVector blocks;
	std::string label;
	ColorType color;
protected:
	virtual void generate_blocks() {
		StringVector s;
		s.push_back( get_label() );
		blocks.clear();
		blocks.push_back( s );
	}
public:
	Button( const int& x, const int& y ) :
		Window( BUTTON_WIDTH, BUTTON_HEIGHT, x, y ), color( BUTTON_NORMAL ) {}
	Button( const int& x, const int& y, const std::string& label ) :
		Window( BUTTON_WIDTH, BUTTON_HEIGHT, x, y ), label( label ), color( BUTTON_NORMAL ) {}
	Button( const int& width, const int& height, const int& x, const int& y, const std::string& label ) :
		Window( width, height, x, y ), label( label ), color( BUTTON_NORMAL ) {}
	virtual ~Button() {}
	virtual void set_label( const std::string& label ) {
		this->label = label;
	}
	virtual std::string get_label() const {
		return label;
	}
	virtual void set_color( const ColorType& color ) {
		this->color = color;
	}
	virtual ColorType get_color() const {
		return color;
	}
	virtual void draw() {
		generate_blocks();
		Color color( get_window(), get_color() );
		mvwprintw( get_window(), 0, 0, get_label().c_str() );
		Window::draw();
	}
};

class PresetCreate : public Button {
public:
	PresetCreate( const int& x, const int& y ) :
		Button( x, y, to_width( "F1 create", BUTTON_WIDTH ) ) {}
};

class PresetName : public Button {
	repulse::Engine* engine;
public:
	PresetName( repulse::Engine* engine, const int& x, const int& y ):
		Button( PRESET_NAME_WIDTH, BUTTON_HEIGHT, x, y, "" ), engine( engine ) {
		set_color( PRESET_NAME );
	}
	virtual std::string get_label() const {
		std::ostringstream o;
		o << engine->get_selected_preset_name() << " (" << engine->get_selected_preset() << ") +-";
		return to_width( o.str(), PRESET_NAME_WIDTH );
	}
};

class PresetSave : public Button {
	repulse::Engine* context;
public:
	PresetSave( const int& x, const int& y ) :
		Button( x, y, to_width( "F2 save", BUTTON_WIDTH ) ) {}
};

class PresetRecall : public Button {
public:
	PresetRecall( const int& x, const int& y ) :
		Button( x, y, to_width( "F3 recall", BUTTON_WIDTH )  ) {}
};

class LocalKeyboard : public Button {
	repulse::Engine* engine;
public:
	LocalKeyboard( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F4 local", BUTTON_WIDTH )  ), engine( engine ) {}
	virtual ColorType get_color() const {
		return engine->is_local_keyboard() ? BUTTON_ON : BUTTON_OFF;
	}
};

class AltWheel : public Button {
	repulse::Engine* engine;
public:
	AltWheel( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F5 wheel", BUTTON_WIDTH )  ), engine( engine ) {}
	virtual ColorType get_color() const {
		return engine->is_alternate_wheel() ? BUTTON_ON : BUTTON_OFF;
	}
};

class Exit : public Button {
	repulse::Engine* engine;
public:
	Exit( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F10 exit", BUTTON_WIDTH )  ), engine( engine ) {
		set_color( BUTTON_OFF );
	}
};

class Omni : public Button {
	repulse::Engine* engine;
public:
	Omni( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F6 omni", BUTTON_WIDTH )  ), engine( engine ) {}
	virtual ColorType get_color() const {
		return engine->is_omni() ? BUTTON_ON : BUTTON_OFF;
	}
};

class Mono : public Button {
	repulse::Engine* engine;
public:
	Mono( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F7 mono", BUTTON_WIDTH )  ), engine( engine ) {}
	virtual ColorType get_color() const {
		return engine->is_mono() ? BUTTON_ON : BUTTON_OFF;
	}
};

class Channel : public Button {
	repulse::Engine* engine;
public:
	Channel( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F12 ch:10", BUTTON_WIDTH ) ), engine( engine ) {
		set_color( BUTTON_ROTATORY );
	}
	virtual std::string get_label() const {
		std::ostringstream o;
		o << "F12 ch:" << (int)( engine->get_base_channel() + 1 );
		return to_width( o.str(), BUTTON_WIDTH );;
	}
};

class Note : public Button {
	repulse::Engine* engine;
public:
	Note( repulse::Engine* engine, const int& x, const int& y ) :
		Button( x, y, to_width( "F11 nt:10", BUTTON_WIDTH ) ), engine( engine ) {
		set_color( BUTTON_ROTATORY );
	}
	virtual std::string get_label() const {
		std::ostringstream o;
		o << "F11 nt:" << (int)engine->get_base_note();
		return to_width( o.str(), BUTTON_WIDTH );;
	}
};

class Sound : public Window, public SelectableRow, public Editable {
	BlockVector blocks;
	repulse::Sound* sound;
protected:
	virtual repulse::Sound* get_sound() const {
		return sound;
	}
	virtual BlockVector& get_blocks() {
		return blocks;
	}
	virtual void generate_blocks() {
		size_t i = 0;
		blocks.clear();
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_miliseconds( sound->get_start_time() ) );
		blocks[i].push_back( to_switch( sound->is_start_soft() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_semitones( sound->get_transpose() ) );
		blocks[i].push_back( to_percentage( sound->get_transpose_velocity() ) );
		blocks[i].push_back( to_percentage( sound->get_transpose_random() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_percentage( sound->get_stretch() ) );
		blocks[i].push_back( to_percentage( sound->get_stretch_velocity() ) );
		blocks[i].push_back( to_width( to_stretch_type( sound->get_stretch_type() ) ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_value( sound->get_over_drive_drive() ) );
		blocks[i].push_back( to_switch( sound->is_over_drive_active() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_frequency( sound->get_filter_frequency() ) );
		blocks[i].push_back( to_switch( sound->is_filter_active() ) );
		blocks[i].push_back( to_filter_type( sound->get_filter_type() ) );
		blocks[i].push_back( to_resonance( sound->get_filter_resonance() ) );
		blocks[i].push_back( to_percentage( sound->get_filter_frequency_velocity() ) );
		blocks[i].push_back( to_percentage( sound->get_filter_frequency_random() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_seconds( sound->get_decay_time() ) );
		blocks[i].push_back( to_decay_type( sound->get_decay_type() ).c_str() );
		i++;
		blocks.push_back( StringVector() );
		if ( sound->get_panning() == 0 ) {
			blocks[i].push_back( to_width( "Centered" ) );
		} else {
			blocks[i].push_back( to_percentage( sound->get_panning() ) );
		}
		blocks[i].push_back( to_percentage( sound->get_panning_velocity() ) );
		blocks[i].push_back( to_percentage( sound->get_panning_random() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_decibel( sound->get_volume() ) );
		blocks[i].push_back( to_percentage( sound->get_volume_velocity() ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_switch( sound->is_muted() ) );
		blocks[i].push_back( to_switch( sound->is_soloed() ) );
		blocks[i].push_back( to_width( "" ) );
	}
public:
	Sound( repulse::Sound* sound, const int& x, const int& y ) :
		Window( SOUND_WIDTH, SOUND_HEIGHT, x, y ), SelectableRow(), sound( sound ) {}
	~Sound() {}
	void edit_value( const EditMode& edit_mode ) {
		int tmp;
		switch ( get_selected_row() ) {
		case ROW_START:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_start_time( sound->get_start_time() + 0.001 );
				break;
			case EDIT_DOWN:
				sound->set_start_time( sound->get_start_time() - 0.001 );
				break;
			case EDIT_UP_FINE:
				sound->set_start_time( sound->get_start_time() + 0.0001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_start_time( sound->get_start_time() - 0.0001 );
				break;
			}
			break;
		case ROW_SOFT:
			sound->set_start_soft( !sound->is_start_soft() );
			break;
		case ROW_TRANSPOSE:
			switch ( edit_mode ) {
			case EDIT_UP:
			case EDIT_UP_FINE:
				sound->set_transpose( sound->get_transpose() + 1 );
				break;
			case EDIT_DOWN:
			case EDIT_DOWN_FINE:
				sound->set_transpose( sound->get_transpose() - 1 );
				break;
			}
			break;
		case ROW_TRANSPOSE_VELOCITY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_transpose_velocity( sound->get_transpose_velocity() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_transpose_velocity( sound->get_transpose_velocity() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_transpose_velocity( sound->get_transpose_velocity() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_transpose_velocity( sound->get_transpose_velocity() - 0.001 );
				break;
			}
			break;
		case ROW_TRANSPOSE_RANDOM:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_transpose_random( sound->get_transpose_random() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_transpose_random( sound->get_transpose_random() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_transpose_random( sound->get_transpose_random() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_transpose_random( sound->get_transpose_random() - 0.001 );
				break;
			}
			break;
		case ROW_STRETCH:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_stretch( sound->get_stretch() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_stretch( sound->get_stretch() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_stretch( sound->get_stretch() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_stretch( sound->get_stretch() - 0.001 );
				break;
			}
			break;
		case ROW_STRETCH_VELOCITY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_stretch_velocity( sound->get_stretch_velocity() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_stretch_velocity( sound->get_stretch_velocity() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_stretch_velocity( sound->get_stretch_velocity() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_stretch_velocity( sound->get_stretch_velocity() - 0.001 );
				break;
			}
			break;
		case ROW_STRETCH_TYPE:
			switch ( edit_mode ) {
			case EDIT_UP:
			case EDIT_UP_FINE:
				tmp = ( sound->get_stretch_type() + 1 ) % ( filtering::TIME_STRETCH_LAST_TYPE + 1 );
				break;
			case EDIT_DOWN:
			case EDIT_DOWN_FINE:
				tmp = sound->get_stretch_type() - 1;
				if ( tmp < 0 ) {
					tmp = filtering::TIME_STRETCH_LAST_TYPE;
				}
				break;
			}
			sound->set_stretch_type( (filtering::TimeStretchType)tmp );
			break;
		case ROW_DRIVE:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_over_drive_drive( sound->get_over_drive_drive() + 1 );
				break;
			case EDIT_DOWN:
				sound->set_over_drive_drive( sound->get_over_drive_drive() - 1 );
				break;
			case EDIT_UP_FINE:
				sound->set_over_drive_drive( sound->get_over_drive_drive() + 0.1 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_over_drive_drive( sound->get_over_drive_drive() - 0.1 );
				break;
			}
			break;
		case ROW_SAT:
			sound->set_over_drive_active( !sound->is_over_drive_active() );
			break;
		case ROW_FREQ:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_filter_frequency( sound->get_filter_frequency() + 100 );
				break;
			case EDIT_DOWN:
				sound->set_filter_frequency( sound->get_filter_frequency() - 100 );
				break;
			case EDIT_UP_FINE:
				sound->set_filter_frequency( sound->get_filter_frequency() + 10 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_filter_frequency( sound->get_filter_frequency() - 10 );
				break;
			}
			break;
		case ROW_FILTER:
			sound->set_filter_active( !sound->is_filter_active() );
			break;
		case ROW_FILTER_TYPE:
			switch ( edit_mode ) {
			case EDIT_UP:
			case EDIT_UP_FINE:
				tmp = ( sound->get_filter_type() + 1 ) % ( filtering::FREQUENCY_FILTER_LAST_TYPE + 1 );
				break;
			case EDIT_DOWN:
			case EDIT_DOWN_FINE:
				tmp = sound->get_filter_type() - 1;
				if ( tmp < 0 ) {
					tmp = filtering::FREQUENCY_FILTER_LAST_TYPE;
				}
				break;
			}
			sound->set_filter_type( (filtering::FrequencyFilterType)tmp );
			break;
		case ROW_RES:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_filter_resonance( sound->get_filter_resonance() - 0.05 );
				break;
			case EDIT_DOWN:
				sound->set_filter_resonance( sound->get_filter_resonance() + 0.05 );
				break;
			case EDIT_UP_FINE:
				sound->set_filter_resonance( sound->get_filter_resonance() - 0.005 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_filter_resonance( sound->get_filter_resonance() + 0.005 );
				break;
			}
			break;
		case ROW_FILTER_VELOCITY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_filter_frequency_velocity( sound->get_filter_frequency_velocity() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_filter_frequency_velocity( sound->get_filter_frequency_velocity() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_filter_frequency_velocity( sound->get_filter_frequency_velocity() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_filter_frequency_velocity( sound->get_filter_frequency_velocity() - 0.001 );
				break;
			}
			break;
		case ROW_FILTER_RANDOM:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_filter_frequency_random( sound->get_filter_frequency_random() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_filter_frequency_random( sound->get_filter_frequency_random() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_filter_frequency_random( sound->get_filter_frequency_random() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_filter_frequency_random( sound->get_filter_frequency_random() - 0.001 );
				break;
			}
			break;
		case ROW_DECAY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_decay_time( sound->get_decay_time() + 0.1 );
				break;
			case EDIT_DOWN:
				sound->set_decay_time( sound->get_decay_time() - 0.1 );
				break;
			case EDIT_UP_FINE:
				sound->set_decay_time( sound->get_decay_time() + 0.01 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_decay_time( sound->get_decay_time() - 0.01 );
				break;
			}
			break;
		case ROW_TYPE:
			switch ( edit_mode ) {
			case EDIT_UP:
			case EDIT_UP_FINE:
				tmp = ( sound->get_decay_type() + 1 ) % ( envelope::DECAY_LAST_TYPE + 1 );
				break;
			case EDIT_DOWN:
			case EDIT_DOWN_FINE:
				tmp = sound->get_decay_type() - 1;
				if ( tmp < 0 ) {
					tmp = envelope::DECAY_LAST_TYPE;
				}
				break;
			}
			sound->set_decay_type( (envelope::DecayType)tmp );
			break;
		case ROW_PAN:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_panning( sound->get_panning() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_panning( sound->get_panning() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_panning( sound->get_panning() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_panning( sound->get_panning() - 0.001 );
				break;
			}
			break;
		case ROW_PAN_VELOCITY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_panning_velocity( sound->get_panning_velocity() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_panning_velocity( sound->get_panning_velocity() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_panning_velocity( sound->get_panning_velocity() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_panning_velocity( sound->get_panning_velocity() - 0.001 );
				break;
			}
			break;
		case ROW_PAN_RANDOM:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_panning_random( sound->get_panning_random() + 0.1 );
				break;
			case EDIT_DOWN:
				sound->set_panning_random( sound->get_panning_random() - 0.1 );
				break;
			case EDIT_UP_FINE:
				sound->set_panning_random( sound->get_panning_random() + 0.01 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_panning_random( sound->get_panning_random() - 0.01 );
				break;
			}
			break;
		case ROW_VOLUME:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_volume( sound->get_volume() + 0.1 );
				break;
			case EDIT_DOWN:
				sound->set_volume( sound->get_volume() - 0.1 );
				break;
			case EDIT_UP_FINE:
				sound->set_volume( sound->get_volume() + 0.01 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_volume( sound->get_volume() - 0.01 );
				break;
			}
			break;
		case ROW_VOLUME_VELOCITY:
			switch ( edit_mode ) {
			case EDIT_UP:
				sound->set_volume_velocity( sound->get_volume_velocity() + 0.01 );
				break;
			case EDIT_DOWN:
				sound->set_volume_velocity( sound->get_volume_velocity() - 0.01 );
				break;
			case EDIT_UP_FINE:
				sound->set_volume_velocity( sound->get_volume_velocity() + 0.001 );
				break;
			case EDIT_DOWN_FINE:
				sound->set_volume_velocity( sound->get_volume_velocity() - 0.001 );
				break;
			}
			break;
		case ROW_MUTED:
			sound->set_muted( !sound->is_muted() );
			break;
		case ROW_SOLOED:
			sound->get_engine()->solo( sound->get_id(), !sound->is_soloed() );
			break;
		}
	}
	void draw() {
		size_t i;
		int j;
		generate_blocks();
		BlockVector::const_iterator it1;
		StringVector::const_iterator it2;
		for ( it1 = blocks.begin(), i = 0, j = 0; it1 != blocks.end(); ++it1, ++i ) {
			Color color( get_window(), i % 2 == 0 ? ROW_A_VALUE : ROW_B_VALUE );
			for ( it2 = it1->begin(); it2 != it1->end(); ++it2, j++ ) {
				if ( get_selected_row() == j ) {
					StandOut stand_out( get_window() );
					Reverse reverse( get_window() );
					mvwprintw( get_window(), j, 0, it2->c_str() );
				} else {
					mvwprintw( get_window(), j, 0, it2->c_str() );
				}
			}
		}
		Window::draw();
	}
};

class SoundJoin : public Sound {
public:
	SoundJoin( repulse::Sound* sound, const int& x, const int& y ) :
		Sound( sound, x, y ) {}
	~SoundJoin() {}
	void edit_value( const EditMode& edit_mode ) {
		if ( get_selected_row() == ROW_LINKED ) {
			get_sound()->get_engine()->set_linked( !get_sound()->get_engine()->is_linked() );
		} else {
			Sound::edit_value( edit_mode );
		}
	}
	void generate_blocks() {
		Sound::generate_blocks();
		get_blocks().back().back() = to_switch( get_sound()->get_engine()->is_linked() );
	}
};

class Engine : public Window, public SelectableRow, public Editable {
	BlockVector blocks;
	repulse::Engine* engine;
protected:
	virtual void generate_blocks() {
		size_t i = 0;
		blocks.clear();
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_semitones( engine->get_transpose() ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_percentage( engine->get_stretch() ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_decibel( engine->get_volume() ) );
		blocks[i].push_back( to_width( "" ) );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_width( "" ) );
		blocks[i].push_back( to_switch( engine->is_linked() ) );
	}
	virtual void generate_blocks_ex() {
		blocks[1][0] = to_semitones( engine->get_transpose() );
		blocks[2][0] = to_percentage( engine->get_stretch() );
		blocks[7][0] = to_decibel( engine->get_volume() );
		blocks[8][2] = to_switch( engine->is_linked() );
	}
public:
	Engine( repulse::Engine* engine, const int& x, const int& y ) :
		Window( SOUND_WIDTH, SOUND_HEIGHT, x, y ), SelectableRow(), Editable(), engine( engine ) {
		generate_blocks();
	}
	~Engine() {}
	void edit_value( const EditMode& edit_mode ) {
		switch ( get_selected_row() ) {
		case ROW_STRETCH:
			switch ( edit_mode ) {
			case EDIT_UP:
				engine->set_stretch_offset( engine->get_stretch_offset() + 0.1 );
				break;
			case EDIT_DOWN:
				engine->set_stretch_offset( engine->get_stretch_offset() - 0.1 );
				break;
			case EDIT_UP_FINE:
				engine->set_stretch_offset( engine->get_stretch_offset() + 0.01 );
				break;
			case EDIT_DOWN_FINE:
				engine->set_stretch_offset( engine->get_stretch_offset() - 0.01 );
				break;
			}
			break;
		case ROW_TRANSPOSE:
			switch ( edit_mode ) {
			case EDIT_UP:
			case EDIT_UP_FINE:
				engine->set_transpose_offset( engine->get_transpose_offset() + 1 );
				break;
			case EDIT_DOWN:
			case EDIT_DOWN_FINE:
				engine->set_transpose_offset( engine->get_transpose_offset() - 1 );
				break;
			}
			break;
		case ROW_VOLUME:
			switch ( edit_mode ) {
			case EDIT_UP:
				engine->set_volume( engine->get_volume() + 0.1 );
				break;
			case EDIT_DOWN:
				engine->set_volume( engine->get_volume() - 0.1 );
				break;
			case EDIT_UP_FINE:
				engine->set_volume( engine->get_volume() + 0.01 );
				break;
			case EDIT_DOWN_FINE:
				engine->set_volume( engine->get_volume() - 0.01 );
				break;
			}
			break;
		case ROW_LINKED:
			engine->set_linked( !engine->is_linked() );
			break;
		}
	}
	void draw() {
		size_t i;
		int j;
		generate_blocks_ex();
		BlockVector::const_iterator it1;
		StringVector::const_iterator it2;
		for ( it1 = blocks.begin(), i = 0, j = 0; it1 != blocks.end(); ++it1, ++i ) {
			Color color( get_window(), i % 2 == 0 ? ROW_A_VALUE : ROW_B_VALUE );
			for ( it2 = it1->begin(); it2 != it1->end(); ++it2, j++ ) {
				if ( get_selected_row() == j ) {
					StandOut stand_out( get_window() );
					Reverse reverse( get_window() );
					mvwprintw( get_window(), j, 0, it2->c_str() );
				} else {
					mvwprintw( get_window(), j, 0, it2->c_str() );
				}
			}
		}
		Window::draw();
	}
};

class Header : public Window {
	BlockVector blocks;
	repulse::Engine* engine;
protected:
	void generate_blocks() {
		size_t i;
		for ( i = 0; i < util::MAX_SOUNDS; ++i ) {
			blocks.push_back( StringVector() );
			std::ostringstream o;
			o << "Sound0" << ( i + 1 );
			blocks[i].push_back( to_width( o.str() ) );
			blocks[i].push_back( to_width(
					split_file_name( engine->get_sounds()[ i ]->get_file_name() ) ) );
			o.str("");
		}
		blocks.push_back( StringVector() );
		blocks[i].push_back( to_width( "Engine" ) );
		blocks[i].push_back( to_width( "" ) );
	}
public:
	Header( repulse::Engine* engine, const int& x, const int& y ) :
		Window( SOUND_WIDTH * 10, HEADER_HEIGHT, x, y ), engine( engine ) {
		generate_blocks();
	}
	~Header() {}
	void draw() {
		{
			Bold bold( get_window() );
			size_t i,j;
			BlockVector::const_iterator it;
			for ( it = blocks.begin(), i = SOUND_WIDTH, j = 0; it != blocks.end(); ++it, i += SOUND_WIDTH, ++j ) {
				{
					Color color( get_window(), HEADER_SOUND );
					mvwprintw( get_window(), 0, i, (*it)[0].c_str() );
				}
				if ( j < util::MAX_SOUNDS ) {
					if ( engine->get_sounds()[j]->is_playing() ) {
						Color color( get_window(), HEADER_PLAYING );
						mvwprintw( get_window(), 0, i+2, ">" );
					} else {
						Color color( get_window(), HEADER_SOUND );
						mvwprintw( get_window(), 0, i+2, " " );
					}
				}
				{
					Color color( get_window(), HEADER_FILE );
					mvwprintw( get_window(), 1, i, (*it)[1].c_str() );
				}
			}
			{
				Color color( get_window(), HEADER_TITLE );
				mvwprintw( get_window(), 0, 1, engine->get_name().c_str() );
			}
		}
		{
			Color color( get_window(), HEADER_VERSION );
			std::ostringstream o;
			o << "v" << util::VERSION_;
			mvwprintw( get_window(), 1, 1, o.str().c_str() );
		}
		Window::draw();
	}
};

class Helper : public Window {
	BlockVector blocks;
protected:
	virtual void generate_blocks() {
		size_t i = 0;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Start    " );
		blocks[i].push_back( " Soft     " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Transpose" );
		blocks[i].push_back( " Velocity " );
		blocks[i].push_back( " Random   " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Stretch  " );
		blocks[i].push_back( " Velocity " );
		blocks[i].push_back( " Type     " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Drive    " );
		blocks[i].push_back( " Sat      " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Freq     " );
		blocks[i].push_back( " Filter   " );
		blocks[i].push_back( " Type     " );
		blocks[i].push_back( " Res      " );
		blocks[i].push_back( " Velocity " );
		blocks[i].push_back( " Random   " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Decay    " );
		blocks[i].push_back( " Type     " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Pan      " );
		blocks[i].push_back( " Velocity " );
		blocks[i].push_back( " Random   " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Volume   " );
		blocks[i].push_back( " Velocity " );
		i++;
		blocks.push_back( StringVector() );
		blocks[i].push_back( " Muted    " );
		blocks[i].push_back( " Soloed   " );
		blocks[i].push_back( " Linked   " );
	}
public:
	Helper( const int& x, const int& y ) :
		Window( SOUND_WIDTH, SOUND_HEIGHT, x, y ) {
		generate_blocks();
	}
	~Helper() {}
	void draw() {
		Bold bold( get_window() );
		size_t i, j;
		BlockVector::const_iterator it1;
		StringVector::const_iterator it2;
		for ( it1 = blocks.begin(), i = 0, j = 0; it1 != blocks.end(); ++it1, ++i ) {
			Color color( get_window(), i % 2 == 0 ? ROW_A_NAME : ROW_B_VALUE );
			for ( it2 = it1->begin(); it2 != it1->end(); ++it2, j++ ) {
				mvwprintw( get_window(), j, 0, it2->c_str() );
			}
		}
		Window::draw();
	}
};

enum MultipleKeys {
	CONTROL_LEFT = 68,
	CONTROL_RIGHT = 67,
	CONTROL_UP = 65,
	CONTROL_DOWN = 66
};

static const int KEY_F1 = KEY_F(1);
static const int KEY_F2 = KEY_F(2);
static const int KEY_F3 = KEY_F(3);
static const int KEY_F4 = KEY_F(4);
static const int KEY_F5 = KEY_F(5);
static const int KEY_F6 = KEY_F(6);
static const int KEY_F7 = KEY_F(7);
static const int KEY_F8 = KEY_F(8);
static const int KEY_F9 = KEY_F(9);
static const int KEY_F10 = KEY_F(10);
static const int KEY_F11 = KEY_F(11);
static const int KEY_F12 = KEY_F(12);

class UI {
	PresetName* preset_name;
	WindowVector windows;
	repulse::Engine* engine;
	bool leave;
	int selected_column;
protected:
	void initialize_curses() {
		initscr();
		clear();
		noecho();
		keypad( stdscr, TRUE );
		halfdelay( 10 );
		curs_set( 0 );
	}
	void initialize_colors() {
		assert( has_colors() == TRUE );
		start_color();
		init_pair( ROW_A_NAME     , COLOR_WHITE , COLOR_CYAN    );
		init_pair( ROW_A_VALUE    , COLOR_BLACK , COLOR_CYAN    );
		init_pair( ROW_B_NAME     , COLOR_WHITE , COLOR_BLUE    );
		init_pair( ROW_B_VALUE    , COLOR_WHITE , COLOR_BLUE    );
		init_pair( HEADER_SOUND   , COLOR_WHITE , COLOR_MAGENTA );
		init_pair( HEADER_FILE    , COLOR_YELLOW, COLOR_MAGENTA );
		init_pair( HEADER_TITLE   , COLOR_GREEN , COLOR_BLACK   );
		init_pair( HEADER_VERSION , COLOR_WHITE , COLOR_BLACK   );
		init_pair( HEADER_PLAYING , COLOR_GREEN , COLOR_MAGENTA );
		init_pair( CELL_EDITING   , COLOR_YELLOW, COLOR_BLACK   );
		init_pair( BUTTON_NORMAL  , COLOR_WHITE , COLOR_RED     );
		init_pair( PRESET_NAME    , COLOR_BLUE  , COLOR_WHITE   );
		init_pair( BUTTON_ON      , COLOR_BLACK , COLOR_WHITE   );
		init_pair( BUTTON_OFF     , COLOR_WHITE , COLOR_BLACK   );
		init_pair( BUTTON_ROTATORY, COLOR_YELLOW, COLOR_BLACK   );
	}
	void initialize_screens() {
		int x = HELPER_WIDTH;
		for ( size_t i = 0; i < util::MAX_SOUNDS; ++i, x += SOUND_WIDTH ) {
			if ( i < util::MAX_SOUNDS - 2 ) {
				windows.push_back( new Sound( engine->get_sounds()[i], x, HEADER_HEIGHT ) );
			} else {
				windows.push_back( new SoundJoin( engine->get_sounds()[i], x, HEADER_HEIGHT ) );
			}
		}
		dynamic_cast<SelectableRow*>( windows[0] )->set_selected_row( 0 );
		windows.push_back( new Engine        ( engine, x, HEADER_HEIGHT ) );
		windows.push_back( new PresetCreate  ( BUTTON_WIDTH, PRESET_ROW ) );
		windows.push_back( new PresetName    ( engine, BUTTON_WIDTH * 2, PRESET_ROW ) );
		windows.push_back( new PresetRecall  ( ( BUTTON_WIDTH * 3 ) + PRESET_NAME_WIDTH, PRESET_ROW ) );
		windows.push_back( new PresetSave    ( ( BUTTON_WIDTH * 2 ) + PRESET_NAME_WIDTH, PRESET_ROW ) );
		windows.push_back( new Helper        ( 0, HEADER_HEIGHT ) );
		windows.push_back( new Header        ( engine, 0, 0 ) );
		windows.push_back( new Helper        ( 0, HEADER_HEIGHT ) );
		windows.push_back( new Header        ( engine, 0, 0 ) );
		windows.push_back( new LocalKeyboard ( engine, BUTTON_WIDTH * 1, PRESET_ROW + 1 ) );
		windows.push_back( new AltWheel      ( engine, BUTTON_WIDTH * 2, PRESET_ROW + 1 ) );
		windows.push_back( new Omni          ( engine, BUTTON_WIDTH * 3, PRESET_ROW + 1 ) );
		windows.push_back( new Mono          ( engine, BUTTON_WIDTH * 4, PRESET_ROW + 1 ) );
		windows.push_back( new Exit          ( engine, BUTTON_WIDTH * 7, PRESET_ROW + 1 ) );
		windows.push_back( new Note          ( engine, BUTTON_WIDTH * 8, PRESET_ROW + 1 ) );
		windows.push_back( new Channel       ( engine, BUTTON_WIDTH * 9, PRESET_ROW + 1 ) );
	}
	void terminate_screens() {
		WindowVector::const_iterator it;
		for ( it = windows.begin(); it != windows.end(); ++it ) {
			delete *it;
		}
	}
	void terminate_curses() {
		endwin();
	}
public:
	UI( repulse::Engine* engine ) :
		engine( engine ), leave( false ), selected_column( 0 ) {
		initialize_curses();
		initialize_colors();
		initialize_screens();
	}
	~UI() {
		terminate_screens();
		terminate_curses();
	}
	bool is_leave() const { return leave; }
	void draw() {
		SelectableRow* column;
		int ch;
		WindowVector::const_iterator it;
		for ( it = windows.begin(); it != windows.end(); ++it ) {
			(*it)->draw();
		}
		doupdate();
		if ( ( ch = getch() ) != ERR ) {
			switch( ch ) {
			case CONTROL_LEFT:
				dynamic_cast<Editable*>( windows[ selected_column ] )->edit_value( EDIT_DOWN_FINE );
				break;
			case CONTROL_RIGHT:
				dynamic_cast<Editable*>( windows[ selected_column ] )->edit_value( EDIT_UP_FINE );
				break;
			case CONTROL_UP:
				dynamic_cast<Editable*>( windows[ selected_column ] )->edit_value( EDIT_UP );
				break;
			case CONTROL_DOWN:
				dynamic_cast<Editable*>( windows[ selected_column ] )->edit_value( EDIT_DOWN );
				break;
			case KEY_LEFT:
				column = dynamic_cast<SelectableRow*>( windows[ selected_column ] );
				selected_column = selected_column - 1;
				if ( selected_column < 0 ) {
					selected_column = util::MAX_SOUNDS;
				}
				dynamic_cast<SelectableRow*>( windows[ selected_column ] )->set_selected_row( column->get_selected_row() );
				column->deselect_row();
				break;
			case KEY_RIGHT:
				column = dynamic_cast<SelectableRow*>( windows[ selected_column ] );
				selected_column = ( selected_column + 1 ) % ( util::MAX_SOUNDS + 1 );
				dynamic_cast<SelectableRow*>( windows[ selected_column ] )->set_selected_row( column->get_selected_row() );
				column->deselect_row();
				break;
			case KEY_DOWN:
				column = dynamic_cast<SelectableRow*>( windows[ selected_column ] );
				column->set_selected_row( column->get_selected_row() );
				column->move_down();
				break;
			case KEY_UP:
				column = dynamic_cast<SelectableRow*>( windows[ selected_column ] );
				column->set_selected_row( column->get_selected_row() );
				column->move_up();
				break;
			case 'q':
			case 'Q':
				engine->get_sounds()[ util::SOUND_01 ]->note_on( 120 );
				break;
			case 'w':
			case 'W':
				engine->get_sounds()[ util::SOUND_02 ]->note_on( 120 );
				break;
			case 'e':
			case 'E':
				engine->get_sounds()[ util::SOUND_03 ]->note_on( 120 );
				break;
			case 'r':
			case 'R':
				engine->get_sounds()[ util::SOUND_04 ]->note_on( 120 );
				break;
			case 't':
			case 'T':
				engine->get_sounds()[ util::SOUND_05 ]->note_on( 120 );
				break;
			case 'y':
			case 'Y':
				engine->get_sounds()[ util::SOUND_06 ]->note_on( 120 );
				break;
			case 'u':
			case 'U':
				engine->get_sounds()[ util::SOUND_07 ]->note_on( 120 );
				break;
			case 'i':
			case 'I':
				engine->get_sounds()[ util::SOUND_08 ]->note_on( 120 );
				break;
			case '+':
				engine->next_preset();
				break;
			case '-':
				engine->previous_preset();
				break;
			case KEY_F1:
				engine->set_selected_preset( engine->create_preset() );
				break;
			case KEY_F2:
				engine->save_preset( engine->get_selected_preset() );
				break;
			case KEY_F3:
				engine->recall_preset( engine->get_selected_preset() );
				break;
			case KEY_F4:
				engine->set_local_keyboard( !engine->is_local_keyboard() );
				break;
			case KEY_F5:
				engine->set_alternate_wheel( !engine->is_alternate_wheel() );
				break;
			case KEY_F6:
				engine->set_omni( !engine->is_omni() );
				break;
			case KEY_F7:
				engine->set_mono( !engine->is_mono() );
				break;
			case KEY_F10:
				leave = true;
				break;
			case KEY_F11:
				engine->set_base_note( ( engine->get_base_note() + 1 ) % 128 );
				break;
			case KEY_F12:
				engine->set_base_channel( ( engine->get_base_channel() + 1 ) % 15 );
				break;
			}
		}
	}
};

}

#endif /* UI_H_ */
