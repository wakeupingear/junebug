#include "JGame.h"
#include "Files.h"
#include "Actors.h"

using namespace junebug;

void JGame::LoadScene(std::string scene)
{
    mSceneQueue.push(scene);
}

void JGame::ReloadScene()
{
    if (IsSceneLoaded())
        mSceneQueue.push(mScene.name);
}

bool JGame::IsSceneLoaded()
{
    return mScene.name != "";
}

const Scene &JGame::GetCurrentScene() const
{
    return mScene;
}

void JGame::LoadQueuedScenes()
{
    Json *sceneInfo = nullptr;
    while (!mSceneQueue.empty())
    {
        std::string sceneStr = mSceneQueue.front();
        mSceneQueue.pop();

        Scene newScene;

        try
        {
            // Obtain the scene info
            if (sceneInfo)
            {
                delete sceneInfo;
                sceneInfo = nullptr;
            }

            if (sceneStr[0] == '{')
                sceneInfo = new Json(sceneStr);
            else if (!StringEndsWith(sceneStr, ".json"))
                sceneInfo = new Json("assets/scenes/" + sceneStr + ".json");
            else
                sceneInfo = new Json(sceneStr + ".json");

            if (!sceneInfo->IsValid())
            {
                PrintLog("Scene", sceneStr, "is invalid");
                continue;
            }

            // Unload the current scene
            int numPersistentActors = 0;
            std::vector<PureActor *> destroyActors;
            for (PureActor *actor : mActors)
            {
                if (!actor->IsPersistent())
                    destroyActors.push_back(actor);
                else
                    numPersistentActors++;
            }
            for (PureActor *actor : destroyActors)
                delete actor;
            mScene.layers.clear();

            // Load the new scene
            const auto &sizeRef = sceneInfo->Get("size")->value.GetArray();
            if (sizeRef.Size() != 2)
            {
                PrintLog("Scene", sceneStr, "has invalid size");
                continue;
            }
            newScene.size = Vec2(sizeRef[0].GetInt(), sizeRef[1].GetInt());

            // Load the layers
            if (sceneInfo->Get("layers")->value.IsArray())
            {
                const auto &layersRef = sceneInfo->Get("layers")->value.GetArray();
                newScene.layers.reserve(layersRef.Size());
                for (auto &layerRef : layersRef) {
                    if (layerRef.IsObject())
                    {
                        const auto &layerObj = layerRef.GetObject();
                        std::string id = Json::GetString(layerObj, "id");
                        if (id == "") continue;

                        Layer layer;
                        layer.id = id;
                        layer.name = Json::GetString(layerObj, "name");
                        layer.depth = Json::GetNumber<int>(layerObj, "depth");
                        if (layerObj.HasMember("visible"))
                            layer.visible = layerObj["visible"].GetBool();

                        newScene.layers[layer.id] = layer;
                    }
                }
            }

            // Load the actors
            if (sceneInfo->Get("actors")->value.IsArray())
            {
                Vec2<int> tempPos;

                const auto &actorsRef = sceneInfo->Get("actors")->value.GetArray();
                for (auto &actorRef : actorsRef)
                    if (actorRef.IsObject())
                    {
                        const auto &actorObj = actorRef.GetObject();
                        std::string type = Json::GetString(actorObj, "type");
                        if (type == "")
                            continue;

                        auto it = mActorConstructors.find(type);
                        if (it == mActorConstructors.end())
                        {
                            PrintLog("Actor", type, "is not registered");
                            continue;
                        }

                        PureActor *actor = it->second(Json::GetVec2<int>(actorObj, "pos"));
                        actor->SetPersistent(Json::GetBool(actorObj, "persistent"));

                        std::string layerId = Json::GetString(actorObj, "layer");
                        if (layerId != "") {
                            auto it = newScene.layers.find(layerId);
                            if (it != newScene.layers.end()) {
                                actor->SetDepth(it->second.depth);
                            }
                        }

                        VisualActor *visualActor = dynamic_cast<VisualActor *>(actor);
                        if (visualActor)
                        {
                            visualActor->SetScale(Json::GetVec2<float>(actorObj, "scale", Vec2(1.0f, 1.0f)));
                            visualActor->SetRotation(Json::GetNumber<float>(actorObj, "rotation"));
                        }

                        if (visualActor) Print(*visualActor);
                        else Print(*actor);
                    }
            }

            // Set the name of the scene
            newScene.name = sceneStr;
            size_t dotPos = sceneStr.find_last_of('.');
            if (dotPos != std::string::npos)
                newScene.name = sceneStr.substr(0, dotPos);
            size_t slashPos = newScene.name.find_last_of('/');
            if (slashPos != std::string::npos)
                newScene.name = newScene.name.substr(slashPos + 1);

            mScene = newScene;
            Print("Loaded scene", mScene.name, "with", mScene.layers.size(), "layer(s) and", mActors.size() - numPersistentActors, "actor(s)");
        }
        catch (std::exception &e)
        {
            PrintLog("Scene", sceneStr, "errored with", e.what());
        }
    }

    if (sceneInfo)
        delete sceneInfo;
}