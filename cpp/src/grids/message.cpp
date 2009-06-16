/*
 *  messenger.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <grids/message.h>


namespace Grids
{

	Message::Message( )
	{

	}

	void Message::giveEvent( Event * evt )
	// Called when an event is received
	{

	}

	void Message::sendEvent( std::string in_type, Value args )
	{
		controller->sendEvent( in_type, args );
	}

	void Message::setController( MessageController *cont )
	{
		controller = cont;
	}

} // end namespace Grids

