#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <json/value.h>

struct Config
{
    // 抗锯齿时边缘检测的阈值，若值为 0 全图采样
    static double anti_aliasing_edge_threshold;

    // 抗锯齿采样数，若值为 0 不使用抗锯齿
    static int anti_aliasing_samples;

    // 使用 Fresnel 方程计算反射和折射系数
    static bool enable_fresnel;

    // 启用纹理滤波
    static bool enable_texture_filtering;

    // 高光指数
    static int hightlight_exponent;

    // 只使用光子图渲染
    static bool photon_map_only;

    // 发射光子数
    static int photon_number;

    // 采样光子数
    static int photon_samples;

    // 光子跟踪最大递归深度
    static int photon_tracing_max_depth;

    // 牛顿迭代法深度
    static int newton_iteration_depth;

    // 输出图片的刷新间隔，单位秒，若值小于等于 0 则不时时刷新
    static double output_refresh_interval;

    // 光线跟踪最大递归深度
    static int raytracing_max_depth;

    // 光线跟踪光线最小权值
    static double raytracing_min_weight;

    // 软阴影采样数
    static int soft_shadow_samples;

    // 保存为 JSON 格式
    static Json::Value toJson();

    // 从 JSON 导入场景
    static void load(const Json::Value& config);

    // 保存 JSON 到文件
    static void save(const std::string& file);

    // 从文件导入场景
    static void loadFrom(const std::string& file);

private:
    Config();
};

#endif // CONFIG_H
