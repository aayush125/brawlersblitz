#include "Characters.hpp"
#include "UI.hpp"

UI::UI(ResourceManager &pManager) : mResManager{pManager} {}

void UI::init_ingame_ui()
{
    mPlayerOneHealthBar.load_healthbar(CharacterBase::characterIDs::PLAYERONE, mResManager);
    mPlayerTwoHealthBar.load_healthbar(CharacterBase::characterIDs::PLAYERTWO, mResManager);

    load_score_digits();
}

void UI::load_score_digits()
{
    for (int i = 0; i < 4; ++i)
    {
        mScoreDigits[i] = mResManager.load_tex((mResManager.get_base_path_from_window() + "../assets/sprites/digits/" + "Number" + std::to_string(i) + ".png").c_str());
    }

    std::cout << "Successfully loaded score digits.\n";

    mPlayerOneScore = mScoreDigits[0];
    mPlayerTwoScore = mScoreDigits[0];
}

void UI::render_ingame_ui()
{
    mPlayerOneHealthBar.render(mResManager);
    mPlayerTwoHealthBar.render(mResManager);

    render_score_digits();
}

void UI::update(float pPlayerOneHealth, float pPlayerTwoHealth, int pPlayerOneScore, int pPlayerTwoScore, int pWindowHeight, int pWindowWidth)
{
    mPlayerOneHealthBar.update_health_bar(pPlayerOneHealth);
    mPlayerTwoHealthBar.update_health_bar(pPlayerTwoHealth);

    update_score_digits(pWindowHeight, pWindowWidth, pPlayerOneScore, pPlayerTwoScore);
}

void UI::update_score_digits(int p_window_height, int p_window_width, int p_player_one_score, int p_player_two_score)
{
    mPlayerOneScore = mScoreDigits[p_player_one_score];
    mPlayerTwoScore = mScoreDigits[p_player_two_score];

    mPlayerOneScoreRect.x = PRIMARY_RES_W / 2 - SCORE_WIDTH - 140; // Offset from the center by 10
    mPlayerTwoScoreRect.x = PRIMARY_RES_W / 2 + SCORE_WIDTH + 140;
}

UI::HealthBar::HealthBar() {}

void UI::HealthBar::load_healthbar(int pPlayerID, ResourceManager &pManager)
{
    std::string outlineName{};
    std::string fillName{};

    if (pPlayerID == CharacterBase::characterIDs::PLAYERONE)
    {
        outlineName = "bluebaroutline.png";
        fillName = "bluebarfill.png";
    }
    else if (pPlayerID == CharacterBase::characterIDs::PLAYERTWO)
    {
        outlineName = "redbaroutline.png";
        fillName = "redbarfill.png";
    }

    mHealthBarOutline = pManager.load_tex((pManager.get_base_path_from_window() + "../assets/sprites/healthbars/" + outlineName).c_str());
    mHealthBarFill = pManager.load_tex((pManager.get_base_path_from_window() + "../assets/sprites/healthbars/" + fillName).c_str());

    mFillSrcRect.x = 0;
    mFillSrcRect.y = 0;
    SDL_QueryTexture(mHealthBarFill, NULL, NULL, &mFillSrcRect.w, &mFillSrcRect.h);
    mOriginalDimensions.x = mFillSrcRect.w;
    mOriginalDimensions.y = mFillSrcRect.h;

    if (pPlayerID == CharacterBase::characterIDs::PLAYERONE)
    {
        mFillDestRect = {PLAYER_ONE_HEALTHBAR_X, HEALTHBAR_Y, mFillSrcRect.w * HEALTHBAR_SCALE_X, mFillSrcRect.h * HEALTHBAR_SCALE_Y};
    }
    else if (pPlayerID == CharacterBase::characterIDs::PLAYERTWO)
    {
        mFillDestRect = {PLAYER_TWO_HEALTHBAR_X, HEALTHBAR_Y, mFillSrcRect.w * HEALTHBAR_SCALE_X, mFillSrcRect.h * HEALTHBAR_SCALE_Y};
    }
}

void UI::HealthBar::render(ResourceManager &pManager)
{
    SDL_Rect outlineDstRect = {mFillDestRect.x - 2, mFillDestRect.y - 2, (mOriginalDimensions.x * 6) + 4, (mOriginalDimensions.y * 8) + 4};

    pManager.render_ui(mHealthBarFill, &mFillSrcRect, &mFillDestRect);
    pManager.render_ui(mHealthBarOutline, NULL, &outlineDstRect);
}

void UI::render_score_digits()
{
    if (mPlayerOneScore)
    {
        mResManager.render_ui(mPlayerOneScore, nullptr, &mPlayerOneScoreRect);
    }

    if (mPlayerTwoScore)
    {
        mResManager.render_ui(mPlayerTwoScore, nullptr, &mPlayerTwoScoreRect);
    }
}

void UI::HealthBar::update_health_bar(float pPlayerHealth)
{
    mFillDestRect.w = mOriginalDimensions.x * (pPlayerHealth / 100.0f) * HEALTHBAR_SCALE_X;
}