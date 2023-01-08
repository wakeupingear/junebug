#include "Actors.h"
#include "Twerp.h"

namespace junebug
{

#define JB_TRANSITION_DEPTH 100000

    class SceneTransition : public VisualActor
    {
    public:
        SceneTransition();
        SceneTransition(std::string newScene, float startTime, float pauseTime, float endTime, TwerpType curve);

        ~SceneTransition();

        virtual void Update(float dt) override;
        virtual void Draw() override = 0;

    protected:
        std::string mNewScene;
        float mStartTime = 0.0f, mPauseTime = 0.0f, mEndTime = 0.0f;
        TwerpType mCurve;

        int mState = 0;
        float mProg = 0.0f;
    };

    class FadeTransition : public SceneTransition
    {
    public:
        FadeTransition(std::string newScene, float startTime, float pauseTime, float endTime, Color fadeCol, TwerpType curve);

        void Update(float dt) override;
        void Draw() override;

    private:
        Color mFadeCol;
    };

    class SlideTransition : public SceneTransition
    {
    public:
        SlideTransition(std::string newScene, float startTime, float pauseTime, float endTime, float startDir, float endDir, Color fadeCol, TwerpType curve);

        void Update(float dt) override;
        void Draw() override;

    private:
        Color mFadeCol;
        Vec2<float> mDir;
    };

    class ScrollTransition : public SceneTransition
    {
    public:
        ScrollTransition(std::string newScene, float time, float dir, Color bgCol, TwerpType curve);
        ~ScrollTransition();

        void Update(float dt) override;
        void Draw() override;

    private:
        Vec2<float> mDir, mCurrPos;

        SDL_Texture *mPrevTex = nullptr;
    };
};