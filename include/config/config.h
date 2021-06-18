#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <stdint.h>
#include <functional>
#include <memory>
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
     * @retval 是否转化成功
     */
    virtual bool fromSring(const std::string& val) = 0;

    /**
     * @brief  获取类型的字符串名称
     * @retval 类型的字符串名称
     */
    virtual std::string getName() const = 0;

private:
    std::string m_name;                     // 配置参数的名称
    std::string m_description;              // 配置参数的描述
    //std::map<uint64_t, on_change_cb> m_cbs; // 变更回调函数组
};


}


#endif // CONFIG_H
