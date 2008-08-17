#include <string>
#include "grids/keychain/publicKey.h"
#include "grids/keychain/privateKey.h"

namespace Grids {

    class Keychain {

      public:

        // construct a new Keychain from a serialized string
        Keychain(std::string &serializedKey);

        // construct a new Keychain with public and or private keys
        Keychain(PublicKey inPubkey, PrivateKey inPrivkey);

        Keychain();
        ~Keychain();

        // return a serialized version of this in pre-allocated string outStr
        uint32_t serialize(std::string *outStr);

        // create a new public/private keypair, optional size in bits arg
        static void generateKeypair();
        static void generateKeypair(uint32_t size);
        // encrypt keypair with passphrase
        static void generateKeypair(std::string &passphrase);
        static void generateKeypair(uint32_t size, std::string &passphrase);

        // apply privkey to plaintext, saving ciphertext in outStr
        //  outStr must be allocated
        // returns: success or failure
        bool encrypt(const std::string *plaintext, std::string *outStr);

        // apply privkey to ciphertext, saving plaintext in outStr
        // returns: success or failure
        bool decrypt(const std::string *ciphertext, std::string *outStr);

        // etc...
        bool sign(const std::string *message, std::string *outStr);
        bool verify(const std::string *plaintext, PublicKey *origPubkey);

        // getters
        PublicKey *getPbkey();
        PublicKey *getPrivkey();
        bool isEncrypted();

      private:
        PublicKey *pubkey;
        PrivateKey *privkey;
        bool encrypted; // is the keypair currently encrypted with a passphrase?
    };

}
