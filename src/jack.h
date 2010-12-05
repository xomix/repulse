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

#ifndef JACK_H_
#define JACK_H_

#include <set>
#include <cassert>
#include <jack/jack.h>
#include <jack/midiport.h>
#include "midi.h"

namespace jack {

typedef jack_default_audio_sample_t sample_t;

class Client;

class Listener {
public:
    Listener() {}
    virtual ~Listener() {}
    virtual void on_process( Client* client ) {}
    virtual void on_sample_rate( Client* client ) {}
    virtual void on_buffer_size( Client* client ) {}
    virtual void on_shutdown( Client* client ) {}
};

typedef std::set<Listener*> ListenerSet;

class Client {
    jack_client_t* jack_client;
    jack_nframes_t sample_rate;
    jack_nframes_t buffer_size;
    size_t data_size;
    ListenerSet jack_listeners;
    std::string name;
private:
    static int callback_process( jack_nframes_t frames, void *arg ) {
        ((Client*)arg)->on_process();
        return 0;
    }
    static int callback_sample_rate( jack_nframes_t sample_rate, void *arg ) {
        ((Client*)arg)->on_sample_rate( sample_rate );
        return 0;
    }
    static int callback_buffer_size( jack_nframes_t buffer_size, void *arg ) {
        ((Client*)arg)->on_buffer_size( buffer_size );
        return 0;
    }
    static void callback_shutdown( void *arg ) {
        ((Client*)arg)->on_shutdown();
    }
protected:
    void on_process() {
        fire_process();
    }
    void on_sample_rate( const jack_nframes_t& sample_rate ) {
        set_sample_rate( sample_rate );
        fire_sample_rate();
    }
    void on_buffer_size( const jack_nframes_t& buffer_size ) {
        set_buffer_size( buffer_size );
        fire_buffer_size();
    }
    void on_shutdown() {
        fire_shutdown();
    }
    void fire_process() {
    	ListenerSet::const_iterator it;
        for ( it = jack_listeners.begin(); it != jack_listeners.end(); ++it ) {
            (*it)->on_process( this );
        }
    }
    void fire_sample_rate() {
    	ListenerSet::const_iterator it;
        for ( it = jack_listeners.begin(); it != jack_listeners.end(); ++it ) {
            (*it)->on_sample_rate( this );
        }
    }
    void fire_buffer_size() {
    	ListenerSet::const_iterator it;
        for ( it = jack_listeners.begin(); it != jack_listeners.end(); ++it ) {
            (*it)->on_buffer_size( this );
        }
    }
    void fire_shutdown() {
    	ListenerSet::const_iterator it;
        for ( it = jack_listeners.begin(); it != jack_listeners.end(); ++it ) {
            (*it)->on_shutdown( this );
        }
    }
public:
    Client( const std::string& name ) :
    	jack_client( jack_client_open( name.c_str(), JackNoStartServer, 0, 0 ) ), name( name ) {
        assert( jack_client != 0 );
        jack_set_process_callback( get_jack_client(), callback_process, this );
        jack_set_sample_rate_callback( get_jack_client(), callback_sample_rate, this );
        jack_on_shutdown( get_jack_client(), callback_shutdown, this );
        set_sample_rate( jack_get_sample_rate( get_jack_client() ) );
        set_buffer_size( jack_get_buffer_size( get_jack_client() ) );
    }
    virtual ~Client() {
        jack_client_close( jack_client );
    }
    void activate() {
    	jack_activate( jack_client );
    }
    void deactivate() {
        jack_deactivate( jack_client );
    }
    const std::string& get_name() const {
    	return name;
    }
    jack_nframes_t time_to_frames( const util::floating_t& seconds ) {
    	return round( get_sample_rate() * seconds );
    }
    util::floating_t frames_to_time( const jack_nframes_t& frames ) {
    	return frames / (util::floating_t)get_sample_rate();
    }
    jack_client_t* get_jack_client() const { return jack_client; }
    void add_jack_listener( Listener* jack_listener ) { jack_listeners.insert( jack_listener ); }
    void remove_jack_listener( Listener* jack_listener ) { jack_listeners.erase( jack_listener ); }
    void set_sample_rate( const jack_nframes_t& sample_rate ) { this->sample_rate = sample_rate; }
    const jack_nframes_t& get_sample_rate() const { return sample_rate; }
    void set_buffer_size( const jack_nframes_t& buffer_size ) {
    	this->buffer_size = buffer_size;
    	data_size = buffer_size * sizeof( sample_t );
    }
    const jack_nframes_t& get_buffer_size() const { return buffer_size; }
    const size_t& get_data_size() const { return data_size; }
};

class Port {
    Client* client;
    jack_port_t* jack_port;
protected:
    Client* get_client() const { return client; }
    void set_jack_port( jack_port_t* jack_port ) { this->jack_port = jack_port; }
public:
	Port( Client* client ) : client( client ), jack_port( 0 ) {}
	virtual ~Port() { jack_port_unregister( client->get_jack_client(), jack_port ); }
    jack_port_t* get_jack_port() const { return jack_port; }
};

class AudioPort : public Port {
public:
	AudioPort( Client* client ) : Port( client ) {}
	~AudioPort() {}
    sample_t* get_buffer() const {
    	return (sample_t*)jack_port_get_buffer( get_jack_port(), get_client()->get_buffer_size() );
    }
};

class AudioOutput : public AudioPort {
public:
    AudioOutput( Client* client, const std::string& name ) : AudioPort( client ) {
        set_jack_port( jack_port_register(
        		get_client()->get_jack_client(),
        		name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput | JackPortIsTerminal, 0 ) );
        assert( get_jack_port() != 0 );
    }
    ~AudioOutput() {}
};

class AudioInput : public AudioPort {
public:
	AudioInput( Client* client, const std::string& name ) : AudioPort( client ) {
        set_jack_port( jack_port_register(
        		get_client()->get_jack_client(),
        		name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 ) );
        assert( get_jack_port() != 0 );
    }
	~AudioInput() {}
};

class BufferedSource {
public:
	BufferedSource() {}
	virtual ~BufferedSource() {}
	virtual bool is_finished() = 0;
	virtual jack_nframes_t get_sample_rate() = 0;
	virtual void reset() = 0;
	virtual jack_nframes_t receive( jack::sample_t** samples ) = 0;
};

} // namespace jack

#endif /* JACK_H_ */
