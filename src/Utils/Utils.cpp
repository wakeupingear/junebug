#include "Utils.h"

namespace junebug
{
    std::string PrintTime()
    {
        using namespace std::chrono;

        // get current time
        auto now = system_clock::now();

        // get number of milliseconds for the current second
        // (remainder after division into seconds)
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        // convert to std::time_t in order to convert to std::tm (broken time)
        auto timer = system_clock::to_time_t(now);

        // convert to broken time
        std::tm bt = __LocaltimeSafe__(timer);

        std::ostringstream oss;

        oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

    SDL_Texture *GetRenderTexture(Vec2<int> size, SDL_Renderer *renderer, SDL_Texture *texture, bool enableForRenderer)
    {
        int texW, texH;
        if (texture)
            SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

        if (!texture || (texW != (int)size.x || texH != (int)size.y))
        {
            if (texture)
                SDL_DestroyTexture(texture);
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.x, (int)size.y);
        }

        if (enableForRenderer)
        {
            SDL_SetRenderTarget(renderer, texture);
            SDL_RenderClear(renderer);
        }

        return texture;
    }

    bool StringEndsWith(const std::string &str, const std::string &suffix)
    {
        return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
}
