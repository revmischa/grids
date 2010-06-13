use 5.008008;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME              => 'Grids',
    VERSION_FROM      => 'lib/Grids.pm', # finds $VERSION

    # required modules
    PREREQ_PM         => {
        Data::UUID => 0,
        Term::ReadLine => 0,
        AnyEvent => 0,
        Storable => 0,
        Class::Autouse => 0,
        JSON::XS => 0,
        JSON => 0,
        Moose => 0,
        Getopt::Long => 0,
        FindBin => 0,
        Digest::SHA1 => 0,
        Sys::Hostname => 0,
    },

    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM  => 'lib/Grids.pm', # retrieve abstract from module
       AUTHOR         => 'Mischa Spiegelmock <mspiegelmock@gmail.com>') : ()),

    LIBS              => [''],
    DEFINE            => '',
    INC               => '-I.',


    DIR               => ['lib/Grids/VM/Register', 'lib/Grids/VM/Memory'],
);
