#include "Game.h"

using namespace junebug;

FC_Font *Game::AddFont(std::string file, int size, int style, Color color)
{
    FC_Font *font = FC_CreateFont();
    FC_LoadFont(font, mRenderer, ("assets/fonts/" + file).c_str(), size, FC_MakeColor(255, 255, 255, 255), style);

    auto it = mFonts.find(file);
    if (it != mFonts.end() && it->second)
        FC_FreeFont(it->second);
    mFonts[file] = font;

    if (!mCurrentFont)
        SetFont(file);

    return font;
}

bool Game::RemoveFont(std::string file)
{
    auto it = mFonts.find(file);
    if (it != mFonts.end() && it->second)
    {
        FC_FreeFont(it->second);
        mFonts.erase(it);
        return true;
    }

    return false;
}

bool Game::SetFont(std::string file)
{
    auto it = mFonts.find(file);
    if (it != mFonts.end() && it->second)
    {
        mCurrentFont = it->second;
        return true;
    }

    return false;
}