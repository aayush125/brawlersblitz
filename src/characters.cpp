#include "Characters.hpp"
#include "Inputs.hpp"

CharacterBase::CharacterBase(int p_speed, int p_char_ID) : m_speed{p_speed}, m_characterID{p_char_ID} {}

void CharacterBase::addSpritesheet(const Spritesheet p_spritesheet) {
    m_spritesheets.push_back(p_spritesheet);
}

const SDL_Rect& CharacterBase::getCharRect() const {
    return m_position;
}

Spritesheet& CharacterBase::getSpritesheet(int index) {
    return m_spritesheets[index];
}

const SDL_Rect& CharacterBase::getCharDimensions() {
    return m_spritesheets[m_index].getSpriteDim();
}

void CharacterBase::performAttack2() {}
void CharacterBase::performAttack3() {}

void CharacterBase::setSpritesheet(int p_index) {
    if (p_index > static_cast<int>(m_spritesheets.size() - 1)) {
        std::string errorMessage = "Spritesheet index for MartialHero out of bounds. Provided " +
                                   std::to_string(p_index) +
                                   " for \"spritesheets\" size of " +
                                   std::to_string(static_cast<int>(m_spritesheets.size() - 1)) +
                                   ". Using the default spritesheet (index: 0) instead.";
        throw std::out_of_range(errorMessage);
    }

    m_index = p_index;
}

void CharacterBase::move(float p_deltaTime, const Uint8* p_keystates, int spritesheetSelectionIndex, const SDL_Rect& p_windowRect, int p_speedIncrement = 0, bool p_moveLeft = false) {

    int speed{};
    bool isMovingRight = false;
    bool isMovingLeft = false;
    SDL_Scancode leftKey;
    SDL_Scancode rightKey;
    SDL_Scancode attackKey;
    SDL_Scancode jumpKey;

    if (m_characterID == CharacterBase::characterIDs::PLAYERONE) {
        leftKey = SDL_SCANCODE_A;
        rightKey = SDL_SCANCODE_D;
        attackKey = SDL_SCANCODE_X;
        jumpKey = SDL_SCANCODE_SPACE;
    } else if (m_characterID == CharacterBase::characterIDs::PLAYERTWO) {
        leftKey = SDL_SCANCODE_LEFT;
        rightKey = SDL_SCANCODE_RIGHT;
        attackKey = SDL_SCANCODE_SLASH;
        jumpKey = SDL_SCANCODE_RETURN;
    } else {
        std::cerr << "Invalid character ID: " << m_characterID << '\n';
    }

    if (p_keystates[rightKey]) {
        // setFlip(false);
        m_lastPressedLeft = false;
        isMovingRight = true;
        speed = m_speed;
        if (p_keystates[SDL_SCANCODE_LSHIFT]) {
            speed += 300;
        }
        // std::cout << "RIGHT RIGHT RIGHT!!!\n";
    } 
    if (p_keystates[leftKey]) {
        // setFlip(true);
        m_lastPressedLeft = true;
        isMovingLeft = true;
        speed = -m_speed;
        if (p_keystates[SDL_SCANCODE_LSHIFT]) {
            speed -= 300;
        }
        // std::cout << "LEFT LEFT LEFT!!!\n";
    }
    if (p_keystates[attackKey]) {
        m_isAttacking = true;
    }

    if (p_keystates[jumpKey] && !m_isInAir) {
        // m_position.y -= 500 * p_deltaTime;
        m_verticalVelocity = -300.0f;
        m_isInAir = true;
    }

    bool isMoving = isMovingRight || isMovingLeft;

    if (isMoving) {
        try {
            setSpritesheet(MartialHero::RUN);
        } catch (const std::out_of_range& e) {
            throw;
        }
    } 
    if (m_isInAir) {
        try {
            setSpritesheet(MartialHero::JUMP);
            m_spritesheets[m_index].m_playOnce = true;
        } catch (const std::out_of_range& e) {
            throw;
        }
    } 
    if (m_isAttacking) {
        try {
            setSpritesheet(MartialHero::ATTACK1);
        } catch (const std::out_of_range& e) {
            throw;
        }
    }
    if (!isMoving && !m_isInAir && !m_isAttacking) {
        try {
            setSpritesheet(MartialHero::IDLE);
        } catch (const std::out_of_range& e) {
            throw;
        }

        return;
    }
    
    m_position.x += speed * p_deltaTime;
    m_position.y += m_verticalVelocity * p_deltaTime * 8;

    // The horizontal padding around each frame averages around 35
    if (m_position.x < -m_position.w + 35) {
        m_position.x = 1920 - 35;
    } else if (m_position.x + 35 > 1920) {
        m_position.x = -m_position.w + 35;
    }
}

MartialHero::MartialHero(ResourceManager* p_manager, int p_speed, int p_char_ID) : CharacterBase(p_speed, p_char_ID) {
    std::cout << "From ID:\t" << p_char_ID << '\n';
    load(p_manager);
}

void MartialHero::playAnim() {
    m_spritesheets[m_index].playAnim(m_position, m_lastPressedLeft);
}

void MartialHero::performAttack1() {
    std::cout << "Performed attack 1 by Martial Hero!\n";
}

void CharacterBase::setPosition(SDL_Rect p_position) {
    // if (p_position.w == 0 || p_position.h == 0) {
    //     std::cerr << "Warning: Setting width or height of the character to zero!\n";
    //     std::cerr << "\tWidth: " << p_position.w << '\n';
    //     std::cerr << "\tHeight: " << p_position.h << '\n';
    // }
    m_position = p_position;
}

const int CharacterBase::getCurrentAnimIndex() const {
    return m_index;
}

void MartialHero::setInitialPosition(SDL_Point p_pos) {
    m_initialPosition = p_pos;
    m_unscaledPosition = p_pos;
    m_position.x = p_pos.x;
    m_position.y = p_pos.y;
}

void MartialHero::update(float deltaTime, const SDL_Rect& p_windowRect) {
    // update process: get keyboard updates -> update position -> update scaling -> call spritesheet update -> ready for rendering

    const Uint8* keystates = getKeyboardState();

    if (!m_isAttacking) {
        try {
            move(deltaTime, keystates, 0, p_windowRect);
        } catch (std::out_of_range& e) {
            throw;
        }
    } else {
        if (m_spritesheets[m_index].finishedPlaying()) {
            m_isAttacking = false;
            setSpritesheet(MartialHero::IDLE);
        }
    }

    m_spritesheets[m_index].update(deltaTime);
}

void MartialHero::load(ResourceManager* p_manager) {
    addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/idle_copy.png").c_str(), 
    1, 4, 8, {0, 1, 2, 3}, true));

    addSpritesheet
    (Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/run_copy_copy.png").c_str(), 
    1, 8, 8, {0, 1, 2, 3, 4, 5, 6, 7}, true));

    addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/jump_copy.png").c_str(), 
    1, 2, 8, {0, 1}, true));

    addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/attack1_copy.png").c_str(), 
    1, 4, 12, {0, 1, 2, 3}, false));
}

const SDL_Rect& MartialHero::getCharDim() const {
    return m_spritesheets[m_index].getSpriteDim();
}

const SDL_Point& MartialHero::getInitPos() const {
    return m_initialPosition;
}

SDL_Rect& CharacterBase::getCurrentPosition() {
    return m_position;
}