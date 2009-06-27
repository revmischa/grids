package Crypt::OTR;

use 5.010000;
use strict;
use warnings;
use Carp;

use AutoLoader;

our $VERSION = '0.01';

sub AUTOLOAD {
    # This AUTOLOAD is used to 'autoload' constants from the constant()
    # XS function.

    my $constname;
    our $AUTOLOAD;
    ($constname = $AUTOLOAD) =~ s/.*:://;
    croak "&Crypt::OTR::constant not defined" if $constname eq 'constant';
    my ($error, $val) = constant($constname);
    if ($error) { croak $error; }
    {
	no strict 'refs';
	# Fixed between 5.005_53 and 5.005_61
#XXX	if ($] >= 5.00561) {
#XXX	    *$AUTOLOAD = sub () { $val };
#XXX	}
#XXX	else {
	    *$AUTOLOAD = sub { $val };
#XXX	}
    }
    goto &$AUTOLOAD;
}

require XSLoader;
XSLoader::load('Crypt::OTR', $VERSION);

#########################

=head1 NAME

Crypt::OTR - Do Off-The-Record message encryption

=head1 SYNOPSIS

    use Crypt::OTR;
    
    # call near the beginning of your program, should only be one per
    # process
    Crypt::OTR->init;

    # create OTR object, set up callbacks
    my $otr = new Crypt::OTR(
        account_name     => "alice",    # name of account associated with this keypair
        protocol_name    => "my_protocol_name", # e.g. 'AIM'
        max_message_size => 1024,            # how much to fragment
    );
    $otr->set_callback('inject' => \&otr_inject);
    $otr->set_callback('display' => \&otr_display);
    $otr->set_callback('connect' => \&otr_connect);
    $otr->set_callback('display' => \&otr_display);

    # create a context for user "bob"
    $otr->establish("bob");  # calls otr_inject($account_name, $protocol, $dest_account, $message)

    # send a message to bob
    my $plaintext = "hello, bob! this is a message from alice";
    if (my $ciphertext = $otr->encrypt("bob", $plaintext)) {
        $my_app->send_message_to_user("bob", $ciphertext);
    } else {
        warn "Your message was not sent - no encrypted conversation is established\n";
    }

    # called from bob's end
    if (my $plaintext = $otr->decrypt("alice", $ciphertext)) {
        print "alice: $plaintext\n";
    } else {
        warn "We received an encrypted message from alice but were unable to decrypt it\n";
    }

    # done with chats
    $otr->disconnect("bob");
    
    # CALLBACKS 
    #  (if writing a multithreaded application you will
    #   probably want to lock a mutex when sending/receiving)

    # called when OTR is ready to send a message after massaging it.
    # this method should transmit $message over a socket or somesuch
    sub otr_inject {
        my ($self, $account_name, $protocol, $dest_account, $message) = @_;
        $my_app->send_message_to_user($dest_account, $message);
    }

    # called after OTR has massaged an incoming message, possibly decrypting it
    sub otr_display {
        my ($self, $account_name, $protocol, $from_account, $message) = @_;
        $my_app->display_message($from_account, $message);
    }

    # called when a verified conversation is established with $from_account
    sub connect {
        my ($self, $from_account) = @_;
        print "Started verified conversation with $from_account\n";
    }

    # called when an unverified conversation is established with $from_account
    sub connect {
        my ($self, $from_account) = @_;
        print "Started unverified conversation with $from_account\n";
    }


=head1 DESCRIPTION

Perl wrapper around libotr2 - see
http://www.cypherpunks.ca/otr/README-libotr-3.2.0.txt

=head2 EXPORT

None by default.

=head1 METHODS

=over 4


=item init()

This method sets up OTR and initializes the global OTR context. It is
probably unsafe to call this more than once

=cut

sub init {
    crypt_otr_init();
}


=item new (%opts)

Options:
 'account_name'     => name of the account in your application
 'protocol_name'    => string identifying your application
 'max_message_size' => how many bytes messages should be fragmented into

=cut

sub new {
    my ($class, %opts) = @_;

    my $self = {

    };

    return bless $self, $class;
}




=head1 SEE ALSO

http://www.cypherpunks.ca/otr

=head1 AUTHOR

Patrick Tierney, E<lt>patrick.l.tierney@gmail.comE<gt>
Mischa Spiegelmock, E<lt>mspiegelmock@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2009 by Patrick Tierney, Mischa Spiegelmock

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.

=cut


1;
__END__
