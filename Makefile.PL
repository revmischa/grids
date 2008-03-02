use 5.008008;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME              => 'Grids',
#    VERSION_FROM      => 'lib/GridsReg.pm', # finds $VERSION

    # required modules
    PREREQ_PM         => {
        Term::ReadLine => 0,
        Storable => 0,
        Crypt::RSA => 0,
    },

#    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
#      (ABSTRACT_FROM  => 'lib/GridsReg.pm', # retrieve abstract from module
#       AUTHOR         => 'mish <mspiegelmock@gmail.com>') : ()),

    LIBS              => [''],
    DEFINE            => '',
    INC               => '-I.',
);
