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

#include <ios>
#include <iostream>
#include <fstream>
#include "ui.h"

int main( int argc, char* argv[] ) {
    if ( argc >= 2 ) {
    	repulse::Engine* engine = new repulse::Engine( argc >= 3 ? argv[2] : "repulse" );
    	engine->set_document_file( argv[1] );
    	engine->load_document();
    	engine->load_waves();
    	engine->load_repulse();
		ui::UI ui( engine );
		while ( !ui.is_leave() ) {
			ui.draw();
		}
		engine->save_repulse();
		delete engine;
    } else {
        std::cout << "repulse <patch_file> [jack_client_name]" << std::endl;
    }
    return 0;
}
