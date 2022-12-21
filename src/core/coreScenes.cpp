#include "Game.h"
#include "Files.h"
#include "Actors.h"
#include "Background.h"

using namespace junebug;

void Game::LoadScene(std::string scene)
{
    mSceneQueue.push(scene);
}

void Game::ReloadScene()
{
    if (IsSceneLoaded())
        mSceneQueue.push(mScene.name);
}

bool Game::IsSceneLoaded()
{
    return mScene.name != "";
}

const Scene &Game::GetCurrentScene() const
{
    return mScene;
}

void Game::LoadQueuedScenes()
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
                sceneInfo = new Json(GetAssetPaths().scenes + sceneStr + ".json");
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
                for (auto &layerRef : layersRef)
                {
                    if (layerRef.IsObject())
                    {
                        const auto &layerObj = layerRef.GetObject();
                        std::string id = Json::GetString(layerObj, "id");
                        if (id == "")
                            continue;

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
                    LoadActor(actorRef, newScene);
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
            // Print("Loaded scene", mScene.name, "with", mScene.layers.size(), "layer(s) and", mActors.size() - numPersistentActors, "actor(s)");
        }
        catch (std::exception &e)
        {
            PrintLog("Scene", sceneStr, "errored with", e.what());
        }
    }

    if (sceneInfo)
        delete sceneInfo;
}

const Vec2<int> Game::GetSceneSize()
{
    if (mScene.name.empty())
        return Vec2<int>::Zero;
    return mScene.size;
}

const std::string Game::GetSceneName()
{
    return mScene.name;
}