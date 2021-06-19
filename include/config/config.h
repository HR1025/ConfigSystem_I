#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <stdint.h>
#include <functional>
#include <memory>
#include <map>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
namespace cfgstm {

/**
 * @brief 配置参数虚基类
 */
class ConfigVarBase{
public:
    using ptr = std::shared_ptr<ConfigVarBase>;

    /**
     * @brief 构造函数
     * @param[in] name 配置参数名称
     * @param[in] description 配置参数描述
     */
    ConfigVarBase(const std::string name, const std::string description):
        m_name(name), m_description(description)
    {};

    /**
     * @brief  将类型转化为字符串
     * @retval 转化完的字符串
     */
    virtual std::string toString() = 0;

    /**
     * @brief 将字符串转化为类型
     */
    virtual void fromSring(const std::string& val) = 0;

    /**
     * @brief  获取类型的字符串名称
     * @retval 类型的字符串名称
     */
    virtual std::string getTypeName() const = 0;

    std::string getName() const { return m_name; }
    std::string getDescription() const { return m_description; }

private:
    std::string m_name;                     // 配置参数的名称
    std::string m_description;              // 配置参数的描述
};


/**
 * @brief 参数类型
 * @note
 */
class ConfigType{};
class BaseType : public ConfigType{};
class UnaryContainerType : public ConfigType{};
class BinaryContainerType : public ConfigType{};
class StructType : public ConfigType{};


template <typename T>
class Transfrom;


/**
 * @brief      将一元容器转化为YAML形式的字符串
 * @param[in]  container 一元容器(set,map,list,vector...)
 * @param[in]  指定采用此方法
 * @note       UnaryContainerType 类型仅仅只是用于函数的重载，
 *             此函数为类递归函数
 */
template <typename unaryContainer>
std::string toString(const unaryContainer& container,
                     UnaryContainerType){
    YAML::Node node(YAML::NodeType::Sequence);
    for(auto& elemt:container){
        node.push_back(YAML::Load(Transfrom<decltype(elemt)>()(elemt)));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}


/**
 * @brief      将YAML形式的字符串转化为一元容器
 * @param[in]  val YAML形式的字符串 一元容器(set,map,list,vector...)
 * @param[out] container YAML形式的字符串 一元容器(set,map,list,vector...)
 * @param[in]  insert 适配函数对象，实现向一元容器插入一个值
 *             function(void(unaryContainer, unaryContainer::type_value))
 * @param[in]  指定采用次充充重载方法
 * @note       UnaryContainerType 类型仅仅只是用于函数的重载，
 *             此函数为类递归函数
 */
template <typename unaryContainer, typename Func>
void fromString(const std::string& val,
                unaryContainer& container,
                Func insert,
                UnaryContainerType){
    YAML::Node node = YAML::Load(val);
    std::stringstream ss;
    for(size_t i = 0; i < node.size(); ++i) {
        ss.str("");
        ss << node[i];
        insert(container, Transfrom<typename unaryContainer::value_type>()(ss.str()));
    }
    return;
}

/**
 * @brief 泛化转化仿函数
 * @note  T 参数类型
 *        存在固有属性configtype,用于判断参数类型
 */
template <typename T>
class Transfrom{
public:
    using configtype = BaseType;

    /**
     * @brief 将类型转化为字符
     */
    std::string operator()(const T& val){
        return std::to_string(val);
    }
    /**
     * @brief 将类型转化为字符
     */
    T operator()(const std::string val){
        std::stringstream ss;
        T res;
        ss<<val;
        ss>>res;
        return res;
    }
};

/**
 * @brief 特化转化仿函数 （vector）
 * @note  T 参数类型
 *        存在固有属性configtype,用于判断参数类型
 */
template <typename T>
class Transfrom<std::vector<T>>{
public:
    using configtype = UnaryContainerType;

    /**
     * @brief 将类型转化为字符
     */
    std::string operator()(const std::vector<T>& val){
        return toString(val, configtype());
    }
    /**
     * @brief 将类型转化为字符
     */
    std::vector<T> operator()(const std::string& val){
        std::vector<T> res;
        auto insert = [](std::vector<T>& vec, T val){
            vec.push_back(val);
        };
        fromString(val, res, insert, configtype());
        return res;
    }
};

/**
 * @brief 特化转化仿函数 （list）
 * @note  T 参数类型
 *        存在固有属性configtype,用于判断参数类型
 */
template <typename T>
class Transfrom<std::list<T>>{
public:
    using configtype = UnaryContainerType;

    /**
     * @brief 将类型转化为字符
     */
    std::string operator()(const std::list<T>& val){
        return toString(val, configtype());
    }
    /**
     * @brief 将类型转化为字符
     */
    std::list<T> operator()(const std::string& val){
        std::list<T> res;
        auto insert = [](std::list<T>& vec, T val){
            vec.push_back(val);
        };
        fromString(val, res, insert, configtype());
        return res;
    }
};

/**
 * @brief 特化转化仿函数 （set）
 * @note  T 参数类型
 *        存在固有属性configtype,用于判断参数类型
 */
template <typename T>
class Transfrom<std::set<T>>{
public:
    using configtype = UnaryContainerType;

    /**
     * @brief 将类型转化为字符
     */
    std::string operator()(const std::set<T> val){
        return toString(val, configtype());
    }
    /**
     * @brief 将类型转化为字符
     */
    std::set<T> operator()(const std::string val){
        std::set<T> res;
        auto insert = [](std::set<T>& vec, T& val){
            vec.insert(val);
        };
        fromString(val, res, insert, configtype());
        return res;
    }
};

/**
 * @brief 特化转化仿函数 （unordered_set）
 * @note  T 参数类型
 *        存在固有属性configtype,用于判断参数类型
 */
template <typename T>
class Transfrom<std::unordered_set<T>>{
public:
    using configtype = UnaryContainerType;

    /**
     * @brief 将类型转化为字符
     */
    std::string operator()(const std::unordered_set<T>& val){
        return toString(val, configtype());
    }
    /**
     * @brief 将类型转化为字符
     */
    std::unordered_set <T> operator()(const std::string& val){
        std::unordered_set<T> res;
        auto insert = [](std::unordered_set<T>& vec, T& val){
            vec.insert(val);
        };
        fromString(val, res, insert, configtype());
        return res;
    }
};

/**
 * @brief  配置参数虚子类
 * @param  T 配置参数的类型
 * @param  transformT 仿函数，实现 string 与 T 的双向转换
 *         function(string(T));
 *         function(T(string));
 * @note   主要使用偏特化实现多态
 */
template <typename T, class transformT = Transfrom<T>>
class ConfigVar : public ConfigVarBase{
public:
    using ptr = std::shared_ptr<ConfigVar>;
    using on_change_cb = std::function<void(const T& oldValue, const T& newValue)>;

    /**
     * @brief      构造函数
     * @param[in]  name 配置参数名称
     * @param[in]  val  配置参数值
     * @param[in]  description 配置参数描述
     */
    ConfigVar(std::string name,
              T val,
              std::string description = ""):
        ConfigVarBase(name, description), m_val(val)
    {}

    /**
     * @brief  将类型转化为字符串
     * @retval 转化完的字符串
     */
    std::string toString() override{
        return transformT()(m_val);
    }
    /**
     * @brief 将字符串转化为类型
     */
    void fromSring(const std::string& val) override{
        m_val = transformT()(val);
        return;
    }

    /**
     * @brief 设置配置参数
     * @note  若成功配置参数，则会触发回调函数组
     */
    void setVal(const T& val){
        if(val != getVal()){
            for(auto& cb:m_cbs){
                cb.second(m_val, val);
            }
            m_val = val;
        }
    }
    T getVal() const { return m_val; }

    /**
     * @brief      添加回调函数
     * @param[in]  cb 回调函数
     * @return     回调函数对应的funcId
     */
    uint64_t addListener(on_change_cb cb){
        static uint64_t funcId = 0;
        m_cbs.insert(std::make_pair<uint64_t, on_change_cb>(++funcId, cb));
        return funcId;
    }
    /**
     * @brief      删除回调函数
     * @param[in]  funcId 与回调函数对应的funcId
     * @return     若不存在则不干任何事情
     */
    void delListener(const uint64_t funcId){
        auto itr = m_cbs.find(funcId);
        if(itr != m_cbs.end()){
            m_cbs.erase(itr);
        }
    }
    /**
     * @brief      获取回调函数
     * @param[in]  funcId 与回调函数对应的funcId
     * @return     若不存在则返回nullptr
     */
    on_change_cb getListener(const uint64_t funcId){
        auto itr = m_cbs.find(funcId);
        return itr==m_cbs.end() ? nullptr : itr->second;
    }
    /**
     * @brief      清空回调函数
     */
    void clearListener(){
        m_cbs.clear();
    }

    /**
     * @brief  获取类型的字符串名称
     * @retval 类型的字符串名称
     */
    std::string getTypeName() const override{
        return typeid(m_val).name();
    }

private:
    T m_val;                                // 数据
    std::map<uint64_t, on_change_cb> m_cbs; // 变更回调函数组
};

/**
 * @brief  配置参数管理类
 * @note   主要使用偏特化实现多态
 */
class Config{
public:
    using ConfigVarMap = std::unordered_map<std::string,
                                            ConfigVarBase::ptr>;

    /**
     * @brief 查找/创建配置参数
     * @param[in] name 配置参数名称
     * @param[in] default_value 参数默认值
     * @param[in] description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                                             const T& default_value,
                                             const std::string& description);


    /**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,则返回nulllptr
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name);

    /**
     * @brief     使用YAML::Node初始化配置模块
     * @param[in] root YAML根节点
     */
    static void LoadFromYaml(const YAML::Node& root);

    /**
     * @brief 加载path文件夹里面的配置文件
     * param[in] 文件路径
     */
    static void LoadFromConfDir(const std::string& path, bool force = false);

    /**
     * @brief 查找配置参数,返回配置参数的基类
     * @param[in] name 配置参数名称
     */
    static ConfigVarBase::ptr LookupBase(const std::string& name);

    /**
     * @brief 遍历所有配置参数
     * param[in] cb 回调函数
     */
    static void Visit(std::function<void(ConfigVarBase::ptr)> cb);

private:
    /**
     * @brief 返回所有的配置项
     */
    static ConfigVarMap& GetDatas() {
        return s_datas;
    }

    static ConfigVarMap s_datas;  // 配置参数信息组
};

}


#endif // CONFIG_H
