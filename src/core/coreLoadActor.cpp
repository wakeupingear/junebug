
#include "Game.h"
#include "Files.h"
#include "Actors.h"
#include "Background.h"
#include "Tileset.h"

using namespace junebug;

void Game::LoadActor(rapidjson::Value &actorRef, Scene &newScene)
{
    if (!actorRef.IsObject())
        return;

    const auto &actorObj = actorRef.GetObject();
    std::string type = Json::GetString(actorObj, "type");
    if (type == "")
        return;

    auto it = mActorConstructors.find(type);
    if (it == mActorConstructors.end())
    {
        PrintLog("Actor", type, "is not registered");
        return;
    }

    PureActor *actor = it->second();
    actor->SetPersistent(Json::GetBool(actorObj, "persistent", false));
    actor->mId = Json::GetString(actorObj, "id");

    actor->SetDepth(Json::GetNumber<int>(actorObj, "depth", actor->GetDepth()));
    std::string layerId = Json::GetString(actorObj, "layer");
    if (layerId != "")
    {
        auto it = newScene.layers.find(layerId);
        if (it != newScene.layers.end())
        {
            actor->SetDepth(it->second.depth);
        }
    }

    VisualActor *visualActor = dynamic_cast<VisualActor *>(actor);
    if (visualActor)
    {
        visualActor->SetPosition(Json::GetVec2<float>(actorObj, "pos", Vec2<>::Zero));

        if (actorRef.HasMember("scale"))
        {
            if (actorRef["scale"].IsArray())
                visualActor->SetScale(Json::GetVec2<float>(actorObj, "scale", Vec2(1.0f, 1.0f)));
            else
                visualActor->SetScale(Json::GetNumber<float>(actorObj, "scale", 1.0f));
        }

        visualActor->SetRotation(Json::GetNumber<float>(actorObj, "rotation"));
        visualActor->SetRoundToCamera(Json::GetBool(actorObj, "roundToCamera", false));

        auto color = Json::GetNumberArray<float>(actorObj, "color");
        if (color.size() == 3)
            visualActor->SetColor(Color(color[0], color[1], color[2]));
        else if (color.size() == 4)
            visualActor->SetColor(Color(color[0], color[1], color[2], color[3]));

        int alpha = Json::GetInt(actorObj, "alpha", -1);
        if (alpha != -1)
            visualActor->SetAlpha(alpha);
        else
        {
            float alphaF = Json::GetFloat(actorObj, "alpha", -1.0f);
            if (alphaF > -0.5f)
                visualActor->SetAlpha(alphaF);
        }

        std::string sprite = Json::GetString(actorObj, "sprite");
        if (sprite != "")
            visualActor->SetSprite(sprite);

        PhysicalActor *physActor = dynamic_cast<PhysicalActor *>(actor);
        if (physActor)
        {
            physActor->SetGravityOffset(Json::GetVec2<float>(actorObj, "gravity", Vec2(0.0f, 0.0f)));
            physActor->SetStatic(Json::GetBool(actorObj, "static", physActor->IsStatic()));
            physActor->SetBounce(Json::GetNumber<float>(actorObj, "bounce"));
            physActor->SetMass(Json::GetNumber<float>(actorObj, "mass", physActor->GetMass()));
            physActor->SetCollType(
                static_cast<CollType>(Json::GetNumber<int>(actorObj, "collType", static_cast<int>(physActor->GetCollType()))));
            physActor->SetCollLayer(Json::GetString(actorObj, "collLayer", physActor->GetCollLayer()));
        }

        Background *bg = dynamic_cast<Background *>(actor);
        if (bg)
        {
            float rate = Json::GetNumber<float>(actorObj, "rate", -100.0f);
            if (rate > -99.0f)
                bg->SetRate(rate);
            else
                bg->SetRate(Json::GetVec2<float>(actorObj, "rate", Vec2<>::Zero));

            bg->SetOffset(Json::GetVec2<float>(actorObj, "offset", Vec2<>::Zero));

            bool tile = Json::GetBool(actorObj, "tile", false);
            if (tile)
                bg->SetTile(tile);
            else
                bg->SetTile(
                    Json::GetVec2<bool>(actorObj, "tile"));
        }

        Tileset *tileset = dynamic_cast<Tileset *>(actor);
        if (tileset)
        {
            tileset->SetTileSize(Json::GetVec2<int>(actorObj, "tileSize", Vec2<int>::Zero));
            tileset->SetTiles(Json::GetNumberArray2D<int>(actorObj, "tiles"));

            if (actorObj.HasMember("colliders") && actorObj["colliders"].IsArray())
            {
                tileset->SetCollType(CollType::TilesetIndividual);

                auto &colliders = tileset->GetColliders();
                const auto &scale = tileset->GetScale();
                Vertices squareCollider = {
                    Vec2<double>(0.0, 0.0),
                    Vec2<double>(tileset->GetTileSize().x * scale.x, 0.0),
                    Vec2<double>(tileset->GetTileSize().x * scale.x, tileset->GetTileSize().y * scale.y),
                    Vec2<double>(0.0, tileset->GetTileSize().y * scale.y)};

                const auto &collList = actorObj["colliders"].GetArray();
                for (auto &collider : collList)
                {
                    if (collider.IsBool() && collider.GetBool())
                    {
                        colliders.push_back(squareCollider);
                    }
                    else if (collider.IsArray())
                    {
                        Vertices newCollider;
                        for (auto &point : collider.GetArray())
                        {
                            if (point.IsArray())
                            {
                                const auto &pointArr = point.GetArray();
                                if (pointArr.Size() == 2)
                                {
                                    newCollider.push_back(Vec2<double>(pointArr[0].GetDouble() * scale.x, pointArr[1].GetDouble() * scale.y) * tileset->GetTileSize());
                                }
                            }
                        }
                        colliders.push_back(newCollider);
                    }
                    else
                        colliders.push_back({});
                }
            }

            if (actorObj.HasMember("collMode"))
            {
                if (actorObj["collMode"].IsString())
                {
                    std::string collMode = Json::GetString(actorObj, "collMode");
                    if (collMode == "individual")
                        tileset->SetCollType(CollType::TilesetIndividual);
                    else if (collMode == "merged")
                        tileset->SetCollType(CollType::TilesetMerged);
                    else if (collMode == "none")
                        tileset->SetCollType(CollType::None);
                }
                else if (actorObj["collMode"].IsInt())
                {
                    tileset->SetCollType(
                        (CollType)Json::GetInt(actorObj, "collMode", (int)tileset->GetCollType()));
                }
            }

            tileset->CalculateNumTiles();
            tileset->SetCollLayer(Json::GetString(actorObj, "collLayer", tileset->GetCollLayer()));

            tileset->SetEditMode((Tileset::TilesetEditMode)Json::GetInt(actorObj, "editMode", (int)tileset->GetEditMode()));
        }
    }

    LoadActor(actor, actorRef, newScene);
}
