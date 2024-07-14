#include "Characters.hpp"
#include "UI.hpp"

UI::UI(ResourceManager& pManager) : mResManager{pManager} {}

void UI::init_ingame_ui() {
    mPlayerOneHealthBar.load_healthbar(CharacterBase::characterIDs::PLAYERONE, mResManager);
    mPlayerTwoHealthBar.load_healthbar(CharacterBase::characterIDs::PLAYERTWO, mResManager);
}

void UI::render_ingame_ui() {
    mPlayerOneHealthBar.render(mResManager);
    mPlayerTwoHealthBar.render(mResManager);
}

void UI::update(float pPlayerOneHealth, float pPlayerTwoHealth) {
    mPlayerOneHealthBar.update_health_bar(pPlayerOneHealth);
    mPlayerTwoHealthBar.update_health_bar(pPlayerTwoHealth);
}

UI::HealthBar::HealthBar() {}

void UI::HealthBar::load_healthbar(int pPlayerID, ResourceManager& pManager) {
    std::string outlineName{};
    std::string fillName{};

    if (pPlayerID == CharacterBase::characterIDs::PLAYERONE) {
        outlineName = "bluebaroutline.png";
        fillName = "bluebarfill.png";
    } else if (pPlayerID == CharacterBase::characterIDs::PLAYERTWO) {
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

    if (pPlayerID == CharacterBase::characterIDs::PLAYERONE) {
        mFillDestRect = {PLAYER_ONE_HEALTHBAR_X, HEALTHBAR_Y, mFillSrcRect.w * HEALTHBAR_SCALE_X, mFillSrcRect.h * HEALTHBAR_SCALE_Y};
    } else if (pPlayerID == CharacterBase::characterIDs::PLAYERTWO) {
        mFillDestRect = {PLAYER_TWO_HEALTHBAR_X, HEALTHBAR_Y, mFillSrcRect.w * HEALTHBAR_SCALE_X, mFillSrcRect.h * HEALTHBAR_SCALE_Y};
    }
}

void UI::HealthBar::render(ResourceManager& pManager) {
    SDL_Rect outlineDstRect = {mFillDestRect.x - 2, mFillDestRect.y - 2, (mOriginalDimensions.x * 6) + 4, (mOriginalDimensions.y * 8) + 4};

    pManager.render_ui(mHealthBarFill, &mFillSrcRect, &mFillDestRect);
    pManager.render_ui(mHealthBarOutline, NULL, &outlineDstRect);
}

void UI::HealthBar::update_health_bar(float pPlayerHealth) {
    mFillDestRect.w = mOriginalDimensions.x * (pPlayerHealth / 100.0f) * HEALTHBAR_SCALE_X;
}