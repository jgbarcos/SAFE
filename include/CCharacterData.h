#ifndef CCHARACTERDATA_H
#define CCHARACTERDATA_H

#include "SAFE/Vector2.h"
#include "SAFE/Component.h"

using namespace safe;

class CCharacterData : public Component {
public:

    CCharacterData() :
        mName(""),
        mBaseHealth(10),
        mBaseMovement(1),
        mBaseAttack(3) 
    {
        mComponentName = "CharacterDataComponent";
        mCurrentHealth = mBaseHealth;
    }

    std::string PrepareLua(sol::state_view& lua) override {
        typedef CCharacterData Comp;
        lua.new_usertype<Comp>(
                mComponentName,
                "name", &Comp::mName,
                "base_health", &Comp::mBaseHealth,
                "health", &Comp::mCurrentHealth,
                "base_movement", &Comp::mBaseMovement,
                "base_attack", &Comp::mBaseAttack
                );

        return "get_character_data";
    }

    void FromLua(sol::table luaT) override {
        mName = luaT.get_or("name", mName);
        mBaseHealth = luaT.get_or("base_health", mBaseHealth);
        mBaseMovement = luaT.get_or("base_movement", mBaseMovement);

        sol::table t = luaT.get<sol::table>("attack_area");
        if (t.valid()) {
            mAttackArea.clear();
            auto fx = [&](sol::object key, sol::object value)
            {
                mAttackArea.push_back(Vector2(value.as<sol::table>()));
            };
            t.for_each(fx);
        }

        mBaseAttack = luaT.get_or("base_attack", mBaseAttack);

        mCurrentHealth = mBaseHealth;
    }


    // Required
    std::string mName;
    int mBaseHealth;
    int mBaseMovement;

    std::vector<Vector2> mAttackArea;
    int mBaseAttack;

    // Generated
    int mCurrentHealth;

};


#endif /* CCHARACTERDATA_H */

