#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "Spritesheet.hpp"
#include "ResourceManager.hpp"

class CharacterBase {
    protected:
        SDL_Rect mPosition{};
        int mIndex = 0;
        std::vector<Spritesheet> mSpritesheets;
        int mSpeed{};
        bool mFacingLeft = false;
        int mCharacterID;
        int mPlayerClass;
        bool mIsAttacking = false;
        float mHealthPoints{};
        float mDamageAmount{};

        // Don't need this as of now, but keeping it just in case. Will be removed in the "final" product if not required by then.
        SDL_Point mInitialPosition{};

    public:
        enum PlayerClasses {
            MartialHero,
            Wizard
        };

        enum characterIDs {
            PLAYERONE,
            PLAYERTWO
        };

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

        bool mIsInAir = false;
        float mJumpVelocity{};

        CharacterBase();
        CharacterBase(int p_speed, int p_char_ID);

        virtual ~CharacterBase() {}

        void add_spritesheet(const Spritesheet p_spritesheet);
        SDL_Rect& get_current_position();
        const int get_current_anim_index() const;
        const bool is_attacking() const;
        const bool facing_left() const;

        void set_spritesheet(int p_index);

        void move(float, const Uint8*, int, const SDL_Rect&, int, bool);
        void set_position(SDL_Rect p_position);

        void render_animation();
        void update(float, const SDL_Rect&, const Uint8*);
        void take_damage(int pDamageAmount = 4);
        const SDL_Rect& get_character_dims() const;
        void set_initial_position(SDL_Point p_pos);
        const SDL_Point& get_initial_position() const;

        virtual const std::string& get_character_class() const;
        virtual void perform_attack_one();
        virtual void perform_attack_two();
        virtual void load(ResourceManager&) = 0;
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

        MartialHero(ResourceManager& p_manager, int p_speed, int p_char_ID, bool pFacingLeft);
        
        virtual const std::string& get_character_class() const override;
        virtual void perform_attack_one() override;
        virtual void perform_attack_two() override;
        void load(ResourceManager& p_manager) override;
};

class Wizard : public CharacterBase {
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

        Wizard(ResourceManager& p_manager, int p_speed, int p_char_ID, bool pFacingLeft);
        
        virtual const std::string& get_character_class() const override;
        virtual void perform_attack_one() override;
        virtual void perform_attack_two() override;
        void load(ResourceManager& p_manager) override;
};