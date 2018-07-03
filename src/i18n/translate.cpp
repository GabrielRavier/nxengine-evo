#include "translate.h"
#include "../common/json.hpp"
#include "../common/misc.h"
#include "../common/utf8.h"
#include "../ResourceManager.h"
#include "minibidi.h"
#include <fstream>
#include <vector>
#include <cstdint>

I18N::I18N() : _strings() {}

I18N::~I18N() {}

bool I18N::load()
{
    std::string path=ResourceManager::getInstance()->getLocalizedPath("system.json");
    std::ifstream fl;
    _strings.clear();
    fl.open(widen(path), std::ifstream::in | std::ifstream::binary);
    if (fl.is_open()) {
        nlohmann::json langfile = nlohmann::json::parse(fl);

        for (auto it = langfile.begin(); it != langfile.end(); ++it)
        {
            std::string result = it.value();
            std::vector<uint32_t> utf32result;
            utf8::utf8to32(result.begin(), result.end(), std::back_inserter(utf32result));
            doBidi(&utf32result[0], utf32result.size(), true, false, NULL, NULL);
            result.clear();
            utf8::utf32to8(utf32result.begin(), utf32result.end(), std::back_inserter(result));

            _strings[ it.key() ] = std::move(result);
        }
        return true;
    }
    return false;
}



const std::string& I18N::translate(const std::string& key)
{
    if (_strings.find(key) != _strings.end())
    {
        return _strings.at(key);
    }
    else
    {
        return key;
    }
}

