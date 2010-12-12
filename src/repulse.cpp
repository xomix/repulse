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

#include <ios>
#include <iostream>
#include <fstream>
#include "ui.h"

int main( int argc, char* argv[] ) {
	int c;
	std::string client_name = "repulse";
	bool auto_connect = false;
    while ( ( c = getopt( argc, argv, "cn:" ) ) != -1 ) {
    	switch ( c ) {
    	case 'c':
    		auto_connect = true;
    		break;
    	case 'n':
    		client_name = optarg;
    		break;
    	}
    }
    if ( optind < argc ) {
    	repulse::Engine* engine = new repulse::Engine( client_name );
    	engine->set_document_file( argv[ optind ] );
    	engine->load();
    	if ( auto_connect ) {
    		engine->auto_connect();
    	}
		ui::UI ui( engine );
		while ( !ui.is_leave() ) {
			ui.update();
		}
		engine->save_repulse();
		delete engine;

    } else {
        std::cout << "repulse [-c] [-n jack_client_name] <patch_file>" << std::endl;
    }
    return 0;
}
