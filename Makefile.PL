use 5.008008;
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'Net',
#    VERSION_FROM      => 'lib/NetReg.pm', # finds $VERSION

    # required modules
    PREREQ_PM         => {
        Term::ReadLine => 0,
        Storable => 0,
        Crypt::RSA => 0,
    },

#    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
#      (ABSTRACT_FROM  => 'lib/NetReg.pm', # retrieve abstract from module
#       AUTHOR         => 'mish <mspiegelmock@gmail.com>') : ()),

    LIBS              => [''], # e.g., '-lm'
    DEFINE            => '', # e.g., '-DHAVE_SOMETHING'
    INC               => '-I.', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
    # OBJECT            => '$(O_FILES)', # link all the C files too
);
