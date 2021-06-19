#include <QCoreApplication>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <config/config.h>
using namespace std;
using namespace cfgstm;
int main()
{
#if 0
    ConfigVar<int> intConfig("interger", 5, "interger test");

    cout<<intConfig.getDescription()<<endl;
    cout<<intConfig.getName()<<endl;
    cout<<intConfig.toString()<<endl;
    intConfig.setVal(6);
    cout<<intConfig.toString()<<endl;
#endif

    vector<int> vec{1, 5, 3, 4, 5};
    ConfigVar<vector<int>> vec_int_Config("vector interger", vec, "vrctor test");
    cout<<vec_int_Config.getDescription()<<endl;
    cout<<vec_int_Config.getName()<<endl;
    cout<<vec_int_Config.toString()<<endl;
    vec_int_Config.fromSring(vec_int_Config.toString());
    cout<<vec_int_Config.toString()<<endl;
    return 0;
}
