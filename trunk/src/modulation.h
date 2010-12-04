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

#ifndef MODULATION_H_
#define MODULATION_H_

#include "../sfmt/sfmt.h"
#include "util.h"

namespace modulation {

static CRandomSFMT RANDOMIZE( time( 0 ) );

static const util::floating_t MIN_VELOCITY = 0;
static const util::floating_t MAX_VELOCITY = 1;
static const util::floating_t DEF_VELOCITY = MIN_VELOCITY;
static const util::floating_t MIN_RANDOM = 0;
static const util::floating_t MAX_RANDOM = 1;
static const util::floating_t DEF_RANDOM = MIN_RANDOM;

class Modulation {
public:
	virtual ~Modulation() {}
	virtual util::floating_t modulate( const util::floating_t& value ) = 0;
};

class Velocity : public Modulation {
	util::floating_t velocity;
	util::floating_t modulation;
public:
	Velocity() : Modulation(), velocity( DEF_VELOCITY ), modulation( 0 ) {}
	void set_velocity( const util::floating_t& velocity ) {
		this->velocity = util::adjust_value( velocity, MIN_VELOCITY, MAX_VELOCITY );
	}
	const util::floating_t& get_velocity() const {
		return velocity;
	}
	virtual void note_on( unsigned char velocity ) {
		modulation = ( 1. - ( velocity / 127. ) ) * this->velocity;
	}
	util::floating_t modulate( const util::floating_t& value ) {
		return value - ( value * modulation );
	}
};

class VelocityRandom : public Velocity {
	util::floating_t random;
	util::floating_t modulation;
	util::floating_t range;
public:
	VelocityRandom() :
		Velocity(), random( DEF_RANDOM ), modulation( 0 ), range( 0 ) {}
	void set_random( const util::floating_t& random ) {
		this->random = util::adjust_value( random, MIN_RANDOM, MAX_RANDOM );
	}
	const util::floating_t& get_random() const {
		return random;
	}
	void set_range( const util::floating_t& range ) {
		this->range = range;
	}
	void set_range( const util::floating_t& a, const util::floating_t& b ) {
		range = ( b - a ) / 2.;
	}
	const util::floating_t get_range() const {
		return range;
	}
	void note_on( unsigned char velocity ) {
		Velocity::note_on( velocity );
		modulation = ( ( RANDOMIZE.Random() * 2. ) - 1. ) * random;
	}
	util::floating_t modulate( const util::floating_t& value ) {
		util::floating_t ret = Velocity::modulate( value );
		return ret + ( range * modulation );
	}
};

} // namespace modulation

#endif /* MODULATION_H_ */
