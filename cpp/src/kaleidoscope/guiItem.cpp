

#include <kaleidoscope/guiItem.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	GuiItem::GuiItem( Device* d, Grids::Value* in_val ) : Object( d, in_val ) {				
		Utility::puts( "new GuiItem" );
	}
	


} // end namespace Kaleidoscope
