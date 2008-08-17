#include <string>

namespace Grids {
    virtual class Grids::Key {
        public:
            Key(std::string &serializedKey);
            std::string serialize();
        private:
            bool encrypted;
    };
}
