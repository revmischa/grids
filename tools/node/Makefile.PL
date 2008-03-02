use inc::Module::Install;
    
# Define metadata
name            'gridsnode';
all_from        '../../lib/Grids/Node.pm';
    
# Specific dependencies
requires        'Carp'              => 0;
#requires        'File::UserConfig'  => '0.04';
    
#no_index        'directory'         => 'demos';
    
#install_script  'bin/myscript';
 
install_share 'share';
   
auto_install;
WriteAll;
