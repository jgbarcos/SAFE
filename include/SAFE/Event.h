#ifndef EVENT_H
#define EVENT_H

namespace safe {

class Event
{
public:
    typedef const char* Type;
    
    virtual ~Event(){}
    
    virtual Type type() const= 0;
    
    virtual std::string toString() const { return std::string(type()); }
};


} // namespace safe

#endif /* EVENT_H */

