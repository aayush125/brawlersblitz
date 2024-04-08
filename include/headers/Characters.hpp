#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "Spritesheet.hpp"
#include "ResourceManager.hpp"

class CharacterBase {
    protected:
        SDL_Rect m_position{}; // scaled position
        SDL_Point m_unscaledPosition{};
        int m_index = 0;
        std::vector<Spritesheet> m_spritesheets;
        int m_speed{};
        bool m_lastPressedLeft = false;
        int m_characterID;
        bool m_isAttacking = false;

        // Don't need this as of now, but keeping it just in case. Will be removed in the "final" product if not required by then.
        SDL_Point m_initialPosition{};

    public:
        enum BasicMovements {
            WALK,
            RUN
        };

        enum characterIDs {
            PLAYERONE,
            PLAYERTWO
        };

        bool m_isInAir = false;
        float m_verticalVelocity{};

        CharacterBase(int p_speed, int p_char_ID);

        virtual ~CharacterBase() {}

        void addSpritesheet(const Spritesheet p_spritesheet);
        Spritesheet& getSpritesheet(int index);
        const SDL_Rect& getCharRect() const;
        SDL_Rect& getCurrentPosition();
        const SDL_Rect& getCharDimensions();
        const int getCurrentAnimIndex() const;

        void setSpritesheet(int p_index);

        void move(float, const Uint8*, int, const SDL_Rect&, int, bool);
        void setPosition(SDL_Rect p_position);

        virtual void playAnim() = 0;
        virtual void performAttack1() = 0;
        // virtual void update(float deltaTime) = 0;
        virtual void update(float, const SDL_Rect&) = 0;
        virtual void load(ResourceManager*) = 0;
        virtual void performAttack2();
        virtual void performAttack3();
};

class MartialHero : public CharacterBase {
    public:
        enum SpritesheetIndices {
            IDLE,
            RUN,
            JUMP,
            ATTACK1,
            ATTACK2,
            DEATH,
            FALL,
            TAKE_HIT
        };

        MartialHero(ResourceManager* p_manager, int p_speed, int p_char_ID);

        void playAnim() override;
        void performAttack1() override;
        void update(float, const SDL_Rect&) override;
        void load(ResourceManager* p_manager) override;

        void setInitialPosition(SDL_Point p_pos);

        const SDL_Rect& getCharDim() const;
        const SDL_Point& getInitPos() const;
};