#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <json/value.h>

struct Config
{
    // 抗锯齿时边缘检测的阈值，若值为 0 全图采样
    static double anti_aliasing_edge_threshold;

    // 抗锯齿边缘识别模式，0 为根据光线跟踪过程自动识别，1 为根据颜色差识别，其他值为全图采样
    static int anti_aliasing_edge_detection_mode;

    // 抗锯齿采样数，若值为 0 不使用抗锯齿
    static int anti_aliasing_samples;

    // 景深采样数，若值为 0 不使用景深
    static int depth_of_field_samples;

    // 使用 Fresnel 方程计算反射和折射系数
    static bool enable_fresnel;

    // 启用纹理滤波
    static bool enable_texture_filtering;

    // 高光指数
    static int hightlight_exponent;

    // 牛顿迭代法深度
    static int newton_iteration_depth;

    // 输出图片的刷新间隔，单位秒，若值小于等于 0 则不时时刷新
    static double output_refresh_interval;

    // 只使用光子图渲染
    static bool photon_map_only;

    // 发射光子数
    static int photon_emitted_number;

    // 采样光子数
    static int photon_sampling_number;

    // 光子跟踪最大递归深度
    static int photon_tracing_max_depth;

    // PPM 每轮迭代的衰减因子
    static double ppm_alpha;

    // PPM 初始搜索半径
    static double ppm_initial_search_radius;

    // PPM 迭代深度
    static int ppm_iteration_depth;

    // PPM 每轮发射光子数
    static int ppm_photon_emitted_number;

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
