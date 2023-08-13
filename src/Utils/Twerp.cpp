#include "Utils.h"
#include "MathLib.h"
#include "Game.h"

namespace junebug
{
    float _TwerpHelper(float _start, float _end, float _pos, TwerpType _type, bool _looped, float _opt1, float _opt2)
    {
        if (_type < 0 || _type >= TwerpType::TWERP_COUNT)
            return 0.0f;

        _pos = Clamp<float>(_looped ? fmod(_pos, 1.0f) : _pos, 0.0f, 1.0f);
        float _chng = _end - _start;
        float _mid = (_start + _end) / 2;

        float _b = _opt1 == Twerp_Undefined ? Twerp_Back_DefaultBounciness : _opt1;
        switch (_type)
        {
        case TWERP_LINEAR:
            return _start + (_end - _start) * _pos;

        case TWERP_INOUT_BACK:
            return (_pos < 0.5f)
                       ? Twerp(_start, _mid, _pos * 2.0f, TWERP_IN_BACK, _looped, _b, _opt2) / 2.0f
                       : Twerp(_mid, _end, (_pos - 0.5f) * 2.0f - 1.0f, TWERP_OUT_BACK, _looped, _b, _opt2);
        case TWERP_IN_BACK:
            return _chng * _pos * _pos * ((_b + 1) * _pos - _b) + _start;
        case TWERP_OUT_BACK:
        {
            _pos--;
            return _chng * (_pos * _pos * ((_b + 1) * _pos + _b) + 1) + _start;
        }

        case TWERP_INOUT_BOUNCE:
            return (_pos < 0.5f)
                       ? Twerp(_start, _mid, _pos * 2.0f, TWERP_IN_BOUNCE, _looped, _opt1, _opt2)
                       : Twerp(_mid, _end, (_pos - 0.5f) * 2.0f, TWERP_OUT_BOUNCE, _looped, _opt1, _opt2);
        case TWERP_IN_BOUNCE:
        {
            _chng = _end - _pos;
            _pos = 1 - _pos;
            return _chng - Twerp(_start, _end, _pos, TWERP_OUT_BOUNCE, _looped, Twerp_Bounce_DefaultBounciness) + _start;
        }
        case TWERP_OUT_BOUNCE:
        {
            if (_pos < 1.0f / 2.75f)
                return _chng * (Twerp_Bounce_DefaultBounciness * _pos * _pos) + _start;
            else if (_pos < 2.0f / 2.75f)
            {
                _pos -= 1.5f / 2.75f;
                return _chng * (Twerp_Bounce_DefaultBounciness * _pos * _pos + 3 / 4) + _start;
            }
            else if (_pos < 2.5 / 2.75)
            {
                _pos -= 2.25f / 2.75f;
                return _chng * (Twerp_Bounce_DefaultBounciness * _pos * _pos + 15 / 16) + _start;
            }

            _pos -= 2.625f / 2.75f;
            return _chng * (Twerp_Bounce_DefaultBounciness * _pos * _pos + 63 / 64) + _start;
        }

        case TWERP_INOUT_CIRCLE:
            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_CIRCLE)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_CIRCLE);
        case TWERP_OUT_CIRCLE:
        {
            _pos--;
            return _chng * sqrt(1 - _pos * _pos) + _start;
        }
        case TWERP_IN_CIRCLE:
            return -_chng * (sqrt(1 - _pos * _pos) - 1) + _start;

        case TWERP_INOUT_CUBIC:
            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_CUBIC)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_CUBIC);
        case TWERP_OUT_CUBIC:
        {
            return _chng * (pow(_pos - 1.0f, 3.0f) + 1.0f) + _start;
        }
        case TWERP_IN_CUBIC:
        {
            return _chng * pow(_pos, 3.0f) + _start;
        }

        case TWERP_INOUT_ELASTIC:
        {
            float _e = _opt1 != Twerp_Undefined ? _opt1 : 0.3f;
            float _d = _opt2 != Twerp_Undefined ? _opt2 : 5.0f;

            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_ELASTIC, _e, _d)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_ELASTIC, _e, _d);
        }
        case TWERP_OUT_ELASTIC:
        {
            float _s, _p;
            float _e = _opt1 != Twerp_Undefined ? _opt1 : 0.3f;
            float _d = _opt2 != Twerp_Undefined ? _opt2 : 5.0f;

            if (_pos == 0 || _chng == 0)
                return _start;
            if (_pos == 1)
                return _end;

            _p = _d * _e;
            _s = (Sign(_chng) == -1) ? _p * 0.25f : _p / (2 * Pi) * Asin(1);

            return _chng * pow(2.0f, -10 * _pos) * sin((_pos * _d - _s) * (2 * Pi) / _p) + _chng + _start;
        }
        case TWERP_IN_ELASTIC:
        {
            float _s, _p;
            float _e = _opt1 != Twerp_Undefined ? _opt1 : 0.3f;
            float _d = _opt2 != Twerp_Undefined ? _opt2 : 5.0f;

            if (_pos == 0 || _chng == 0)
                return _start;
            if (_pos == 1)
                return _end;

            _p = _d * _e;
            _s = Sign(_chng) == -1 ? _p * 0.25f : _p / (2 * Pi) * Asin(1);

            return -(_chng * pow(2.0f, 10 * (--_pos)) * sin((_pos * _d - _s) * (Pi * 2) / _p)) + _start;
        }

        case TWERP_INOUT_EXPO:
            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_EXPO)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_EXPO);
        case TWERP_OUT_EXPO:
            return _chng * (-pow(2.0f, -10 * _pos) + 1) + _start;
        case TWERP_IN_EXPO:
            return _chng * pow(2.0f, 10 * (_pos - 1)) + _start;

        case TWERP_INOUT_QUAD:
            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_QUAD)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_QUAD);
        case TWERP_OUT_QUAD:
            return -_chng * _pos * (_pos - 2) + _start;
        case TWERP_IN_QUAD:
            return _chng * _pos * _pos + _start;

        case TWERP_INOUT_QUART:
            return (_pos < .5) ? Twerp(_start, _mid, _pos * 2, TWERP_IN_QUART)
                               : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_QUART);
        case TWERP_OUT_QUART:
            return -_chng * (((_pos - 1) * (_pos - 1) * (_pos - 1) * (_pos - 1)) - 1) + _start;
        case TWERP_IN_QUART:
            return _chng * (_pos * _pos * _pos * _pos) + _start;

        case TWERP_INOUT_QUINT:
            return _pos < .5 ? Twerp(_start, _mid, _pos * 2, TWERP_IN_QUINT)
                             : Twerp(_mid, _end, (_pos - .5f) * 2, TWERP_OUT_QUINT);
        case TWERP_OUT_QUINT:
            return _chng * ((_pos - 1) * (_pos - 1) * (_pos - 1) * (_pos - 1) * (_pos - 1) + 1) + _start;
        case TWERP_IN_QUINT:
            return _chng * _pos * _pos * _pos * _pos * _pos + _start;

        case TWERP_INOUT_SINE:
            return _chng * 0.5f * (1 - cos(Pi * _pos)) + _start;
        case TWERP_OUT_SINE:
            return _chng * sin(_pos * Pi / 2.0f) + _start;
        case TWERP_IN_SINE:
            return _chng * (1 - cos(_pos * Pi / 2.0f)) + _start;

        default:
            return 0;
        }
    }

    float Twerp(float _start, float _end, float _pos, TwerpType _type, bool _looped, float _opt1, float _opt2)
    {
        return _TwerpHelper(_start, _end, _pos, _type, _looped, _opt1, _opt2);
    }
    int Twerp(int _start, int _end, float _pos, TwerpType _type, bool _looped, float _opt1, float _opt2)
    {
        return (int)_TwerpHelper((float)_start, (float)_end, _pos, _type, _looped, _opt1, _opt2);
    }
    Uint8 Twerp(Uint8 _start, Uint8 _end, float _pos, TwerpType _type, bool _looped, float _opt1, float _opt2)
    {
        return (Uint8)_TwerpHelper((float)_start, (float)_end, _pos, _type, _looped, _opt1, _opt2);
    }

    void TwerpAsync(Actor *actor, float &value, float start, float end, float time, TwerpType type, bool looped, float opt1, float opt2)
    {
        TwerpPropertyFloat prop = {&(value), start, end, time, 0.0f, type, looped, opt1, opt2};
        Game *game = Game::Get();
        if (game)
        {
            game->RegisterTwerpAsync(actor, prop, game->GetTwerpAsyncsFloat());
            value = start;
        }
    }
    void TwerpAsync(Actor *actor, int &value, int start, int end, float time, TwerpType type, bool looped, float opt1, float opt2)
    {
        TwerpPropertyInt prop = {&(value), start, end, time, 0.0f, type, looped, opt1, opt2};
        Game *game = Game::Get();
        if (game)
        {
            game->RegisterTwerpAsync(actor, prop, game->GetTwerpAsyncsInt());
            value = start;
        }
    }
    void TwerpAsync(Actor *actor, Uint8 &value, Uint8 start, Uint8 end, float time, TwerpType type, bool looped, float opt1, float opt2)
    {
        TwerpPropertyUint8 prop = {&(value), start, end, time, 0.0f, type, looped, opt1, opt2};
        Game *game = Game::Get();
        if (game)
        {
            game->RegisterTwerpAsync(actor, prop, game->GetTwerpAsyncsUint8());
            value = start;
        }
    }
    // Probably should make this better since it's a bit of a hack
    void TwerpAsync(Actor *actor, Vec2<float> &value, Vec2<float> start, Vec2<float> end, float time, TwerpType type, bool looped, float opt1, float opt2)
    {
        TwerpAsync(actor, value.x, start.x, end.x, time, type, looped, opt1, opt2);
        TwerpAsync(actor, value.y, start.y, end.y, time, type, looped, opt1, opt2);
    }
    void TwerpAsync(Actor *actor, Vec2<int> &value, Vec2<int> start, Vec2<int> end, float time, TwerpType type, bool looped, float opt1, float opt2)
    {
        TwerpAsync(actor, value.x, start.x, end.x, time, type, looped, opt1, opt2);
        TwerpAsync(actor, value.y, start.y, end.y, time, type, looped, opt1, opt2);
    }

    bool ToggleTwerpAsync(Actor *actor, float &value, int forceState)
    {
        TwerpPropertyFloat prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->ToggleTwerpAsync(actor, prop, game->GetTwerpAsyncsFloat(), forceState);
        return false;
    }
    bool ToggleTwerpAsync(Actor *actor, int &value, int forceState)
    {
        TwerpPropertyInt prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->ToggleTwerpAsync(actor, prop, game->GetTwerpAsyncsInt(), forceState);
        return false;
    }
    bool ToggleTwerpAsync(Actor *actor, Uint8 &value, int forceState)
    {
        TwerpPropertyUint8 prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->ToggleTwerpAsync(actor, prop, game->GetTwerpAsyncsUint8(), forceState);
        return false;
    }
    bool ToggleTwerpAsync(Actor *actor, Vec2<float> &value, int forceState)
    {
        return ToggleTwerpAsync(actor, value.x, forceState) && ToggleTwerpAsync(actor, value.y, forceState);
    }
    bool ToggleTwerpAsync(Actor *actor, Vec2<int> &value, int forceState)
    {
        return ToggleTwerpAsync(actor, value.x, forceState) && ToggleTwerpAsync(actor, value.y, forceState);
    }

    bool StopTwerpAsync(Actor *actor, float &value)
    {
        TwerpPropertyFloat prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->StopTwerpAsync(actor, prop, game->GetTwerpAsyncsFloat());
        return false;
    }
    bool StopTwerpAsync(Actor *actor, int &value)
    {
        TwerpPropertyInt prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->StopTwerpAsync(actor, prop, game->GetTwerpAsyncsInt());
        return false;
    }
    bool StopTwerpAsync(Actor *actor, Uint8 &value)
    {
        TwerpPropertyUint8 prop = {&(value)};
        Game *game = Game::Get();
        if (game)
            return game->StopTwerpAsync(actor, prop, game->GetTwerpAsyncsUint8());
        return false;
    }
    bool StopTwerpAsync(Actor *actor, Vec2<float> &value)
    {
        return StopTwerpAsync(actor, value.x) && StopTwerpAsync(actor, value.y);
    }
    bool StopTwerpAsync(Actor *actor, Vec2<int> &value)
    {
        return StopTwerpAsync(actor, value.x) && StopTwerpAsync(actor, value.y);
    }
}

using namespace junebug;

void Game::UpdateTwerps(float dt)
{
    UpdateTwerpList(mTwerpAsyncsFloat, dt);
    UpdateTwerpList(mTwerpAsyncsInt, dt);
    UpdateTwerpList(mTwerpAsyncsUint8, dt);
}