/*
 *  messengerController.cpp
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

#include <grids/messageController.h>
#include <grids/message.h>

namespace Grids
{

	MessageController::MessageController( )
	{

	}

	void MessageController::setInterface( Interface * intr )
	{
		controller_interface = intr;
	}


	void MessageController::sendEvent( std::string in_type, Value args)

	{
		//controller_interface->sendEvent( in_type, args );
	}

	void MessageController::giveEvent( Event * evt )
	{
		for( std::vector< Message * >::size_type i = 0; i < messages.size(); i++ )
		{
			messages[i]->giveEvent( evt );
		}

	}

	void MessageController::handleOtrMessage( Kaleidoscope::Device * d, std::string sender, std::string target, std::string message )
	{

	}

	Value MessageController::convertStringToValue( std::string in_string )
	{


	}

	void MessageController::handleOtrNotify( Kaleidoscope::Device * d, std::string sender, std::string notify_type, std::string primary_notify, std::string secondary_notify )
	{


	}

	void MessageController::handleOtrNewFingerprint( Kaleidoscope::Device * d, std::string sender, std::string fingerprint )
	{

	}

	void MessageController::handleOtrLog( Kaleidoscope::Device * d, std::string log )
	{

	}

} // end namespace Grids
