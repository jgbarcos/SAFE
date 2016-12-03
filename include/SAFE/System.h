#ifndef SYSTEM_H
#define SYSTEM_H

namespace safe {

class System
{
    public:
        virtual void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities)=0;

};

} // namespace safe

#endif // SYSTEM_H
