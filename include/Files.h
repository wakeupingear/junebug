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
#include <vector>

using namespace rapidjson;

namespace junebug
{
    class Json
    {
    public:
        Json();
        Json(std::string data, bool isFile = false);
        Json(Document &doc);
        ~Json();

        std::string Stringify() const;

        Document *GetDoc();
        bool IsValid() const;
        GenericMemberIterator<false, UTF8<>, MemoryPoolAllocator<>>
        Get(std::string key);

        void Save();

#pragma region Getters
        static int GetInt(const Value &val, int def = 0)
        {
            if (val.IsInt())
                return val.GetInt();
            else if (val.IsInt64())
                return (int)val.GetInt64();
            else if (val.IsUint())
                return (int)val.GetUint();
            else if (val.IsUint64())
                return (int)val.GetUint64();
            else if (val.IsString())
            {
                std::string str = val.GetString();
                try
                {
                    return std::stoi(str);
                }
                catch (...)
                {
                    return def;
                }
            }

            return def;
        }
        static int GetInt(const GenericObject<false, Value> &obj, std::string key, int def = 0)
        {
            if (obj.HasMember(key.c_str()))
                return GetInt(obj[key.c_str()], def);
            return def;
        }
        static int GetInt(Json *json, std::string key, int def = 0)
        {
            if (json && json->IsValid())
                return GetInt(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        static float GetFloat(const Value &val, float def = 0)
        {
            if (val.IsFloat())
                return val.GetFloat();
            else if (val.IsString())
            {
                std::string str = val.GetString();
                try
                {
                    return std::stof(str);
                }
                catch (...)
                {
                    return def;
                }
            }

            return def;
        }
        static float GetFloat(const GenericObject<false, Value> &obj, std::string key, float def = 0.0f)
        {
            if (obj.HasMember(key.c_str()))
                return GetFloat(obj[key.c_str()], def);
            return def;
        }
        static float GetFloat(Json *json, std::string key, float def = 0.0f)
        {
            if (json && json->IsValid())
                return GetFloat(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        template <typename T>
        static T GetNumber(const Value &val, T def = 0)
        {
            if (val.IsInt())
                return (T)val.GetInt();
            else if (val.IsFloat())
                return (T)val.GetFloat();
            else if (val.IsString())
            {
                std::string str = val.GetString();
                try
                {
                    return (T)std::stof(str);
                }
                catch (...)
                {
                    return def;
                }
            }
            else if (val.IsBool())
                return (T)val.GetBool();
            else if (val.IsDouble())
                return (T)val.GetDouble();
            else if (val.IsInt64())
                return (T)val.GetInt64();
            else if (val.IsUint())
                return (T)val.GetUint();
            else if (val.IsUint64())
                return (T)val.GetUint64();

            return def;
        }
        template <typename T>
        static T GetNumber(const GenericObject<false, Value> &obj, std::string key, T def = 0)
        {
            if (obj.HasMember(key.c_str()))
                return GetNumber(obj[key.c_str()], def);
            return def;
        }
        template <typename T>
        static T GetNumber(Json *json, std::string key, T def = 0)
        {
            if (json && json->IsValid())
                return GetNumber(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        template <typename T>
        static Vec2<T> GetVec2(const GenericObject<false, Value> &obj, std::string key, Vec2<T> def = Vec2<T>::Zero)
        {
            Vec2<T> res(def);

            if (obj.HasMember(key.c_str()))
            {
                auto &val = obj[key.c_str()];
                if (val.IsArray())
                {
                    if (val.Size() > 0)
                    {
                        res.x = (T)GetNumber<T>(val[0]);
                        if (val.Size() > 1)
                            res.y = (T)GetNumber<T>(val[1]);
                    }
                }
            }

            return res;
        }
        template <typename T>
        static Vec2<T> GetVec2(const Value &val, Vec2<T> def = Vec2<T>::Zero)
        {
            Vec2<T> res(def);

            if (val.IsArray())
            {
                if (val.Size() > 0)
                {
                    res.x = (T)GetNumber<T>(val[0]);
                    if (val.Size() > 1)
                        res.y = (T)GetNumber<T>(val[1]);
                }
            }

            return res;
        }
        template <typename T>
        static Vec2<T> GetVec2(Json *json, std::string key, Vec2<T> def = Vec2<T>::Zero)
        {
            if (json && json->IsValid())
                return GetVec2(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        template <typename T>
        static Vec3<T> GetVec3(const GenericObject<false, Value> &obj, std::string key, Vec3<T> def = Vec3<T>::Zero)
        {
            Vec3<T> res(def);

            if (obj.HasMember(key.c_str()))
            {
                auto &val = obj[key.c_str()];
                if (val.IsArray())
                {
                    if (val.Size() > 0)
                    {
                        res.x = GetNumber<T>(val[0]);
                        if (val.Size() > 1)
                        {
                            res.y = GetNumber<T>(val[1]);
                            if (val.Size() > 2)
                                res.z = GetNumber<T>(val[2]);
                        }
                    }
                }
            }

            return res;
        }
        template <typename T>
        static Vec3<T> GetVec3(const Value &val, Vec3<T> def = Vec3<T>::Zero)
        {
            Vec3<T> res(def);

            if (val.IsArray())
            {
                if (val.Size() > 0)
                {
                    res.x = GetNumber<T>(val[0]);
                    if (val.Size() > 1)
                    {
                        res.y = GetNumber<T>(val[1]);
                        if (val.Size() > 2)
                            res.z = GetNumber<T>(val[2]);
                    }
                }
            }

            return res;
        }
        template <typename T>
        static Vec3<T> GetVec3(Json *json, std::string key, Vec3<T> def = Vec3<T>::Zero)
        {
            if (json && json->IsValid())
                return GetVec3(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        static bool GetBool(const Value &val, bool def = false)
        {
            if (val.IsBool())
                return val.GetBool();
            else if (val.IsString())
            {
                std::string str = val.GetString();
                if (str == "true")
                    return true;
                else if (str == "false")
                    return false;
            }
            else if (val.IsInt())
                return val.GetInt() != 0;
            else if (val.IsFloat())
                return val.GetFloat() != 0;
            else if (val.IsDouble())
                return val.GetDouble() != 0;
            else if (val.IsInt64())
                return val.GetInt64() != 0;
            else if (val.IsUint())
                return val.GetUint() != 0;
            else if (val.IsUint64())
                return val.GetUint64() != 0;

            return def;
        }
        static bool GetBool(const GenericObject<false, Value> &obj, std::string key, bool def = false)
        {
            if (obj.HasMember(key.c_str()))
                return GetBool(obj[key.c_str()], def);
            return def;
        }
        static bool GetBool(Json *json, std::string key, bool def = false)
        {
            if (json && json->IsValid())
                return GetBool(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        static std::string GetString(const Value &val, std::string def = "")
        {
            if (val.IsString())
                return val.GetString();
            else if (val.IsInt())
                return std::to_string(val.GetInt());
            else if (val.IsFloat())
                return std::to_string(val.GetFloat());
            else if (val.IsDouble())
                return std::to_string(val.GetDouble());
            else if (val.IsInt64())
                return std::to_string(val.GetInt64());
            else if (val.IsUint())
                return std::to_string(val.GetUint());
            else if (val.IsUint64())
                return std::to_string(val.GetUint64());
            else if (val.IsBool())
                return val.GetBool() ? "true" : "false";

            return def;
        }
        static std::string GetString(const GenericObject<false, Value> &obj, std::string key, std::string def = "")
        {
            if (obj.HasMember(key.c_str()))
                return GetString(obj[key.c_str()], def);
            return def;
        }
        static std::string GetString(Json *json, std::string key, std::string def = "")
        {
            if (json && json->IsValid())
                return GetString(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        template <typename T>
        static std::vector<T> GetNumberArray(const Value &val, std::vector<T> def = std::vector<T>())
        {
            std::vector<T> res;

            if (val.IsArray())
            {
                for (auto &v : val.GetArray())
                {
                    res.push_back(GetNumber<T>(v));
                }
            }

            if (res.size() == 0)
                return def;

            return res;
        }
        template <typename T>
        static std::vector<T> GetNumberArray(const GenericObject<false, Value> &obj, std::string key, std::vector<T> def = std::vector<T>())
        {
            if (obj.HasMember(key.c_str()))
                return GetNumberArray(obj[key.c_str()], def);
            return def;
        }
        template <typename T>
        static std::vector<T> GetNumberArray(Json *json, std::string key, std::vector<T> def = std::vector<T>())
        {
            if (json && json->IsValid())
                return GetNumberArray(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        template <typename T>
        static std::vector<std::vector<T>> GetNumberArray2D(const Value &val, std::vector<std::vector<T>> def = std::vector<std::vector<T>>())
        {
            std::vector<std::vector<T>> res;

            if (val.IsArray())
            {
                for (auto &v : val.GetArray())
                {
                    res.push_back(GetNumberArray<T>(v));
                }
            }

            return res;
        }
        template <typename T>
        static std::vector<std::vector<T>> GetNumberArray2D(const GenericObject<false, Value> &obj, std::string key, std::vector<std::vector<T>> def = std::vector<std::vector<T>>())
        {
            if (obj.HasMember(key.c_str()))
                return GetNumberArray2D(obj[key.c_str()], def);
            return def;
        }
        template <typename T>
        static std::vector<std::vector<T>> GetNumberArray2D(Json *json, std::string key, std::vector<std::vector<T>> def = std::vector<std::vector<T>>())
        {
            if (json && json->IsValid())
                return GetNumberArray2D(json->GetDoc()->GetObject(), key, def);
            return def;
        }

        static std::vector<std::string> GetStringArray(const Value &val, std::vector<std::string> def = std::vector<std::string>())
        {
            std::vector<std::string> res;

            if (val.IsArray())
            {
                for (auto &v : val.GetArray())
                {
                    res.push_back(GetString(v));
                }
            }

            return res;
        }
        static std::vector<std::string> GetStringArray(const GenericObject<false, Value> &obj, std::string key, std::vector<std::string> def = std::vector<std::string>())
        {
            if (obj.HasMember(key.c_str()))
                return GetStringArray(obj[key.c_str()], def);
            return def;
        }
        static std::vector<std::string> GetStringArray(Json *json, std::string key, std::vector<std::string> def = std::vector<std::string>())
        {
            if (json && json->IsValid())
                return GetStringArray(json->GetDoc()->GetObject(), key, def);
            return def;
        }
#pragma endregion

#pragma region Scene Utils
        rapidjson::Value *GetActor(std::string &id);
#pragma endregion

    protected:
        Document doc;

    private:
        bool mIsFile{false};
        std::string mPath{""};
    };
}