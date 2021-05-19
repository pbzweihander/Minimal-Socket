/**
 * Author:    Andrea Casalino
 * Created:   01.28.2020
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef SAMPLE_NAMESMAP_H
#define SAMPLE_NAMESMAP_H

#include <map>
#include <string>

namespace sck::sample {
    class NamesMap {
    public:
        NamesMap();

        inline const std::string& getCursorName() const { return this->cursor->first; };
        inline const std::string& getCursorSurname() const { return this->cursor->second; };

        static const std::string& getSurname(const std::string& name);

        NamesMap& operator++();

        inline static std::size_t size() { return namesSurnames.size(); };

    private:
        static const std::map<std::string, std::string> namesSurnames;
        static const std::string unknown;

        std::map<std::string, std::string>::const_iterator cursor;
    };
}

#endif
