


#include <grids/utility.h>




namespace Grids
{

	Utility::Utility( )
	{


	}

	Utility::~Utility()
	{

	}

	// readIDFile reads a stored id file, which should
	// be stored as <GridsID>.gid
	// 
	GridsID Utility::readIDFromFile( )
	{
		std::string root;
		std::string idfile;
		std::ifstream readfile;
		std::string id_read;

		root = expandFilename( "~/.grids/" );

		idfile = root + "ID.gid";

		readfile.open( idfile.c_str() );

		if( readfile.fail() ) {
			id_read = "";
		}
		else {
			getline( readfile, id_read );
		}

		return id_read;
	}

	void Utility::writeIDToFile( GridsID new_id )
	{
		std::string root;
		std::string idfile;
		std::ofstream writefile;
		std::string id_write;

		id_write = new_id; // GridsIDs may not be std::strings in the future, and may need conversion

		root = expandFilename( "~/.grids/" );

		idfile = root + "ID.gid";

		writefile.open( idfile.c_str(), std::ios::trunc ); // delete the old contents of the id file, if there is any

		writefile << id_write << std::endl;
	}


	// expand_filename(filename)
	// Expand "~/" with the $HOME env. variable in a file name.
	// The caller must free the string after use.
	std::string Utility::expandFilename( std::string in_dir )
	{
		if ( in_dir.find( "~/" ) != std::string::npos ) 
		{
			std::string homedir = "";
			homedir = getenv("HOME");
			if ( !homedir.empty() )
				return homedir + in_dir.substr( 1 );
		}

		return in_dir;
	}

}

