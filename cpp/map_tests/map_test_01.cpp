#include <iostream>
#include <string>
#include <map>

using namespace std;

int main()
{
	map< string, string > test_send;
	
	test_send.insert( pair<string, string>( "_event", "CHANGE_POSITION" )  );
	
	cout << test_send[0] << endl;
	
}
