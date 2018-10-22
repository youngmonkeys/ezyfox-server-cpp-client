#pragma once

#define EZY_NAMESPACE com::tvd12::ezyfoxserver::client

#define EZY_NAMESPACE_START \
namespace com {\
namespace tvd12 {\
namespace ezyfoxserver {\
namespace client {\

#define EZY_NAMESPACE_END \
}\
}\
}\
}


#define EZY_NAMESPACE_START_WITH(package) \
using namespace com::tvd12::ezyfoxserver::client; \
namespace com {\
namespace tvd12 {\
namespace ezyfoxserver {\
namespace client {\
namespace package {

#define EZY_NAMESPACE_START_WITH_ONLY(package) \
namespace com {\
namespace tvd12 {\
namespace ezyfoxserver {\
namespace client {\
namespace package {

#define EZY_NAMESPACE_END_WITH \
}\
}\
}\
}\
}

//=========================================
#define EZY_PROPERTY_READONLY(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual varType get##varName(void) const;

//=========================================
#define EZY_PROPERTY_READONLY_PASS_BY_REF(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual const varType& get##varName(void) const;

//=========================================
#define EZY_PROPERTY(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual varType get##varName(void);\
public:\
    virtual void set##varName(varType value);

//=========================================
#define EZY_PROPERTY_PASS_BY_REF(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual const varType& get##varName(void) const;\
public:\
    virtual void set##varName(const varType& value);

//=========================================
#define EZY_SYNTHESIZE_READONLY(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual varType get##varName(void) const { return m##varName; }

#define EZY_SYNTHESIZE_WRITEONLY(varType, varName)\
protected:\
varType m##varName;\
public:\
virtual void set##varName(varType value) { m##varName = value; }

//=========================================
#define EZY_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual const varType& get##varName(void) const { return m##varName; }

//=========================================
#define EZY_SYNTHESIZE(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual varType get##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(varType value){ m##varName = value; }

//=========================================
#define EZY_SYNTHESIZE_GET(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual varType get##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(varType value);

//=========================================
#define EZY_SYNTHESIZE_SET(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual void set##varName(varType value){ m##varName = value; }

//=========================================
#define EZY_SYNTHESIZE_PASS_BY_REF(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual const varType& get##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(const varType& value){ m##varName = value; }

//=========================================
#define EZY_SYNTHESIZE_PASS_BY_REF_GET(varType, varName)\
protected:\
    varType m##varName;\
public:\
    virtual const varType& get##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(const varType& value);

//=========================================
#define EZY_SYNTHESIZE_BOOL(varName)\
protected:\
    bool m##varName;\
public:\
    virtual bool is##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(bool value){ m##varName = value; }

//=========================================
#define EZY_SYNTHESIZE_BOOL_GET(varName)\
protected:\
    bool mIs##varName;\
public:\
    virtual bool is##varName(void) const { return m##varName; }\
public:\
    virtual void set##varName(bool value);

//=========================================
#define EZY_SYNTHESIZE_BOOL_READONLY(varName)\
protected:\
    bool m##varName;\
public:\
    virtual bool is##varName(void) const { return m##varName; }

#define EZY_SAFE_DELETE(p)           do { if(p) {delete (p); (p) = nullptr; } } while(0);
#define EZY_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0);
#define EZY_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0);
#define EZY_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0);
#define EZY_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0);
#define EZY_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0);
#define EZY_BREAK_IF(cond)           if(cond) break
#define EZY_SAFE_DELETE_VECTOR(vector) \
for(int i = 0 ; i < vector.size() ; i++) {\
    auto p = vector[i];\
    do { if(p) {delete (p); (p) = nullptr; } } while(0);\
}\
vector.clear();

#define EZY_SAFE_RELEASE_VECTOR(vector) \
for(int i = 0 ; i < vector.size() ; i++) {\
    auto p = vector[i];\
    do { if(p) { (p)->release(); } } while(0);\
}\
vector.clear();

#define EZY_SINGLETON_GET_INSTANCE(className) \
public: \
    static className* getInstance() { \
        static className sInstance; \
        return &sInstance; \
    } \
private: \
    className(); \
public: \
    className(className const&) = delete; \
    void operator = (className const&) = delete;

#define EZY_FOREACH_MAP(map)\
for(auto it = map.begin() ; it != map.end() ; ++it)
