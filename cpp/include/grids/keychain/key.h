#pragma once
#include <string>

namespace Grids {
  class Key {
        public:
            Key(std::string &serializedKey);
            virtual std::string serialize();
        private:
            bool encrypted;
    };
}
