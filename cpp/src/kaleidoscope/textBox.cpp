

#include <kaleidoscope/textBox.h>
#include <kaleidoscope/gui.h>
#include <kaleidoscope/rect.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/simpleCube.h>
#include <kaleidoscope/utility.h>

#include <SDL/SDL_opengl.h>
#include <SDL_ttf/SDL_ttf.h>

namespace Kaleidoscope
{
	TextBox::TextBox( Device* d, Grids::Value* in_val ) : GuiItem( d, in_val )  {
		Utility::puts( "new TextBox" );				
	}
	
	TextBox::~TextBox(){
		freeSurfaces();
	}	

	GridsID TextBox::requestCreateTextBox( Device* d, GuiItem* parent, Vec3D in_pos, Vec3D in_scl, std::string box_text ){
		GridsID parent_id = d->getInterface()->getObjectController()->getIdFromPointer( parent );
		
		Utility::puts( "Requested Parent: ", parent_id );
		
		return requestCreateTextBox( d, parent_id, in_pos, in_scl, box_text );		
	}

	GridsID TextBox::requestCreateTextBox( Device* d, GridsID parent, Vec3D in_pos, Vec3D in_scl, std::string box_text ){
		Grids::Value* temp_value = new Grids::Value();

		Utility::puts( "Requested Parent: ", parent );

		setRoom( temp_value, d->getMyRoom() );

		(*temp_value)[ "type" ][ "name" ] = "TextBox";
	
		(*temp_value)[ "text" ] = box_text;

		loadPosition( temp_value, in_pos, Vec3D( 0.0f, 0.0f, 0.0f), in_scl );

		return requestCreate( d, parent, temp_value );		
	}
		
	void TextBox::create( Device* d, Grids::Value* in_val ){
			
		//Utility::puts( "TextBox::create parent   ", attr[ "parent" ].asString());
		
		createTextBox( d, &attr );
	}
	
	void TextBox::createTextBox( Device* d, Grids::Value* in_val ){			
		//Utility::puts( "TextBox::createTextBox" );
				
		updateText( d, getTextFromAttr( &attr ) );

		//Utility::puts( "Got text" );		
	}
	
	void TextBox::updateText( Device* d, std::string new_text ){
		lock();
				
		text_box_sfc = Gui::createSDLTextSurface( d, new_text, d->screen_font );

		unlock();
	}

	std::string TextBox::getTextFromAttr( Grids::Value* in_value ){
		//Utility::puts( "Getting text" );
		return (*in_value)[ "text" ].asString();
	}

	/* The problem:
	 * On the server, there is a bunch of data, all in sync,
	 * but this does not directly map into what you need to draw.
	 * In order to know how to draw an object, you need to get it's type,
	 * Either with value[ "name" ] etc, or register these objects in arrays or hashes.
	 * When rendering, the objects use the synced values to actually draw things.
	 * 
	 * The system of storing GridsIDs with pointers should be implemented with the 
	 * create / request-create 
	 */

	/* These member functions will likely go in Gui::drawAll */	
	void TextBox::draw( Device* d ){
		//Utility::puts( "textBox draw" );
		Vec3D temp_pos = getPosition( );
		
		float pos_x = temp_pos.X;
		float pos_y = temp_pos.Y;
				
		//std::cout << pos_x << " : " << pos_y << std::endl;

		Vec3D txt_scale = getScale();

		lock();

		int temp_x = text_box_sfc->w * txt_scale.X;
		int temp_y = text_box_sfc->h * txt_scale.Y;

		glBindTexture( GL_TEXTURE_2D, d->texture );

		glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA, text_box_sfc->w, text_box_sfc->h, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, text_box_sfc->pixels );

		unlock();

		glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );

		glPushMatrix();

		glTranslatef( (pos_x / FONT_SCALE_X) , (pos_y / FONT_SCALE_Y), 0.0f );

		glBegin(GL_QUADS); {
			glTexCoord2i(0,0); glVertex2i(0, 0);
			glTexCoord2i(1,0); glVertex2i( temp_x*2, 0);
			glTexCoord2i(1,1); glVertex2i( temp_x*2, -temp_y*2);
			glTexCoord2i(0,1); glVertex2i(0, -temp_y*2);
		} glEnd();

		glPopMatrix();
	}
	
	/* Creates a new SDL_Surface based on the Grids Values. */
	void TextBox::update(){

	}
	
		
	SDL_Surface* TextBox::getSurface( Device* d){
		return text_box_sfc;
	}

	void TextBox::freeSurfaces(){
		SDL_FreeSurface( text_box_sfc );
	}
		

} // end namespace Kaleidoscope
