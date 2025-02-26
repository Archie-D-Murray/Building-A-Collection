#include "enemy.hpp"

class HeavyEnemy : public Enemy {
    Vector2 targetPos;
    float targetRange;
public:
    HeavyEnemy(Game* game, Vector2 position);
    void Init(Game* game) override;
    void Update(float dt, Player& player) override;
    void Render(Sprites::RenderData* data) override;
    void Destroy(Game* game) override;
    void Fire(Game* game) override;
    bool CanFire(const Player& player) override;
};
