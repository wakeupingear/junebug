#include "Utils.h"
#include "MathLib.h"

namespace junebug
{
    float Twerp(TwerpType _type, float _start, float _end, float _pos, bool _looped, float _opt1, float _opt2)
    {
        if (_type < 0 || _type >= TwerpType::TWERP_COUNT)
            return 0.0f;

        _pos = Math::Clamp(_looped ? fmod(_pos, 1.0f) : _pos, 0.0f, 1.0f);
        float _chng = _end - _start;
        float _mid = (_start + _end) / 2;

        float _b = _opt1 == Twerp_Undefined ? Twerp_Back_DefaultBounciness : _opt1;
        switch (_type)
        {
        case TWERP_LINEAR:
            return _start + (_end - _start) * _pos;

        case TWERP_INOUT_BACK:
            return (_pos < 0.5f)
                       ? Twerp(TWERP_IN_BACK, _start, _mid, _pos * 2.0f, _looped, _b, _opt2) / 2.0f
                       : Twerp(TWERP_OUT_BACK, _mid, _end, (_pos - 0.5f) * 2.0f - 1.0f, _looped, _b, _opt2);
        case TWERP_IN_BACK:
            return _chng * _pos * _pos * ((_b + 1) * _pos - _b) + _start;
        case TWERP_OUT_BACK:
            _pos--;
            return _chng * (_pos * _pos * ((_b + 1) * _pos + _b) + 1) + _start;

        default:
            return 0;
        }
    }
}