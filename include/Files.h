#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <string>

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

        std::string Stringify();

        Document *GetDoc();

    private:
        Document doc;
    };
}