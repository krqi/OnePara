/********************************************************************
	purpose: unified data structure, you can only use onepare for any data structure
			 keep your mind in your business, free of data structure
	LICENSE: Retain commercial authority yaoyoujiqing@qq.com,
			 personal and opensource use GPL v3
*********************************************************************/
#include <stdio.h>
#include <vector>
#include <sstream>
#include "onepara.h"

OnePara& OnePara::operator[](const std::string name)
{
    // process "." style
    std::string::size_type i = name.find_first_of('.');
    if (std::string::npos == i) // last one
    {
        return m_map[name];
    }
    else
    {
        std::string ls = name.substr(0, i);
        std::string rs = name.substr(i + 1, name.size() - i - 1);
        return (*this)[ls][rs];
    }
}

OnePara& OnePara::operator[](const int i)
{
    char name[32] = {0};
    sprintf(name, "%d", i);
    return (*this)[name];
}

OnePara& OnePara::operator=(const OnePara& v)
{
    setValue(v);

    return *this;
}

OnePara& OnePara::operator=(const bson::BSONElement& v)
{
    setElement(v);

    return *this;
}

OnePara& OnePara::operator=(const bson::BSONObj& v)
{
    setBSON(v);
    return *this;
}

OnePara& OnePara::operator=(const char* v)
{
    setString(v);
    return *this;
}

OnePara::OnePara(const OnePara& v)
{
    setValue(v);
}

OnePara::OnePara(const bson::BSONElement& v)
{
    setElement(v);
}

OnePara::OnePara(const bson::BSONObj& v)
{
    setBSON(v);
}

OnePara::OnePara(const char* v)
{
    setString(v);
}

void OnePara::clear()
{
    m_value = boost::any();
    m_map.clear();
}

int OnePara::remove(const std::string& name)
{
    return m_map.erase(name);
}

OnePara::~OnePara()
{
}

const std::type_info & OnePara::type() const
{
    return m_map.empty() ? m_value.type() : typeid(OnePara);
}

boost::any OnePara::toAny()
{
    return m_value;
}

int OnePara::toInt()
{
    return boost::any_cast<int>(m_value);
}

unsigned short OnePara::toUShort()
{
    return boost::any_cast<unsigned short>(m_value);
}

float OnePara::toFloat()
{
    return boost::any_cast<float>(m_value);
}

double OnePara::toDouble()
{
    return boost::any_cast<double>(m_value);
}

std::string& OnePara::toString()
{
    return boost::any_cast<std::string&>(m_value);
}

long OnePara::toLong()
{
    return boost::any_cast<long>(m_value);
}

long long OnePara::toLongLong()
{
    return boost::any_cast<long long>(m_value);
}

bool OnePara::toBool()
{
    return boost::any_cast<bool>(m_value);
}

bson::BSONElement OnePara::toBSONElement()
{
    std::vector<char> vv = boost::any_cast<std::vector<char> >(m_value);

    return bson::BSONElement(&vv[0]);
}

void OnePara::setValue(const OnePara &v)
{
    clear();
    m_value = v.m_value;

    for (MMAP::const_iterator it = v.m_map.begin(); it != v.m_map.end(); ++it)
    {
        m_map[it->first] = it->second;
    }
}

void OnePara::setElement(const bson::BSONElement &v)
{
    clear();

    m_value = std::vector<char>(v.rawdata(), v.rawdata() + v.size());
}

const OnePara::ValueType OnePara::getType() const
{
    ValueType t = VT_INVALID;

    if (!m_map.empty())
    {
        t = VT_MYPARA;
    }
    else if (typeid(std::string) == m_value.type() || typeid(const std::string) == m_value.type())
    {
        t = VT_STRING;
    }
    else if(typeid(int) == m_value.type() || typeid(const int) == m_value.type())
    {
        t = VT_INT;
    }
    else if(typeid(bool) == m_value.type() || typeid(const bool) == m_value.type())
    {
        t = VT_BOOL;
    }
    else if(typeid(std::vector<char>) == m_value.type())
    {
        t = VT_BSONELEMENT; // Binary store as element
    }
    else if (typeid(long) == m_value.type() || typeid(const long) == m_value.type())
    {
        t = VT_LONG;
    }
    else if (typeid(long long) == m_value.type() || typeid(const long long) == m_value.type())
    {
        t = VT_LONGLONG;
    }
    else if (typeid(float) == m_value.type() || typeid(const float) == m_value.type())
    {
        t = VT_FLOAT;
    }
    else if (typeid(double) == m_value.type() || typeid(const double) == m_value.type())
    {
        t = VT_DOUBLE;
    }
    else if (typeid(const bson::Date_t) == m_value.type() || typeid(bson::Date_t) == m_value.type())
    {
        t = VT_DATE;
    }
    else if (typeid(const bson::OID) == m_value.type() || typeid(bson::OID) == m_value.type())
    {
        t = VT_OID;
    }
    else if (typeid(const unsigned short) == m_value.type() || typeid(unsigned short) == m_value.type())
    {
        t = VT_USHORT;
    }
    else
    {
        t = VT_INVALID; // TODO write log
    }

    return t;
}

bson::Date_t OnePara::toDate() const
{
    return boost::any_cast<const bson::Date_t>(m_value);
}

bson::OID OnePara::toOID() const
{
    return boost::any_cast<const bson::OID>(m_value);
}

int OnePara::setBSON(const bson::BSONObj& in)
{
    bson::BSONObj o = in;
    return setBSON(o);
}

void OnePara::setString(const char* v)
{
    clear();
    m_value = std::string(v);
}

int OnePara::setBSON(bson::BSONObj& in)
{
    clear();

    int iErrCode = 0;
    for (bson::BSONObj::iterator it = in.begin(); it.more(); )
    {
        bson::BSONElement e = it.next();

        int iType = 0;
        iType = e.type();

        switch (iType)
        {
        case bson::EOO:
            break;
        case bson::Undefined:
        case bson::jstNULL:
        case bson::MaxKey:
        case bson::MinKey:
        case bson::BinData:
        case bson::RegEx:
            // ´æÎªBSONElement
            m_map[e.fieldName()] = e;
            break;
        case bson::Bool:
            m_map[e.fieldName()] = e.boolean();
            break;
        case bson::NumberInt:
            m_map[e.fieldName()] = e._numberInt();
            break;
        case bson::Date:
        case bson::Timestamp:
            m_map[e.fieldName()] = e.date();
            break;
        case bson::NumberDouble:
            m_map[e.fieldName()] = e._numberDouble();
        case bson::NumberLong:
            m_map[e.fieldName()] = e._numberLong();
            break;
        case bson::jstOID:
            m_map[e.fieldName()] = e.__oid();
            break;
        case bson::Symbol:
        case bson::Code:
        case bson::String:
            m_map[e.fieldName()] = e.String();
            break;
        case bson::CodeWScope: // not support
            m_map[e.fieldName()] = std::string("CodeWScope not support!");
            break;
        case bson::DBRef: // not support
            m_map[e.fieldName()] = std::string("DBRef not support!");
            break;
        case bson::Object:
            m_map[e.fieldName()] = e.Obj();
            break;
        case bson::Array: //special
            m_map[std::string(e.fieldName()) + "_Arr-ay_"] = e.Obj();
            break;

        default: // should not
            // TODO log
            break;
        }
    }

    return iErrCode;
}

bson::BSONObj& OnePara::getBSON(bson::BSONObj& out)
{
    bson::BSONObjBuilder ob;

    if (m_map.empty())
    {
        return out = ob.obj();
    }

    static bool isArry = false;

    for (MMAP::iterator it = m_map.begin(); it != m_map.end(); ++it)
    {
        ValueType t = it->second.getType();
        isArry = false;

        switch (t)
        {
        case VT_FLOAT:
            ob.append(it->first, it->second.toFloat());
            isArry = false;
            break;
        case VT_DOUBLE:
            ob.append(it->first, it->second.toDouble());
            isArry = false;
            break;
        case VT_INT:
            ob.append(it->first, it->second.toInt());
            isArry = false;
            break;
        case VT_STRING:
            ob.append(it->first, it->second.toString());
            isArry = false;
            break;
        case VT_INT64:
        case VT_LONGLONG:
            ob.append(it->first, it->second.toLongLong());
            isArry = false;
            break;
        case VT_LONG:
            ob.append(it->first, (long long)it->second.toLong());
            isArry = false;
            break;
        case VT_DATE:
            ob.append(it->first, it->second.toDate());
            isArry = false;
            break;
        case VT_BOOL:
            ob.append(it->first, it->second.toBool());
            isArry = false;
            break;
        case VT_OID:
            ob.append(it->first, it->second.toOID());
            isArry = false;
            break;
        case VT_MYPARA:
            {
                bson::BSONObj o;
                std::string strName = it->first;
                
                bson::BSONObj& subobj = it->second.getBSON(o);
                if (!strName.find("_Arr-ay_")) // Arry
                {
                    if (isArry)
                    {
                        ob.appendArray(strName.substr(8), subobj);
                    } 
                    else
                    {
                        ob.append(strName.substr(8), subobj);
                    }
                    isArry = true;
                } 
                else
                {
                    if (isArry)
                    {
                        ob.appendArray(strName, subobj);
                    } 
                    else
                    {
                        ob.append(strName, subobj);
                    }
                    isArry = false;
                }

                break;
            }
        case VT_BSONELEMENT:
            ob.append(bson::BSONElement(&(boost::any_cast<std::vector<char> >(it->second.toAny()))[0]));
            isArry = false;
            break;
        case VT_INVALID:
            isArry = false;
            break;
        default: 
            break;
        }
    }

    return out = ob.obj();
}

int OnePara::display(const std::string &root, std::basic_ostream<char, std::char_traits<char> >& os)
{
    int iErrCode = 0;

    if (!m_value.empty())
    {
        ValueType t = getType();
        switch (t)
        {
        case VT_FLOAT:
            os << root << " = " << toFloat() << "(float)" << std::endl;
            break;
        case VT_DOUBLE:
            os << root << " = " << toDouble() << "(double)" << std::endl;
            break;
        case VT_INT:
            os << root << " = " << toInt() << "(int)" << std::endl;
            break;
        case VT_STRING:
            os << root << " = " << toString() << "(string)" << std::endl;
            break;
        case VT_INT64:
        case VT_LONGLONG:
            os << root << " = " << toLongLong() << "(longlong)" << std::endl;
            break;
        case VT_LONG:
            os << root << " = " << toLong() << "(long)" << std::endl;
            break;
        case VT_DATE:
            os << root << " = " << toDate() << "(date)" << std::endl;
            break;
        case VT_BOOL:
            os << root << " = " << (toBool()?"true":"false") << "(bool)" << std::endl;
            break;
        case VT_OID:
            os << root << " = " << toOID() << "(bson::OID)" << std::endl;
            break;
        case VT_MYPARA: 
            break;
        case VT_BSONELEMENT:
            os << root << " = " << bson::BSONElement(&(boost::any_cast<std::vector<char> >(toAny()))[0]).toString() << "(bson::BSONElement)" << std::endl;
            break;
        case VT_USHORT:
            os << root << " = " << toUShort() << "(unsigned short)" << std::endl;
            break;

        case VT_INVALID:
            os << root << " = (unknown type," << m_value.type().name() << ")" << std::endl;
            break;
        default:  
            break;
        }
    }

    for (MMAP::iterator it = m_map.begin(); it != m_map.end(); ++it)
    {
        it->second.display(root + "." + it->first);
    }

    return iErrCode;
}

bool OnePara::isNull()
{
    return (m_value.empty() && m_map.empty());
}

bool OnePara::hasField(const std::string& name)
{
    return !(m_map.find(name) == m_map.end());
}

bool OnePara::hasField(int i)
{
    char name[32] = {0};
    sprintf(name, "%d", i);
    return hasField(name);
}

OnePara& OnePara::append(const OnePara& v)
{
    for (MMAP::const_iterator it = v.m_map.begin(); it != v.m_map.end(); ++it)
    {
        if (!hasField(it->first))
        {
            m_map[it->first] = it->second;
        }
    }

    return *this;
}

const std::string OnePara::toString()
{
    int iErrCode = 0;
    std::ostringstream os;
    if (!m_value.empty())
    {
        ValueType t = getType();
        switch (t)
        {
        case VT_FLOAT:
            os << toFloat();
            break;
        case VT_DOUBLE:
            os << toDouble();
            break;
        case VT_INT:
            os << toInt();
            break;
        case VT_STRING:
            os << toString();
            break;
        case VT_INT64:
        case VT_LONGLONG:
            os << toLongLong();
            break;
        case VT_LONG:
            os << toLong();
            break;
        case VT_DATE:
            os << toDate();
            break;
        case VT_BOOL:
            os << (toBool()?"true":"false");
            break;
        case VT_OID:
            os << toOID();
            break;
        case VT_MYPARA: 
            break;
        case VT_BSONELEMENT:
            os << bson::BSONElement(&(boost::any_cast<std::vector<char> >(toAny()))[0]).toString();
            break;

        case VT_INVALID:
            break;
        default:
            break;
        }
    }

    return os.str();
}

std::string OnePara::toJSON()
{
    bson::BSONObj bo;
    getBSON(bo);
    return bo.jsonString(bson::TenGen);
}
