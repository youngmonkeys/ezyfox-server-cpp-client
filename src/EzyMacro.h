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

#define EZY_SINGLETON_GET_INSTANCE(className) \
public: \
    static className* getInstance() { \
        static className sInstance; \
        return &sInstance; \
    } \
private: \
    className(const className &); \
    void operator = (const className &);

