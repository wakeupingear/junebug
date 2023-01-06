#include "Files.h"
#include "Utils.h"

#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

using namespace junebug;
using namespace rapidjson;

Json::Json()
{
}

Json::Json(std::string data, bool isFile) : Json()
{
    if (isFile)
    {
        mPath = data;
        mIsFile = true;
    }

    if (data[0] == '{')
    {
        doc.Parse(data.c_str());

        if (!IsValid())
        {
            PrintLog("Json parse error:", std::to_string(doc.GetParseError()), "at", std::to_string(doc.GetErrorOffset()));
        }
    }
    else
    {
        std::ifstream file(data);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        doc.Parse(str.c_str());

        if (!IsValid())
        {
            PrintLog("Json parse error for ", data, ":", std::to_string(doc.GetParseError()), "at", std::to_string(doc.GetErrorOffset()));
        }
    }
}

Json::Json(Document &doc) : Json()
{
    this->doc.CopyFrom(doc, doc.GetAllocator());
}

Json::~Json()
{
}

std::string Json::Stringify() const
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

Document *Json::GetDoc()
{
    return &doc;
}

bool Json::IsValid() const
{
    return !doc.HasParseError();
}

GenericMemberIterator<false, UTF8<>, MemoryPoolAllocator<>> Json::Get(std::string key)
{
    return doc.FindMember(key.c_str());
}

rapidjson::Value *Json::GetActor(std::string &id)
{
    if (id.empty() || !doc.HasMember("actors"))
        return nullptr;
    auto &actorsV = doc["actors"];
    if (!actorsV.IsArray())
        return nullptr;
    for (auto &actor : actorsV.GetArray())
    {
        const auto &actorObj = actor.GetObject();
        if (GetString(actorObj, "id") == id)
        {
            print("found actor");
            return &actor;
        }
    }
    return nullptr;
}

void Json::Save()
{
    if (!mIsFile)
    {
        PrintLog("Cannot save Json object that was not loaded from a file");
        return;
    }

    FILE *fp = fopen(mPath.c_str(), "w");
    char buffer[1024];
    FileWriteStream fs(fp, buffer, sizeof(buffer));
    PrettyWriter<FileWriteStream> writer(fs);
    doc.Accept(writer);
    fclose(fp);
}