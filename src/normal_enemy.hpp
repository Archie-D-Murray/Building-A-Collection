#include "enemy.hpp"
#include "render_data.hpp"

class NormalEnemy : public Enemy {
    float followRange = 650.0f;
    float projectileRadius;
public:
    NormalEnemy(Game* game, Vector2 position);
    void Init(Game* game) override;
    void Update(float dt, Game* game) override;
    void Render(Sprites::RenderData* data) override;
    void Destroy(Game* game) override;
    void Fire(Game* game) override;
};
