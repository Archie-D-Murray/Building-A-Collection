#pragma once

#include <vector>
class Health;

class DamageReciever {
    public:
    virtual void OnDamage(Health* health) = 0;
};

class Health {
private:
    float currentHealth = 1.0f;
    float maxHealth = 1.0f;
    bool isDead = false;
    std::vector<DamageReciever*> damageReceivers;
public:

    Health(float maxHealth);

    inline float CurrentHealth() { return currentHealth; };
    inline float MaxHealth() { return maxHealth; };
    inline float PercentHealth() { return maxHealth == 0.0f ? 1.0f : currentHealth / maxHealth; };

    void Damage(float amount);
    void BindDamageReceiver(DamageReciever* receiver);
    inline bool IsDead() { return isDead; };
};
