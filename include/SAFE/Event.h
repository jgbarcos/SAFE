#ifndef EVENT_H
#define EVENT_H

namespace safe {

class Event {
public:
    typedef std::string Type;

    virtual ~Event() { }

    virtual Type type() const = 0;

    virtual std::string toString() const {
        return std::string(type());
    }
    
    bool mIsLuaEvent = false;
};

} // namespace safe

#endif /* EVENT_H */

