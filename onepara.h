/********************************************************************
    purpose: unified data structure, you can only use onepare for any data structure
			 keep your mind in your business, free of data structure
	LICENSE: Retain commercial authority yaoyoujiqing@qq.com, 
			 personal and opensource use GPL v3
*********************************************************************/
#ifndef _ONEPARA_H__
#define _ONEPARA_H__

#include <string>
#include <map>
#include <boost/any.hpp>

// here used bson for input and output from files, you can delete it from code, or you can easyly change it to other types like xml/json/Protobuf..
#include <bson/bson.h>

class OnePara
{
public:
    typedef std::map<std::string, OnePara> MMAP;
    enum ValueType
    {
        VT_INVALID,
        VT_BOOL,
        VT_FLOAT,
        VT_DOUBLE,
        VT_INT,
        VT_STRING,
        VT_INT64,
        VT_LONG,
        VT_LONGLONG,
        VT_DATE,
        VT_OID,
        VT_MYPARA,
        VT_BSONELEMENT,
        VT_USHORT, //personal added type, unsigned short
        VT_VECTOR = VT_BSONELEMENT  //internal use, using BSONElement to save raw data, can't use it outside
    };

    OnePara(){}
    OnePara(const OnePara& v);
    // BSONElement need special process
    OnePara(const bson::BSONElement& v);
    OnePara(const bson::BSONObj& v);
    OnePara(const char* v);
    
    template <typename T>
    OnePara(const T& v)
    {
        clear();
        m_value = v;
    }

    // get element
    OnePara& operator[](const std::string name);
    OnePara& operator[](const int i);

    // add element
    OnePara& operator=(const OnePara& v);
    // BSONElement need special process, Serialization also requires special
    OnePara& operator=(const bson::BSONElement& v);
    // BSONobj
    OnePara& operator=(const bson::BSONObj& v);
    // string
    OnePara& operator=(const char* v);
    
    template <typename T>
    OnePara& operator=(const T& v)
    {
        clear();
        m_value = v;
        return *this;
    }

    // OnePara add others element
    OnePara& append(const OnePara& v);

    // clear
    void clear();
    // delete
    int remove(const std::string& name);

    // get data
    boost::any toAny();
    int toInt();
    float toFloat();
    double toDouble();
    std::string& toString();
    long toLong();
    long long toLongLong();
    bool toBool();
    bson::Date_t toDate() const;
    bson::OID toOID() const;
    bson::BSONElement toBSONElement();
    unsigned short toUShort(); // add 

    //trans to string
    const std::string toString();
    // type
    const std::type_info & type() const;
    const OnePara::ValueType getType() const;

    template <typename T>
    inline const T any_cast()
    {
        if (m_value.empty())
        {
            return T(); //nothing
        }
        
        return boost::any_cast<T>(m_value);
    }

    template <typename T>
    inline const T unsafe_any_cast()
    {
        return boost::unsafe_any_cast<T>(m_value);
    }

    //get contont ref
    template <typename T>
    inline T& any_cast_this()
    {
        return boost::any_cast<T&>(m_value);
    }

    bool hasField(const std::string& name);
    bool hasField(int i);
    bool isNull();

    // follow STL style
public:
    typedef std::string key_type;
    typedef OnePara mapped_type;
    typedef OnePara referent_type;
    typedef MMAP::value_compare value_compare;
    typedef MMAP::allocator_type allocator_type;
    typedef MMAP::size_type size_type;
    typedef MMAP::difference_type difference_type;
    typedef MMAP::pointer pointer;
    typedef MMAP::const_pointer const_pointer;
    typedef MMAP::reference reference;
    typedef MMAP::const_reference const_reference;
    typedef MMAP::iterator iterator;
    typedef MMAP::const_iterator const_iterator;
    typedef MMAP::reverse_iterator reverse_iterator;
    typedef MMAP::const_reverse_iterator const_reverse_iterator;
    typedef MMAP::value_type value_type;

    iterator begin()
    {
        return m_map.begin();
    }

    iterator end()
    {
        return m_map.end();
    }

    const_iterator begin() const
    {
        return m_map.begin();
    }

    const_iterator end() const
    {
        return m_map.end();
    }

    size_type size()
    {
        return m_map.size();
    }

    iterator find(const key_type& _Keyval)
    {
        return m_map.find(_Keyval);
    }

    const_iterator find(const key_type& _Keyval) const
    {
        return m_map.find(_Keyval);
    }

    bool empty() const
    {
        return m_map.empty();
    }

public:
    // Serialization and deserialization
    bson::BSONObj& getBSON(bson::BSONObj& out);
    std::string toJSON();
    void writeJSON(std::basic_ostream<char, std::char_traits<char> >& os)
    {
        os << toJSON();
    }

    // debug
    int display(const std::string &root = "root", std::basic_ostream<char, std::char_traits<char> >& os = std::cout);
    ~OnePara();

protected:
    void setElement(const bson::BSONElement &v);
    void setValue(const OnePara &v);
    int setBSON(const bson::BSONObj& in);
    int setBSON(bson::BSONObj& in);
    void setString(const char* v);
private:
    MMAP m_map;
    boost::any m_value;
};

#endif // _ONEPARA_H__
