


#include <kaleidoscope/test_gridsEnvironment.h>
#include <kaleidoscope/kaleidoscope.h>

#include <gtest/gtest.h>

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
		
	//GridsTest::GridsEnvironment* main_env = new GridsTest::GridsEnvironment();

    	//::testing::AddGlobalTestEnvironment( main_env );
	
	return RUN_ALL_TESTS();
}
