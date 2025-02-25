#include "enemy.hpp"

class NormalEnemy : public Enemy {
    float followRange = 350.0f;
public:
    void Init(Game* game) override;
    void Update(float dt, const Player& player) override;
    void Render(Sprites::RenderData* data) override;
    void Destroy(Game* game) override;
    void Fire(Game* game) override;
};
