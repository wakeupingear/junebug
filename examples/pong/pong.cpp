#define SDL_MAIN_HANDLED

#include <junebug.h>
#include <components.h>
using namespace junebug;

class Ball : public PhysicalActor
{
public:
    Ball(Vec2<float> pos) : PhysicalActor(pos, "koopa")
    {
        GetSprite()->SetOrigin(SpriteOrigin::Center);
        SetScale(0.075f);

        auto func = [this]()
        {
            if (GetPosition().x < Game::Get()->GetScreenWidth() / 2)
                points.y++;
            else
                points.x++;
            Reset();
            ShakeCamera(Vec2(5, 5), 0.2f);
        };
        auto comp = new OffscreenComponent(this, true, func);
        comp->SetPadding(46.0f, 46.0f);
        SetBounce(1.0f);
    }

    void FirstUpdate(float dt)
    {
        Reset();
    }

    void Update(float dt)
    {
        time += dt;
        if (time > 4.0f)
        {
            SetVelocity(GetVelocity() * 1.4f);
            time = 0.0f;
        }
    }

    void Reset()
    {
        Vec2 dir = Random::GetDirection(0.0f, 90.0f, -45.0f) * Random::GetSign();
        SetVelocity(dir * 200);
        SetPosition(GetStartPosition());
        time = 0.0f;

        print("Points:", points);
    }

    float time = 0.0f;
    Vec2<int> points{0, 0};
};

class Paddle : public PhysicalActor
{
public:
    Paddle(Vec2<float> pos, int player) : PhysicalActor(pos, "koopa"), mPlayer(player)
    {
        SetOrigin(SpriteOrigin::Center);
        SetScale(0.07f, 0.2f);
    }

    void Update(float dt)
    {
        MovePosition(Vec2(0.0f, InputsDir("up", "down", mPlayer) * 200 * dt));
        ClampPosition(GetActorSize() / 2, Vec2<float>(Game::Get()->GetSceneSize()) - GetActorSize() / 2);
    }

    int mPlayer = 0;
};

class Wall : public PhysicalActor
{
public:
    Wall(Vec2<float> pos) : PhysicalActor(pos, "koopa")
    {
        SetVisible(false);
        SetScale(Game::Get()->GetSceneSize().x / GetSpriteSize().x, 0.1f);
    }
};

class Pong : public Game
{
public:
    void LoadData()
    {
        new Ball(Vec2<float>(GetScreenWidth() / 2, GetScreenHeight() / 2));

        new Paddle(Vec2<float>(20, GetScreenHeight() / 2), 0);
        new Paddle(Vec2<float>(GetScreenWidth() - 20, GetScreenHeight() / 2), 1);

        new Wall(Vec2<float>(GetScreenWidth() / 2, -16.0f));
        new Wall(Vec2<float>(GetScreenWidth() / 2, GetScreenHeight() + 16.0f));

        SetInputMapping("up", {KEY_W});
        SetInputMapping("down", {KEY_S});

        SetInputMapping("up", {KEY_UP}, 1);
        SetInputMapping("down", {KEY_DOWN}, 1);
    }
};

int main(int argc, char *argv[])
{
    Pong pong;
    pong.Options().title = "Pong";
    pong.isDebug = false;
    return pong.Run(384, 216);
}