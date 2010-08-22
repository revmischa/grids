#!/usr/bin/env perl

# This is a helper script to compile the protocol definitions into class files

use Moose;
use Google::ProtocolBuffers;
use FindBin;
use lib "$FindBin::Bin/../lib";
use Grids::Protocol::Serializer::ProtocolBuffer;

Grids::Protocol::Serializer::ProtocolBuffer->compile;
