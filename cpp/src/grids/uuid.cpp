



#include <grids/uuid.h>

namespace Grids
{
	UUID::UUID(){
		
	}
	
	UUID::~UUID(){

	}
	
	std::string UUID::getNewUUID(){
		uuid_obj.make( UUID_MAKE_V4 );
	
		return uuid_obj.string();
	}

} // end namespace Grids
