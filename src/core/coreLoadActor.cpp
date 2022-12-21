#include "Game.h"
#include "Files.h"
#include "Actors.h"
#include "Background.h"

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
    actor->SetPersistent(Json::GetBool(actorObj, "persistent"));

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
        visualActor->SetScale(Json::GetVec2<float>(actorObj, "scale", Vec2(1.0f, 1.0f)));
        visualActor->SetRotation(Json::GetNumber<float>(actorObj, "rotation"));

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

        Background *bg = dynamic_cast<Background *>(actor);
        if (bg)
        {
            float rate = Json::GetNumber<float>(actorObj, "rate", -100.0f);
            if (rate > -99.0f)
                bg->SetRate(rate);
            else
                bg->SetRate(Json::GetVec2<float>(actorObj, "rate", Vec2<>::Zero));

            bg->SetOffset(Json::GetVec2<float>(actorObj, "offset", Vec2<>::Zero));

            bool tile = Json::GetBool(actorObj, "tile");
            if (tile)
                bg->SetTile(tile);
            else
                bg->SetTile(
                    Json::GetVec2<bool>(actorObj, "tile"));
        }
    }

    LoadActor(actor, actorRef, newScene);
}
