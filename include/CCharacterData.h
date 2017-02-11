#ifndef CCHARACTERDATA_H
#define CCHARACTERDATA_H

using namespace safe;

class CCharacterData : public Component
{
public:
    CCharacterData() :
        mName(""),
        mBaseHealth(10),
        mBaseMovement(1)
    {   mComponentName = "CharacterDataComponent"; }

    void FromLua(sol::table luaT) override {
        mName = luaT.get_or("name", mName);
        mBaseHealth = luaT.get_or("base_health", mBaseHealth);
        mBaseMovement = luaT.get_or("base_movement", mBaseMovement);
    }
    

    // Required
    std::string mName;
    int mBaseHealth;
    int mBaseMovement;
    
};


#endif /* CCHARACTERDATA_H */

