


#include <kaleidoscope/define.h>
#include <grids/define.h>

#include <grids/test_simpleObject.h>
#include <kaleidoscope/test_deviceTester.h>

#include <gtest/gtest.h>

#include <string>

namespace {
	
	// The fixture for testing class Grids::Object.
	class ObjectTest : public ::testing::Test {
	protected:
		
		ObjectTest(){
		}
		
		~ObjectTest(){
			
		}
		
		virtual void SetUp(){
			test_id = "12345678";
			create_val[ "req" ][ "attr" ][ "id" ] = test_id;		

			obj = new GridsTest::SimpleObject( d->getDevice(), &create_val );
		}
		
		static void SetUpTestCase() {
			d = new GridsTest::SimpleDevice();
		}
		
		static void TearDownTestCase() {
			delete d;
		}

		static GridsTest::SimpleDevice* d;
		GridsTest::SimpleObject* obj;

		Grids::Value create_val;
		Grids::GridsID test_id;
	};
		
	GridsTest::SimpleDevice* ObjectTest::d = NULL;

	/* 1. specified "req""attr""id" is the same as returned ID
	 * 2. Parent is NULL when not specified
	 */
	TEST_F(ObjectTest, initTest ) {
		
		EXPECT_STREQ( test_id.c_str(), obj->getID().c_str() ); 

		EXPECT_EQ( NULL, obj->getParent() );
	}
	
	TEST_F(ObjectTest, TestGetSetID){
		Grids::GridsID get_set_id = "112233444";
		
		obj->setID( get_set_id );
		
		EXPECT_STREQ( get_set_id.c_str(), obj->getID().c_str() );
	}

	TEST_F(ObjectTest, TestLoadPosition){
		Grids::Value load_value;

		Kal::Vec3D pos = Kal::Vec3D( 1.1f, 2.2f, 3.3f );
		Kal::Vec3D rot = Kal::Vec3D( 4.4f, 5.5f, 6.6f );
		Kal::Vec3D scl = Kal::Vec3D( 7.7f, 8.8f, 9.9f );
		
		obj->loadPosition( &load_value, pos, rot, scl );
		
		EXPECT_DOUBLE_EQ( pos.X, load_value[ "pos" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( pos.Y, load_value[ "pos" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( pos.Z, load_value[ "pos" ][ 2u ].asDouble() );

		EXPECT_DOUBLE_EQ( rot.X, load_value[ "rot" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( rot.Y, load_value[ "rot" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( rot.Z, load_value[ "rot" ][ 2u ].asDouble() );

		EXPECT_DOUBLE_EQ( scl.X, load_value[ "scl" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( scl.Y, load_value[ "scl" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( scl.Z, load_value[ "scl" ][ 2u ].asDouble() );
	}

	TEST_F(ObjectTest, getSetRoomIDTest ){
		Grids::GridsID temp_id = "3344555";
		obj->setRoomID( temp_id );
		
		EXPECT_EQ( temp_id.c_str(), obj->getRoomID().c_str() );
	}

	TEST_F(ObjectTest, getAttrTest1 ){
				
		Grids::Value attr_value;
		std::string attr_key = "lol hi";

		attr_value[ "req" ][ "attr" ][ "foo" ] = attr_key;
				
		Grids::Value* out_value = obj->getAttr( &attr_value );
		
		if( out_value->isNull() )
			FAIL();		
		
		if( (*out_value)[ "foo" ].empty() )
			FAIL();
		else{		
			EXPECT_STREQ( attr_value[ "req" ][ "attr" ][ "foo" ].asString().c_str(),
					    (*out_value)[ "foo" ].asString().c_str() ); 
		}
		
		delete out_value;
		
	}

	TEST_F(ObjectTest, getAttrTest2 ){
		
		Grids::Value* attr_value = obj->getAttr();
		
		if( !( (*attr_value)[ "id" ] ) )
			FAIL();
		else{							
			EXPECT_STREQ( test_id.c_str(), (*attr_value)[ "id" ].asString().c_str() );
		}
	}

	TEST_F(ObjectTest, getSetParent ){
		unsigned int test_object = 2345552;
			
		obj->setParent( (Grids::Object*)test_object );
		
		EXPECT_EQ( test_object, (unsigned int)(obj->getParent() ) );
	}

	TEST_F(ObjectTest, setParentValueTest ){
		Grids::Value temp_val;
		Grids::GridsID temp_id = "44553311";
			
		Grids::Object::setParentValue( &temp_val, temp_id );
		
		EXPECT_STREQ( temp_id.c_str(), temp_val[ "parent" ].asString().c_str() );
	}
	
	TEST_F(ObjectTest, setAttrPosRotSclTest){
		Kal::Vec3D pos = Kal::Vec3D( 1.1f, 2.2f, 3.3f );
		Kal::Vec3D rot = Kal::Vec3D( 4.4f, 5.5f, 6.6f );
		Kal::Vec3D scl = Kal::Vec3D( 7.7f, 8.8f, 9.9f );

		obj->setParent( NULL );

		obj->setAttrPosition( pos );
		obj->setAttrRotation( rot );
		obj->setAttrScale( scl );

		Grids::Value* out_attr = obj->getAttr();

		EXPECT_DOUBLE_EQ( pos.X, (*out_attr)[ "pos" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( pos.Y, (*out_attr)[ "pos" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( pos.Z, (*out_attr)[ "pos" ][ 2u ].asDouble() );
		
		EXPECT_DOUBLE_EQ( rot.X, (*out_attr)[ "rot" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( rot.Y, (*out_attr)[ "rot" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( rot.Z, (*out_attr)[ "rot" ][ 2u ].asDouble() );

		EXPECT_DOUBLE_EQ( scl.X, (*out_attr)[ "scl" ][ 0u ].asDouble() );
		EXPECT_DOUBLE_EQ( scl.Y, (*out_attr)[ "scl" ][ 1u ].asDouble() );
		EXPECT_DOUBLE_EQ( scl.Z, (*out_attr)[ "scl" ][ 2u ].asDouble() );
	}
	
	TEST_F(ObjectTest, getSetRecursiveTest ){

		Kal::Vec3D pos_1 = Kal::Vec3D( 1.1f, 2.2f, 3.3f );
		Kal::Vec3D rot_1 = Kal::Vec3D( 4.4f, 5.5f, 6.6f );
		Kal::Vec3D scl_1 = Kal::Vec3D( 7.7f, 8.8f, 9.9f );

		Kal::Vec3D pos_2 = Kal::Vec3D( 2.1f, 3.2f, 3.3f );
		Kal::Vec3D rot_2 = Kal::Vec3D( 4.24f, 76.5f, 6.6f );
		Kal::Vec3D scl_2 = Kal::Vec3D( 3.7f, 6.8f, 29.9f );

		Kal::Vec3D pos_3 = Kal::Vec3D( 1.11f, 21.2f, 32.3f );
		Kal::Vec3D rot_3 = Kal::Vec3D( 4.34f, 51.5f, 633.6f );
		Kal::Vec3D scl_3 = Kal::Vec3D( 71.217f, 8.38f, 9.9f );

		Kal::Vec3D pos_f = pos_1 + pos_2 + pos_3;
		Kal::Vec3D rot_f = rot_1 + rot_2 + rot_3;
		Kal::Vec3D scl_f = scl_1 * scl_2 * scl_3;

		Grids::GridsID id_1 = "21341232";
		Grids::GridsID id_2 = "23411111";
		Grids::GridsID id_3 = "5111234";
		
		Grids::Value val_1; val_1[ "req" ][ "attr" ][ "id" ] = id_1;		
		Grids::Value val_2; val_2[ "req" ][ "attr" ][ "id" ] = id_2;		
		Grids::Value val_3; val_3[ "req" ][ "attr" ][ "id" ] = id_3;		
		
		
		GridsTest::SimpleObject* obj_1 = new GridsTest::SimpleObject( d->getDevice(), &val_1 );
		GridsTest::SimpleObject* obj_2 = new GridsTest::SimpleObject( d->getDevice(), &val_2 );
		GridsTest::SimpleObject* obj_3 = new GridsTest::SimpleObject( d->getDevice(), &val_3 );		
		
		obj_3->setParent( obj_2 );
		obj_2->setParent( obj_1 );
		obj_1->setParent( NULL );		

		obj_1->setAttrPosition( pos_1 );
		obj_1->setAttrRotation( rot_1 );
		obj_1->setAttrScale( scl_1 );

		obj_2->setAttrPosition( pos_2 );
		obj_2->setAttrRotation( rot_2 );
		obj_2->setAttrScale( scl_2 );

		obj_3->setAttrPosition( pos_3 );
		obj_3->setAttrRotation( rot_3 );
		obj_3->setAttrScale( scl_3 );


		Kal::Vec3D pos_out = obj_3->getPosition();
		Kal::Vec3D rot_out = obj_3->getRotation();
		Kal::Vec3D scl_out = obj_3->getScale();
		
		EXPECT_DOUBLE_EQ( pos_f.X, pos_out.X );
		EXPECT_DOUBLE_EQ( pos_f.Y, pos_out.Y );
		EXPECT_DOUBLE_EQ( pos_f.Z, pos_out.Z );
		
		EXPECT_DOUBLE_EQ( rot_f.X, rot_out.X );
		EXPECT_DOUBLE_EQ( rot_f.Y, rot_out.Y );
		EXPECT_DOUBLE_EQ( rot_f.Z, rot_out.Z );

		EXPECT_DOUBLE_EQ( scl_f.X, scl_out.X );
		EXPECT_DOUBLE_EQ( scl_f.Y, scl_out.Y );
		EXPECT_DOUBLE_EQ( scl_f.Z, scl_out.Z );
	}
	TEST_F(ObjectTest, setPosRotSclTest ){

		Kal::Vec3D pos = Kal::Vec3D( 1.1f, 2.2f, 3.3f );
		Kal::Vec3D rot = Kal::Vec3D( 4.4f, 5.5f, 6.6f );
		Kal::Vec3D scl = Kal::Vec3D( 7.7f, 8.8f, 9.9f );

		obj->setParent( NULL );

		obj->setAttrPosition( pos );
		obj->setAttrRotation( rot );
		obj->setAttrScale( scl );

		Kal::Vec3D pos_out = obj->getPosition();
		Kal::Vec3D rot_out = obj->getRotation();
		Kal::Vec3D scl_out = obj->getScale();
		
		EXPECT_DOUBLE_EQ( pos.X, pos_out.X );
		EXPECT_DOUBLE_EQ( pos.Y, pos_out.Y );
		EXPECT_DOUBLE_EQ( pos.Z, pos_out.Z );
		
		EXPECT_DOUBLE_EQ( rot.X, rot_out.X );
		EXPECT_DOUBLE_EQ( rot.Y, rot_out.Y );
		EXPECT_DOUBLE_EQ( rot.Z, rot_out.Z );

		EXPECT_DOUBLE_EQ( scl.X, scl_out.X );
		EXPECT_DOUBLE_EQ( scl.Y, scl_out.Y );
		EXPECT_DOUBLE_EQ( scl.Z, scl_out.Z );
	}

	
	TEST_F(ObjectTest, addDeleteChildrenTest){
		unsigned int t_child_1 = 123121;
		unsigned int t_child_2 = 4134555;
		unsigned int t_child_3 = 23466643;
		
		std::vector< Grids::Object* > children_out;

		obj->addChild( (Grids::Object*)t_child_1 );
		obj->addChild( (Grids::Object*)t_child_2 );
		obj->addChild( (Grids::Object*)t_child_3 );
	
		children_out = obj->getChildren();
		
		EXPECT_EQ( t_child_1, (unsigned int)(children_out[0]) );
		EXPECT_EQ( t_child_2, (unsigned int)(children_out[1]) );
		EXPECT_EQ( t_child_3, (unsigned int)(children_out[2]) );
	}



}  // namespace

