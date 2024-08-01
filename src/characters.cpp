#include "Characters.hpp"
#include "Inputs.hpp"

CharacterBase::CharacterBase() {}

CharacterBase::CharacterBase(int p_speed, int p_char_ID) : mSpeed{p_speed}, mCharacterID{p_char_ID} {}

void CharacterBase::add_spritesheet(const Spritesheet p_spritesheet)
{
    mSpritesheets.push_back(p_spritesheet);
}

void CharacterBase::set_spritesheet(int p_index)
{
    if (p_index > static_cast<int>(mSpritesheets.size() - 1))
    {
        std::string errorMessage = "Spritesheet index for MartialHero out of bounds. Provided " +
                                   std::to_string(p_index) +
                                   " for \"spritesheets\" size of " +
                                   std::to_string(static_cast<int>(mSpritesheets.size() - 1)) +
                                   ". Using the default spritesheet (index: 0) instead.";
        throw std::out_of_range(errorMessage);
    }

    mIndex = p_index;
}

const bool CharacterBase::is_attacking() const
{
    return mIsAttacking;
}

const bool CharacterBase::facing_left() const
{
    return mFacingLeft;
}

void CharacterBase::move(float p_deltaTime, const Uint8 *p_keystates, int spritesheetSelectionIndex, const SDL_Rect &p_windowRect, int p_speedIncrement = 0, bool p_moveLeft = false)
{

    int speed{};
    bool isMovingRight = false;
    bool isMovingLeft = false;
    static bool performingAttackOne = false;
    static bool performingAttackTwo = false;
    SDL_Scancode leftKey;
    SDL_Scancode rightKey;
    SDL_Scancode attackOneKey;
    SDL_Scancode attackTwoKey;
    SDL_Scancode jumpKey;
    SDL_Scancode sprintKey;

    if (!mIsAttacking)
    {
        performingAttackOne = false;
        performingAttackTwo = false;
    }

    if (mCharacterID == CharacterBase::characterIDs::PLAYERONE)
    {
        leftKey = SDL_SCANCODE_A;
        rightKey = SDL_SCANCODE_D;
        attackOneKey = SDL_SCANCODE_Z;
        attackTwoKey = SDL_SCANCODE_X;
        jumpKey = SDL_SCANCODE_SPACE;
        sprintKey = SDL_SCANCODE_LSHIFT;
    }
    else if (mCharacterID == CharacterBase::characterIDs::PLAYERTWO)
    {
        leftKey = SDL_SCANCODE_LEFT;
        rightKey = SDL_SCANCODE_RIGHT;
        attackOneKey = SDL_SCANCODE_PERIOD;
        attackTwoKey = SDL_SCANCODE_SLASH;
        jumpKey = SDL_SCANCODE_RETURN;
        sprintKey = SDL_SCANCODE_RSHIFT;
    }
    else
    {
        std::cerr << "Invalid character ID: " << mCharacterID << '\n';
    }

    if (p_keystates[rightKey])
    {
        mFacingLeft = false;
        isMovingRight = true;
        speed = mSpeed;
        if (p_keystates[sprintKey])
        {
            speed += 300;
        }
    }
    if (p_keystates[leftKey])
    {
        mFacingLeft = true;
        isMovingLeft = true;
        speed = -mSpeed;
        if (p_keystates[sprintKey])
        {
            speed -= 300;
        }
    }
    if (p_keystates[attackOneKey])
    {
        mIsAttacking = true;
        performingAttackOne = true;
    }
    else if (p_keystates[attackTwoKey])
    {
        mIsAttacking = true;
        performingAttackTwo = true;
    }

    if (p_keystates[jumpKey] && !mIsInAir)
    {
        mJumpVelocity = -300.0f;
        mIsInAir = true;
    }

    if (mCharacterID == CharacterBase::characterIDs::PLAYERONE && p_keystates[SDL_SCANCODE_Q])
    {
        try
        {
            set_spritesheet(MartialHero::DEATH);
        }
        catch (const std::out_of_range &e)
        {
            throw;
        }
        mDeathAnimPlaying = true;

        return;
    }

    bool isMoving = isMovingRight || isMovingLeft;

    if (!isMoving && !mIsInAir && !mIsAttacking)
    {
        try
        {
            set_spritesheet(MartialHero::IDLE);
        }
        catch (const std::out_of_range &e)
        {
            throw;
        }

        return;
    }

    if (isMoving)
    {
        try
        {
            set_spritesheet(MartialHero::RUN);
        }
        catch (const std::out_of_range &e)
        {
            throw;
        }
    }

    if (mIsInAir)
    {
        try
        {
            set_spritesheet(MartialHero::JUMP);
        }
        catch (const std::out_of_range &e)
        {
            throw;
        }
    }
    if (mIsAttacking)
    {
        if (performingAttackOne)
            perform_attack_one();
        else if (performingAttackTwo)
            perform_attack_two();
    }

    mPosition.x += speed * p_deltaTime;

    // The horizontal padding around each frame averages around 200
    if (mPosition.x < -mPosition.w + 200)
    {
        mPosition.x = 1920 - 200;
    }
    else if (mPosition.x + 200 > 1920)
    {
        mPosition.x = -mPosition.w + 200;
    }
}

void CharacterBase::render_animation()
{
    mSpritesheets[mIndex].play_spritesheet(mPosition, mFacingLeft);
}

void CharacterBase::set_position(SDL_Rect p_position)
{
    mPosition = p_position;
}

void CharacterBase::reset_player()
{
    mPosition.x = mInitialPosition.x;
    mPosition.y = mInitialPosition.y;

    mHealthPoints = 100;

    set_spritesheet(IDLE);

    mDead = false;
    mDeathAnimPlaying = false;
    mIsAttacking = false;

    if (mCharacterID == PLAYERONE)
        mFacingLeft = false;
}

const int CharacterBase::get_current_anim_index() const
{
    return mIndex;
}

bool CharacterBase::dead()
{
    return mDead;
}

void CharacterBase::set_initial_position(SDL_Point p_pos)
{
    mInitialPosition = p_pos;
    mPosition.x = p_pos.x;
    mPosition.y = p_pos.y;
}

void CharacterBase::update(float deltaTime, const SDL_Rect &p_windowRect, const Uint8 *pKeystates, bool gameOver)
{
    // update process: get keyboard updates -> update position -> update scaling -> call spritesheet update -> ready for rendering

    if (mDead)
    {
        return;
    }

    if (gameOver)
    {
        if (get_current_anim_index() != CharacterBase::IDLE)
            set_spritesheet(CharacterBase::IDLE);
        mSpritesheets[mIndex].update(deltaTime);
        return;
    }

    if (mHealthPoints <= 0 && !mDeathAnimPlaying)
    {
        try
        {
            set_spritesheet(CharacterBase::DEATH);
            mDeathAnimPlaying = true;
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }

    if (!mIsAttacking && !mDeathAnimPlaying)
    {
        try
        {
            move(deltaTime, pKeystates, 0, p_windowRect);
        }
        catch (std::out_of_range &e)
        {
            throw;
        }
    }
    else
    {
        if (mSpritesheets[mIndex].finished_playing())
        {
            if (mDeathAnimPlaying)
            {
                mDead = true;
                return;
            }
            else
            {
                if (mIsAttacking)
                    mIsAttacking = false;
                set_spritesheet(MartialHero::IDLE);
            }
        }
    }
    mPosition.y += mJumpVelocity * deltaTime * 8;
    mSpritesheets[mIndex].update(deltaTime);
}

const SDL_Rect &CharacterBase::get_character_dims() const
{
    return mSpritesheets[mIndex].get_sprite_dims();
}

const SDL_Point &CharacterBase::get_initial_position() const
{
    return mInitialPosition;
}

SDL_Rect &CharacterBase::get_current_position()
{
    return mPosition;
}

void CharacterBase::take_damage(int pDamageAmount)
{
    if (mHealthPoints >= pDamageAmount)
        mHealthPoints -= pDamageAmount;
    else if (mHealthPoints >= 0)
        mHealthPoints -= mHealthPoints;
}

const float CharacterBase::get_current_health() const
{
    return mHealthPoints;
}

const int CharacterBase::get_current_score() const
{
    return mScore;
}

void CharacterBase::increment_score()
{
    mScore++;
}

const std::string &CharacterBase::get_character_class() const
{
    static const std::string &baseClass = "CharacterBase";
    return baseClass;
}

void CharacterBase::perform_attack_one()
{
    std::cerr << "Base class attack one called\n";
}

void CharacterBase::perform_attack_two()
{
    std::cerr << "Base class attack two called\n";
}

MartialHero::MartialHero(ResourceManager &p_manager, int p_speed, int p_char_ID, bool pFacingLeft) : CharacterBase(p_speed, p_char_ID)
{
    mHealthPoints = 100;
    mDamageAmount = 4;
    mFacingLeft = pFacingLeft;
    load(p_manager);
}

const std::string &MartialHero::get_character_class() const
{
    static const std::string &martialHeroClass = "MartialHero";
    return martialHeroClass;
}

void MartialHero::perform_attack_one()
{
    try
    {
        set_spritesheet(MartialHero::ATTACK1);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}

void MartialHero::perform_attack_two()
{
    try
    {
        set_spritesheet(MartialHero::ATTACK2);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}

void MartialHero::load(ResourceManager &p_manager)
{
    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/idle_copy.png").c_str(),
                                1, 4, 8, {0, 1, 2, 3}, true, false));

    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/run_copy_copy.png").c_str(),
                                1, 8, 8, {0, 1, 2, 3, 4, 5, 6, 7}, true, false));

    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/jump_copy.png").c_str(),
                                1, 2, 8, {0, 1}, true, false));

    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/attack1_copy.png").c_str(),
                                1, 4, 12, {0, 1, 2, 3}, false, false));

    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/Attack2.png").c_str(),
                                1, 4, 12, {0, 1, 2, 3}, false, false));

    add_spritesheet(Spritesheet(p_manager,
                                (p_manager.get_base_path_from_window() + "../assets/sprites/martial_hero/death_copy.png").c_str(),
                                1, 7, 6, {0, 1, 2, 3, 4, 5, 6}, false, true));
}

Wizard::Wizard(ResourceManager &p_manager, int p_speed, int p_char_ID, bool pFacingLeft) : CharacterBase(p_speed, p_char_ID)
{
    mHealthPoints = 100;
    mDamageAmount = 4;
    mFacingLeft = pFacingLeft;
    load(p_manager);
}

const std::string &Wizard::get_character_class() const
{
    static const std::string &huntressClass = "Wizard";
    return huntressClass;
}

void Wizard::perform_attack_one()
{
    try
    {
        set_spritesheet(Wizard::ATTACK1);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}

void Wizard::perform_attack_two()
{
    try
    {
        set_spritesheet(Wizard::ATTACK2);
    }
    catch (const std::exception &e)
    {
        throw;
    }
}

void Wizard::load(ResourceManager &pManager)
{
    add_spritesheet(Spritesheet(pManager,
                                (pManager.get_base_path_from_window() + "../assets/sprites/huntress/Idle.png").c_str(),
                                1, 6, 10, {0, 1, 2, 3, 4, 5}, true, false));

    add_spritesheet(Spritesheet(pManager,
                                (pManager.get_base_path_from_window() + "../assets/sprites/huntress/Run.png").c_str(),
                                1, 8, 10, {0, 1, 2, 3, 4, 5, 6, 7}, true, false));

    add_spritesheet(Spritesheet(pManager,
                                (pManager.get_base_path_from_window() + "../assets/sprites/huntress/Jump.png").c_str(),
                                1, 2, 10, {0, 1}, true, false));

    add_spritesheet(Spritesheet(pManager,
                                (pManager.get_base_path_from_window() + "../assets/sprites/huntress/Attack1.png").c_str(),
                                1, 8, 14, {0, 1, 2, 3, 4, 5, 6, 7}, false, false));

    add_spritesheet(Spritesheet(pManager,
                                (pManager.get_base_path_from_window() + "../assets/sprites/huntress/Attack2.png").c_str(),
                                1, 8, 14, {0, 1, 2, 3, 4, 5, 6, 7}, false, false));
}
