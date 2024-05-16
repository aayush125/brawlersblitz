#include <SDL2/SDL.h>
#include <memory>

#include "Background.hpp"
#include "Characters.hpp"
#include "Physics.hpp"
#include "ResourceManager.hpp"
#include "UI.hpp"

class GameEngine {
    public:
        GameEngine(ResourceManager& pManager, RenderWindow& pWindow);

        void add_player(int pPlayerID, int pPlayerClass);
        void init(int pScene);
        void init_scene();
        void init_physics(int pGroundCoords);
        void init_players();
        void update_game(float pDeltaTime);
        void render_scene();
        // void take_hit(CharacterBase& pCharacterOne, CharacterBase& pCharacterTwo);

        CharacterBase& get_player_one();
        CharacterBase& get_player_two();
        bool can_register_attack(int pPlayerID);
    
    private:
        bool mPlayerOneInitialized = false;
        bool mPlayerTwoInitialized = false;
        bool mPlayerOneAttackProcessed = false;
        bool mPlayerTwoAttackProcessed = false;
        int mDefaultPlayerSpeed = 380;
        int mCurrentScene{};
        std::unique_ptr<Background> background = nullptr;

        std::unique_ptr<UI> in_game_ui = nullptr;

        ResourceManager& mManager;
        RenderWindow& mWindow;

        std::unique_ptr<CharacterBase> mPlayerOne;
        std::unique_ptr<CharacterBase> mPlayerTwo;
        Physics mPhysics;

        template <typename T>
        void assign_player(int pPlayerID);

        void physics_update(float pDeltaTime);
        void player_updates(float pDeltaTime, const Uint8* pKeystates);
        void handle_player_attacks();
};