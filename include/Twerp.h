#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

namespace junebug
{
#define Twerp_Undefined -127.0f
#define Twerp_Back_DefaultBounciness 1.5f
#define Twerp_Bounce_DefaultBounciness 7.5625f
    enum TwerpType
    {
        TWERP_LINEAR,
        TWERP_INOUT_BACK,
        TWERP_IN_BACK,
        TWERP_OUT_BACK,
        TWERP_INOUT_BOUNCE,
        TWERP_OUT_BOUNCE,
        TWERP_IN_BOUNCE,
        TWERP_INOUT_CIRCLE,
        TWERP_OUT_CIRCLE,
        TWERP_IN_CIRCLE,
        TWERP_INOUT_CUBIC,
        TWERP_OUT_CUBIC,
        TWERP_IN_CUBIC,
        TWERP_INOUT_ELASTIC,
        TWERP_OUT_ELASTIC,
        TWERP_IN_ELASTIC,
        TWERP_INOUT_EXPO,
        TWERP_OUT_EXPO,
        TWERP_IN_EXPO,
        TWERP_INOUT_QUAD,
        TWERP_OUT_QUAD,
        TWERP_IN_QUAD,
        TWERP_INOUT_QUART,
        TWERP_OUT_QUART,
        TWERP_IN_QUART,
        TWERP_INOUT_QUINT,
        TWERP_OUT_QUINT,
        TWERP_IN_QUINT,
        TWERP_INOUT_SINE,
        TWERP_OUT_SINE,
        TWERP_IN_SINE,
        TWERP_COUNT
    };

    // Fancy lerp function with various curve types.
    // Credit to PixelatedPope
    /// @param start The start value
    /// @param end The end value
    /// @param pos The position on the curve
    /// @param type The type of curve to use
    /// @param looped OPTIONAL Whether or not the curve is looped
    /// @return The value at the given position on the curve
    float Twerp(float _start, float _end, float _pos, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined);
    int Twerp(int _start, int _end, float _pos, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined);
    Uint8 Twerp(Uint8 _start, Uint8 _end, float _pos, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined);

    struct TwerpPropertyBase
    {
        float time;
        float timeElapsed;
        TwerpType type;
        bool looped;
        float opt1;
        float opt2;

        TwerpPropertyBase() : time(0.0f), timeElapsed(0.0f), type(TWERP_LINEAR), looped(false), opt1(Twerp_Undefined), opt2(Twerp_Undefined) {}

        TwerpPropertyBase(float _time, float _timeElapsed, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined)
            : time(_time), timeElapsed(_timeElapsed), type(_type), looped(_looped), opt1(_opt1), opt2(_opt2) {}
    };

    struct TwerpPropertyFloat : public TwerpPropertyBase
    {
        float start;
        float end;
        float *value;

        TwerpPropertyFloat(float *value) : value(value) {}

        TwerpPropertyFloat(float *_value, float _start, float _end, float _time, float _timeElapsed, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined)
            : TwerpPropertyBase(_time, _timeElapsed, _type, _looped, _opt1, _opt2), start(_start), end(_end), value(_value) {}
    };
    struct TwerpPropertyInt : public TwerpPropertyBase
    {
        int start;
        int end;
        int *value;

        TwerpPropertyInt(int *value) : value(value) {}

        TwerpPropertyInt(int *_value, int _start, int _end, float _time, float _timeElapsed, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined)
            : TwerpPropertyBase(_time, _timeElapsed, _type, _looped, _opt1, _opt2), start(_start), end(_end), value(_value) {}
    };
    struct TwerpPropertyUint8 : public TwerpPropertyBase
    {
        Uint8 start;
        Uint8 end;
        Uint8 *value;

        TwerpPropertyUint8(Uint8 *value) : value(value) {}

        TwerpPropertyUint8(Uint8 *_value, Uint8 _start, Uint8 _end, float _time, float _timeElapsed, TwerpType _type = TWERP_LINEAR, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined)
            : TwerpPropertyBase(_time, _timeElapsed, _type, _looped, _opt1, _opt2), start(_start), end(_end), value(_value) {}
    };

    // Twerp (lerp) function, but make it a coroutine!
    /// @param actor The actor to twerp (use `this` in a class)
    /// @param value A reference to the value to twerp
    /// @param start The start value
    /// @param end The end value
    /// @param time The time to twerp across in seconds
    /// @param type The type of curve to use
    /// @param looped OPTIONAL Whether or not the curve is looped
    /// @return The coroutine ID, or -1 if the coroutine could not be created
    void TwerpAsync(class PureActor *actor, float &value, float start, float end, float time, TwerpType type = TWERP_LINEAR, bool looped = false, float opt1 = Twerp_Undefined, float opt2 = Twerp_Undefined);
    void TwerpAsync(class PureActor *actor, int &value, int start, int end, float time, TwerpType type = TWERP_LINEAR, bool looped = false, float opt1 = Twerp_Undefined, float opt2 = Twerp_Undefined);
    void TwerpAsync(class PureActor *actor, Uint8 &value, Uint8 start, Uint8 end, float time, TwerpType type = TWERP_LINEAR, bool looped = false, float opt1 = Twerp_Undefined, float opt2 = Twerp_Undefined);

    bool ToggleTwerpAsync(class PureActor *actor, float &value, int forceState = -1);
    bool ToggleTwerpAsync(class PureActor *actor, int &value, int forceState = -1);
    bool ToggleTwerpAsync(class PureActor *actor, Uint8 &value, int forceState = -1);

    bool StopTwerpAsync(class PureActor *actor, float &value);
    bool StopTwerpAsync(class PureActor *actor, int &value);
    bool StopTwerpAsync(class PureActor *actor, Uint8 &value);
};