#include "common/config.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/writer.h>

double Config::anti_aliasing_edge_threshold = 0.01;

int Config::anti_aliasing_samples = 4;

bool Config::enable_fresnel = true;

bool Config::enable_texture_filtering = true;

int Config::hightlight_exponent = 50;

int Config::newton_iteration_depth = 20;

double Config::output_refresh_interval = 1;

int Config::raytracing_max_depth = 20;

double Config::raytracing_min_weight = 0.05;

int Config::soft_shadow_samples = 4;

void Config::load(const Json::Value& config)
{
    anti_aliasing_edge_threshold = config["anti_aliasing_edge_threshold"].asDouble();
    anti_aliasing_samples = config["anti_aliasing_samples"].asInt();
    enable_fresnel = config["enable_fresnel"].asBool();
    enable_texture_filtering = config["enable_texture_filtering"].asBool();
    hightlight_exponent = config["hightlight_exponent"].asInt();
    newton_iteration_depth = config["newton_iteration_depth"].asInt();
    output_refresh_interval = config["output_refresh_interval"].asDouble();
    raytracing_max_depth = config["raytracing_max_depth"].asInt();
    raytracing_min_weight = config["raytracing_min_weight"].asDouble();
    soft_shadow_samples = config["soft_shadow_samples"].asInt();
}

Json::Value Config::toJson()
{
    Json::Value config;
    config["anti_aliasing_edge_threshold"] = anti_aliasing_edge_threshold;
    config["anti_aliasing_samples"] = anti_aliasing_samples;
    config["enable_fresnel"] = enable_fresnel;
    config["enable_texture_filtering"] = enable_texture_filtering;
    config["hightlight_exponent"] = hightlight_exponent;
    config["newton_iteration_depth"] = newton_iteration_depth;
    config["output_refresh_interval"] = output_refresh_interval;
    config["raytracing_max_depth"] = raytracing_max_depth;
    config["raytracing_min_weight"] = raytracing_min_weight;
    config["soft_shadow_samples"] = soft_shadow_samples;
    return config;
}

void Config::save(const std::string& file)
{
    std::ofstream fout(file.c_str());
    fout << toJson() << std::endl;
    fout.close();
}

void Config::loadFrom(const std::string& file)
{
    Json::Value config;
    std::ifstream fin(file.c_str());
    if (!fin)
    {
        std::cerr << "ERROR: No such config file '" + file + "'" << std::endl;
        return;
    }
    fin >> config;
    fin.close();

    load(config);
}
