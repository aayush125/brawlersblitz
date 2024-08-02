#include "GameEngine.hpp"
#include "Utils.hpp"

GameEngine::GameEngine(ResourceManager &pManager, RenderWindow &pWindow) : mManager{pManager}, mWindow{pWindow}
{
    background = std::make_unique<Background>(mManager);
    in_game_ui = std::make_unique<UI>(mManager);
}

void GameEngine::init(int pScene)
{
    mCurrentScene = pScene;
    init_scene();
    init_physics(background->getCollider(Background::collionBoxes::GROUND).y);
    init_players();
}

void GameEngine::init_players()
{
    SDL_Rect ground = background->getCollider(Background::collionBoxes::GROUND);

    std::cout << "Player One dims: w = " << mPlayerOne->get_character_dims().w << " h = " << mPlayerOne->get_character_dims().h << '\n';
    std::cout << "Player Two dims: w = " << mPlayerTwo->get_character_dims().w << " h = " << mPlayerTwo->get_character_dims().h << '\n';

    const int PLAYER_ONE_SCALE = mPlayerOne->get_character_class() == "MartialHero" ? SPRITE_SCALE_FACTOR : WIZARD_SCALE;
    const int PLAYER_TWO_SCALE = mPlayerTwo->get_character_class() == "MartialHero" ? SPRITE_SCALE_FACTOR : WIZARD_SCALE;

    SDL_Point charOneInitialPosition = {0, (ground.y - (mPlayerOne->get_character_dims().h * PLAYER_ONE_SCALE))};
    SDL_Point charTwoInitialPosition = {1000, (ground.y - (mPlayerTwo->get_character_dims().h * PLAYER_TWO_SCALE))};

    mPlayerOne->set_initial_position(charOneInitialPosition);
    mPlayerTwo->set_initial_position(charTwoInitialPosition);

    SDL_Rect charOnePosition = {charOneInitialPosition.x,
                                charOneInitialPosition.y,
                                mPlayerOne->get_character_dims().w * PLAYER_ONE_SCALE,
                                mPlayerOne->get_character_dims().h * PLAYER_ONE_SCALE};

    SDL_Rect charTwoPosition = {charTwoInitialPosition.x,
                                charTwoInitialPosition.y,
                                mPlayerTwo->get_character_dims().w * PLAYER_TWO_SCALE,
                                mPlayerTwo->get_character_dims().h * PLAYER_TWO_SCALE};

    mPlayerOne->set_position(charOnePosition);
    mPlayerTwo->set_position(charTwoPosition);

    try
    {
        mPlayerOne->set_spritesheet(MartialHero::IDLE);
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "\nError setting spritesheet index for char one: \"" << e.what() << "\"\n";
    }

    try
    {
        mPlayerTwo->set_spritesheet(MartialHero::IDLE);
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "\nError setting spritesheet index for char two: \"" << e.what() << "\"\n";
    }

    std::cout << "Current P1 position.x: " << mPlayerOne->get_current_position().x << '\n';
    std::cout << "Current P1 position.y: " << mPlayerOne->get_current_position().y << '\n';
}

template <typename T>
void GameEngine::assign_player(int pPlayerID)
{
    if (pPlayerID == CharacterBase::characterIDs::PLAYERONE)
    {
        mPlayerOne = std::make_unique<T>(mManager, mDefaultPlayerSpeed, pPlayerID, false);
        mPlayerOneInitialized = true;
    }
    else if (pPlayerID == CharacterBase::characterIDs::PLAYERTWO)
    {
        mPlayerTwo = std::make_unique<T>(mManager, mDefaultPlayerSpeed, pPlayerID, true);
        mPlayerTwoInitialized = true;
    }
    else
    {
        throw std::runtime_error("Error assigning player. Only two players can be assigned.");
    }
}

void GameEngine::init_physics(int pGroundCoords)
{
    mPhysics.set_ground_coords(pGroundCoords);
}

bool GameEngine::can_register_attack(int pPlayerID)
{
    switch (pPlayerID)
    {
    case CharacterBase::characterIDs::PLAYERONE:
        if (mPlayerOne->facing_left())
        {
            if (mPlayerOne->get_current_position().x >= mPlayerTwo->get_current_position().x)
                return true;
        }
        else
        {
            if (mPlayerOne->get_current_position().x <= mPlayerTwo->get_current_position().x)
                return true;
        }
        break;
    case CharacterBase::characterIDs::PLAYERTWO:
        if (mPlayerTwo->facing_left())
        {
            if (mPlayerTwo->get_current_position().x >= mPlayerOne->get_current_position().x)
                return true;
        }
        else
        {
            if (mPlayerTwo->get_current_position().x <= mPlayerOne->get_current_position().x)
                return true;
        }
        break;
    default:
        std::cerr << "Invalid character ID provided. Given ID: " << pPlayerID << '\n';
        break;
    }

    return false;
}

void GameEngine::add_player(int pPlayerID, int pPlayerClass)
{
    if (mPlayerOneInitialized && mPlayerTwoInitialized)
    {
        throw std::runtime_error("Both characters already made.");
    }

    switch (pPlayerClass)
    {
    case CharacterBase::PlayerClasses::MartialHero:
        try
        {
            assign_player<MartialHero>(pPlayerID);
        }
        catch (const std::exception &e)
        {
            throw;
        }
        break;
    case CharacterBase::PlayerClasses::Wizard:
        try
        {
            assign_player<Wizard>(pPlayerID);
        }
        catch (const std::exception &e)
        {
            throw;
        }
        break;
    default:
        throw std::runtime_error("Unknown player class provided.");
        break;
    }
}

CharacterBase &GameEngine::get_player_one()
{
    return *mPlayerOne;
}

CharacterBase &GameEngine::get_player_two()
{
    return *mPlayerTwo;
}

void GameEngine::init_scene()
{
    background->init(mCurrentScene);
    in_game_ui->init_ingame_ui();
}

void GameEngine::physics_update(float pDeltaTime)
{
    mPhysics.gravity(*mPlayerOne, pDeltaTime);
    mPhysics.gravity(*mPlayerTwo, pDeltaTime);
}

void GameEngine::handle_player_attacks()
{
    static int charwidth;
    static int charwidthTwo;
    static const std::string &playerOneClass = mPlayerOne->get_character_class();
    static const std::string &playerTwoClass = mPlayerTwo->get_character_class();
    if ((mPlayerOne->get_current_anim_index() == CharacterBase::ATTACK1 || mPlayerOne->get_current_anim_index() == CharacterBase::ATTACK2) || (mPlayerTwo->get_current_anim_index() == CharacterBase::ATTACK1 || mPlayerTwo->get_current_anim_index() == CharacterBase::ATTACK2))
    {
        // charwidth = CHAR_ATTACK_WIDTH;
        // charwidthTwo = 60;
        if (mPlayerOne->is_attacking())
        {
            if (playerOneClass == "MartialHero")
            {
                charwidth = charwidthTwo = MARTIAL_ATTACK_WIDTH;
            }
            else if (playerOneClass == "Wizard")
            {
                charwidth = charwidthTwo = WIZARD_ATTACK_WIDTH;
            }
        }
        else if (mPlayerTwo->is_attacking())
        {
            if (playerTwoClass == "MartialHero")
            {
                charwidth = charwidthTwo = MARTIAL_ATTACK_WIDTH;
            }
            else if (playerTwoClass == "Wizard")
            {
                charwidth = charwidthTwo = WIZARD_ATTACK_WIDTH;
            }
        }

        // if (playerTwoClass == "MartialHero") {
        //     charwidthTwo = MARTIAL_ATTACK_WIDTH;
        // } else if (playerTwoClass == "Wizard") {
        //     charwidthTwo = WIZARD_ATTACK_WIDTH;
        // }
    }
    else
    {
        charwidth = charwidthTwo = CHAR_NORMAL_WIDTH;
    }

    bool colliding = mPhysics.collides(mPlayerOne->get_current_position(), mPlayerTwo->get_current_position(), charwidth, charwidthTwo);
    if (mPlayerOne->is_attacking() && !mPlayerTwo->is_attacking() && can_register_attack(CharacterBase::characterIDs::PLAYERONE))
    {
        if (colliding && !mPlayerOneAttackProcessed)
        {
            if (playerOneClass == "Wizard")
                mPlayerTwo->take_damage(4);
            else
                mPlayerTwo->take_damage();
            mPlayerOneAttackProcessed = true;
        }
    }
    else if (!mPlayerOne->is_attacking() && mPlayerTwo->is_attacking() && can_register_attack(CharacterBase::characterIDs::PLAYERTWO))
    {
        if (colliding && !mPlayerTwoAttackProcessed)
        {
            if (playerTwoClass == "Wizard")
                mPlayerOne->take_damage(4);
            else
                mPlayerOne->take_damage();
            mPlayerTwoAttackProcessed = true;
        }
    }
    else if (mPlayerOne->is_attacking() && mPlayerTwo->is_attacking())
    {
        if (colliding && (!mPlayerOneAttackProcessed || !mPlayerTwoAttackProcessed))
        {
            if (can_register_attack(CharacterBase::characterIDs::PLAYERONE))
            {
                if (playerTwoClass == "Wizard")
                    mPlayerOne->take_damage(4);
                else
                    mPlayerOne->take_damage();
            };
            if (can_register_attack(CharacterBase::characterIDs::PLAYERTWO))
            {
                if (playerOneClass == "Wizard")
                    mPlayerTwo->take_damage(4);
                else
                    mPlayerTwo->take_damage();
            };

            if (!mPlayerOneAttackProcessed)
                mPlayerOneAttackProcessed = true;
            if (!mPlayerTwoAttackProcessed)
                mPlayerTwoAttackProcessed = true;
        }
    }

    if (!mPlayerOne->is_attacking())
    {
        mPlayerOneAttackProcessed = false;
    }
    if (!mPlayerTwo->is_attacking())
    {
        mPlayerTwoAttackProcessed = false;
    }
}

GameEngine::RoundResult GameEngine::check_win()
{
    // bool playerOneDefeated = mPlayerOne->get_current_health() <= 0;
    // bool playerTwoDefeated = mPlayerTwo->get_current_health() <= 0;

    bool playerOneDefeated = mPlayerOne->dead();
    bool playerTwoDefeated = mPlayerTwo->dead();

    if (playerOneDefeated)
    {
        return GameEngine::RoundResult::PLAYER_TWO_WINS;
    }
    else if (playerTwoDefeated)
    {
        return GameEngine::RoundResult::PLAYER_ONE_WINS;
    }
    else if (playerOneDefeated && playerTwoDefeated)
    {
        return GameEngine::RoundResult::DRAW;
    }

    return GameEngine::RoundResult::NO_WINNER;
}

void GameEngine::reset_game_state()
{
    // Reset the positions of the players and their healths
    mPlayerOne->reset_player();
    mPlayerTwo->reset_player();
}

void GameEngine::game_over(std::unique_ptr<CharacterBase> &player)
{
    std::cout << "Calling gameOver!-------\n";
    mRoundOver = true;
    player->increment_score();
    std::cout << "Worked?\n";
    reset_game_state();
    mRoundOver = false;
}

void GameEngine::player_updates(float pDeltaTime, const Uint8 *pKeystates)
{
    // Check current health. If it's run out, play the death animation and proceed with the winning screen. [for the future: after winning screen, return to start menu]

    int winningPlayer = check_win();
    // bool roundOver = winningPlayer != GameEngine::RoundResult::NO_WINNER;

    if (winningPlayer == GameEngine::RoundResult::PLAYER_ONE_WINS && !mRoundOver)
    {
        if (mPlayerTwo->dead())
        {
            game_over(mPlayerOne);
        }
    }
    else if (winningPlayer == GameEngine::RoundResult::PLAYER_TWO_WINS && !mRoundOver)
    {
        if (mPlayerOne->dead())
        {
            game_over(mPlayerTwo);
        }
    }

    try
    {
        if (!mPlayerOne->dead())
            mPlayerOne->update(pDeltaTime, mWindow.get_window_rect(), pKeystates, mRoundOver);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error updating player one: " << e.what() << '\n';
    }

    try
    {
        if (!mPlayerTwo->dead())
            mPlayerTwo->update(pDeltaTime, mWindow.get_window_rect(), pKeystates, mRoundOver);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error updating player two: " << e.what() << '\n';
    }

    handle_player_attacks();
}

void GameEngine::update_game(float pDeltaTime)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    // Apply physics updates to all game entities
    physics_update(pDeltaTime);
    // Apply player updates
    player_updates(pDeltaTime, keystates);

    // Apply UI updates
    in_game_ui->update(mPlayerOne->get_current_health(), mPlayerTwo->get_current_health(), mPlayerOne->get_current_score(), mPlayerTwo->get_current_score(), mWindow.get_window_height(), mWindow.get_window_width());
}

void GameEngine::render_scene()
{
    background->render();

    if (mPlayerTwo->get_current_anim_index() == CharacterBase::ATTACK1)
    {
        mPlayerTwo->render_animation();
        mPlayerOne->render_animation();
    }
    else
    {
        mPlayerOne->render_animation();
        mPlayerTwo->render_animation();
    }

    in_game_ui->render_ingame_ui();
}