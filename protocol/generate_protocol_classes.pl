#!/usr/bin/env perl

# This is a helper script to compile the protocol definitions into class files

use Moose;
use Google::ProtocolBuffers;
use FindBin;
use lib "$FindBin::Bin/../lib";
use Grids::Protocol::Serializer::ProtocolBuffer;

# find .proto files
my $proto_dir = $FindBin::Bin;
my @files = glob("$proto_dir/*.proto");

# generate perl files
#Grids::Protocol::Serializer::ProtocolBuffer->compile(@files);

# generate CPP files
my $PROTOC = '/home/bobo/dev/protobuf-2.3.0/src/protoc';
my $file_list = join(' ', @files);
`$PROTOC --proto_path=$proto_dir --cpp_out=$proto_dir/cpp $file_list`
