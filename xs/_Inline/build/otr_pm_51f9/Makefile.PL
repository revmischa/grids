use ExtUtils::MakeMaker;
my %options = %{       
{
  'TYPEMAPS' => [
    '/usr/share/perl/5.10/ExtUtils/typemap'
  ],
  'NAME' => 'otr_pm_51f9',
  'CC' => 'gcc',
  'INC' => '-I/home/lazarus/dev/grids/grids/xs -I/usr/include',
  'LIBS' => [
    '-L/usr/lib -lotr'
  ],
  'VERSION' => '0.00'
}
};
WriteMakefile(%options);

# Remove the Makefile dependency. Causes problems on a few systems.
sub MY::makefile { '' }
