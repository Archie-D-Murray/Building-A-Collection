#include "enemy.hpp"
#include "render_data.hpp"

class HeavyEnemy : public Enemy {
    float targetRange = 350.0f;
    Vector2 dashDir;
    float dashTime = 2.0f;
    float dashTimer = 0.0f;
public:
    HeavyEnemy(Game* game, Vector2 position);
    void Init(Game* game) override;
    void Update(float dt, Game* game) override;
    void Render(Sprites::RenderData* data) override;
    void Destroy(Game* game) override;
    void Fire(Game* game) override;
    bool CanFire(const Player& player) override;
};
