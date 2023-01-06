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
    while (!mSceneQueue.empty())
    {
        std::string sceneStr = mSceneQueue.front();
        mSceneQueue.pop();

        Scene newScene;

        try
        {
            // Obtain the scene info
            if (mSceneInfo)
            {
                delete mSceneInfo;
                mSceneInfo = nullptr;
            }

            if (sceneStr[0] == '{')
                mSceneInfo = new Json(sceneStr);
            else if (!StringEndsWith(sceneStr, ".json"))
                mSceneInfo = new Json(GetAssetPaths().scenes + sceneStr + ".json", true);
            else
                mSceneInfo = new Json(sceneStr + ".json", true);

            if (!mSceneInfo->IsValid())
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
            const auto &sizeRef = mSceneInfo->Get("size")->value.GetArray();
            if (sizeRef.Size() != 2)
            {
                PrintLog("Scene", sceneStr, "has invalid size");
                continue;
            }
            newScene.size = Vec2(sizeRef[0].GetInt(), sizeRef[1].GetInt());

            // Check if the key "gravity" exists
            SetGravity(Json::GetVec2<float>(mSceneInfo->GetDoc()->GetObject(), "gravity", mGravity));

            // Load the layers
            if (mSceneInfo->Get("layers")->value.IsArray())
            {
                const auto &layersRef = mSceneInfo->Get("layers")->value.GetArray();
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
            if (mSceneInfo->Get("actors")->value.IsArray())
            {
                Vec2<int> tempPos;

                const auto &actorsRef = mSceneInfo->Get("actors")->value.GetArray();
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
        }
        catch (std::exception &e)
        {
            PrintLog("Scene", sceneStr, "errored with", e.what());
        }
    }
}

const Vec2<int> Game::GetSceneSize()
{
    return mScene.size;
}

const std::string Game::GetSceneName()
{
    return mScene.name;
}