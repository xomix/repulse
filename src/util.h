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

const static std::string VERSION_ = "0.0.1";

typedef float floating_t;

static const floating_t PI = acos(-1);
static const floating_t PI_HALF = PI / 2;
static const floating_t PI_DOUBLE = PI * 2;

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

static const size_t MAX_SOUNDS = 8;

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

} // namespace util

#endif /* UTIL_H_ */
