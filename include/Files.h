#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <string>
#include <fstream>

using namespace rapidjson;

namespace junebug
{
    class Json
    {
    public:
        Json();
        Json(std::string data);
        Json(Document &doc);
        ~Json();

        std::string Stringify() const;

        Document *GetDoc();

        friend std::ostream &operator<<(std::ostream &os,   Json &json) {
            std::string str = json.Stringify();
            os<<str;
            return os;
        }

    private:
        Document doc;
    };
}