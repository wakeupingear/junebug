#include "Files.h"

using namespace junebug;
using namespace rapidjson;

Json::Json()
{
}

Json::Json(std::string data) : Json()
{
    if (data[0] == '{')
    {
        doc.Parse(data.c_str());
    }
    else
    {
        std::ifstream file(data);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        doc.Parse(str.c_str());
    }
}

Json::Json(Document &doc) : Json()
{
    this->doc.CopyFrom(doc, doc.GetAllocator());
}

Json::~Json()
{
}

std::string Json::Stringify()
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

Document* Json::GetDoc()
{
    return &doc;
}