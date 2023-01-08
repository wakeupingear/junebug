#include "Transitions.h"
#include "Game.h"
#include "Camera.h"

using namespace junebug;

SceneTransition::SceneTransition() : VisualActor()
{
    SetPersistent(true);
    SetDepth(JB_TRANSITION_DEPTH);
    Game::Get()->SetSceneTransitioning(true);
}
SceneTransition::SceneTransition(std::string newScene, float startTime, float pauseTime, float endTime, TwerpType curve) : SceneTransition()
{
    mNewScene = newScene;
    mStartTime = startTime;
    mPauseTime = pauseTime;
    mEndTime = endTime;
    mCurve = curve;

    mState = (!NearZero(mStartTime)) ? 0 : (!NearZero(mPauseTime)) ? 1
                                                                   : 2;
}
SceneTransition::~SceneTransition()
{
    Game::Get()->SetSceneTransitioning(false);
}
void SceneTransition::Update(float dt)
{
    mProg += dt;
    if (mState == 0)
    {
        if (mProg >= mStartTime)
        {
            mProg = 0;
            mState = (!NearZero(mPauseTime)) ? 1 : (!NearZero(mEndTime)) ? 2
                                                                         : 3;
            Game::Get()->ChangeScene(mNewScene);
        }
    }
    else if (mState == 1)
    {
        if (mProg >= mPauseTime)
        {
            mProg = 0;
            mState = (!NearZero(mEndTime)) ? 2 : 3;
        }
    }
    else if (mState == 2)
    {
        if (mProg >= mEndTime)
        {
            mState = 3;
        }
    }

    if (mState > 2)
    {
        Destroy();
    }
}

FadeTransition::FadeTransition(std::string newScene, float startTime, float pauseTime, float endTime, Color fadeCol, TwerpType curve) : SceneTransition(newScene, startTime, pauseTime, endTime, curve)
{
    mFadeCol = fadeCol;
    SetColor(fadeCol);
    mColor.a = 0.0f;
}
void FadeTransition::Update(float dt)
{
    SceneTransition::Update(dt);

    if (mState == 0)
    {
        mColor.a = (mFadeCol.a) * Twerp(0.0f, 1.0f, mProg / mStartTime, mCurve);
    }
    else if (mState == 2)
    {
        mColor.a = mFadeCol.a * Twerp(1.0f, 0.0f, mProg / mEndTime, mCurve);
    }
}
void FadeTransition::Draw()
{
    Camera *cam = Game::Get()->GetActiveCamera();
    if (!cam)
        return;

    Vec2<float> camPos = cam->GetPosition();
    DrawRectangle(camPos, camPos + cam->GetSize(), mColor);
}

SlideTransition::SlideTransition(std::string newScene, float startTime, float pauseTime, float endTime, float startDir, float endDir, Color fadeCol, TwerpType curve) : SceneTransition(newScene, startTime, pauseTime, endTime, curve)
{
    mFadeCol = fadeCol;
    SetColor(fadeCol);

    mDir = Vec2<float>(Cos(ToRadians(startDir)), -Sin(ToRadians(endDir)));
    mDir /= Max(Abs(mDir.x), Abs(mDir.y));
}
void SlideTransition::Update(float dt)
{
    SceneTransition::Update(dt);

    if (mState == 0)
    {
        SetPosition(Vec2<float>(
            Twerp(-1.0f, 0.0f, mProg / mStartTime, mCurve) * mDir.x,
            Twerp(-1.0f, 0.0f, mProg / mStartTime, mCurve) * mDir.y));
    }
    else if (mState == 2)
    {
        SetPosition(Vec2<float>(
            Twerp(0.0f, 1.0f, mProg / mEndTime, mCurve) * mDir.x, Twerp(0.0f, 1.0f, mProg / mEndTime, mCurve) * mDir.y));
    }
}
void SlideTransition::Draw()
{
    Camera *cam = Game::Get()->GetActiveCamera();
    if (!cam)
        return;

    Vec2<float> camPos = cam->GetPosition() + cam->GetSize() * GetPosition();
    DrawRectangle(camPos, camPos + cam->GetSize(), mColor);
}

ScrollTransition::ScrollTransition(std::string newScene, float time, float dir, Color bgCol, TwerpType curve) : SceneTransition(newScene, 0.0f, 0.0f, time, curve)
{
    SetColor(bgCol);

    mDir = Vec2<float>(Cos(ToRadians(dir)), -Sin(ToRadians(dir)));
    mDir /= Max(Abs(mDir.x), Abs(mDir.y));
}
ScrollTransition::~ScrollTransition()
{
    if (mPrevTex)
        SDL_DestroyTexture(mPrevTex);
}
void ScrollTransition::Update(float dt)
{
    SceneTransition::Update(dt);

    if (mState == 2)
    {
        SetPosition(Vec2<float>(
            Twerp(0.0f, -1.0f, mProg / mEndTime, mCurve) * mDir.x,
            Twerp(0.0f, -1.0f, mProg / mEndTime, mCurve) * mDir.y));
        mCurrPos = Vec2<float>(
            Twerp(1.0f, 1.0f, mProg / mEndTime, mCurve) * mDir.x,
            Twerp(1.0f, 1.0f, mProg / mEndTime, mCurve) * mDir.y);
    }
}
void ScrollTransition::Draw()
{
    Camera *cam = Game::Get()->GetActiveCamera();
    if (!cam)
        return;

    SDL_Texture *camTex = cam->GetTexture();
    if (!camTex)
        return;
    SDL_Texture *currTex = CopyTexture(camTex);
    if (!mPrevTex)
        mPrevTex = CopyTexture(camTex);

    DrawRectangle(cam->GetPosition(), cam->GetPosition() + cam->GetSize(), mColor);

    Vec2<float> prevPos = cam->GetPosition() + cam->GetSize() * GetPosition(), currPos = prevPos + cam->GetSize() * mCurrPos;
    DrawTexture(mPrevTex, prevPos, cam->GetSize());
    DrawTexture(currTex, currPos, cam->GetSize());

    SDL_DestroyTexture(currTex);
}