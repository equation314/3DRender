#include "common/config.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <json/reader.h>
#include <json/writer.h>

double Config::anti_aliasing_edge_threshold = 0.01;

int Config::anti_aliasing_edge_detection_mode = 0;

int Config::anti_aliasing_samples = 4;

int Config::depth_of_field_samples = 64;

bool Config::enable_fresnel = true;

bool Config::enable_texture_filtering = true;

int Config::hightlight_exponent = 50;

bool Config::photon_map_only = true;

int Config::photon_emitted_number = 1000000;

int Config::photon_sampling_number = 1000;

int Config::photon_tracing_max_depth = 10;

double Config::ppm_alpha = 0.7;

double Config::ppm_initial_search_radius = 1;

int Config::ppm_iteration_depth = 20;

int Config::ppm_photon_emitted_number = 100000;

int Config::newton_iteration_depth = 20;

double Config::output_refresh_interval = 0.1;

int Config::raytracing_max_depth = 20;

double Config::raytracing_min_weight = 0.05;

int Config::soft_shadow_samples = 4;

void Config::load(const Json::Value& config)
{
    anti_aliasing_edge_threshold = config["anti_aliasing_edge_threshold"].asDouble();
    anti_aliasing_edge_detection_mode = config["anti_aliasing_edge_detection_mode"].asInt();
    anti_aliasing_samples = config["anti_aliasing_samples"].asInt();
    depth_of_field_samples = config["depth_of_field_samples"].asInt();
    enable_fresnel = config["enable_fresnel"].asBool();
    enable_texture_filtering = config["enable_texture_filtering"].asBool();
    hightlight_exponent = config["hightlight_exponent"].asInt();
    newton_iteration_depth = config["newton_iteration_depth"].asInt();
    output_refresh_interval = config["output_refresh_interval"].asDouble();
    photon_map_only = config["photon_map_only"].asBool();
    photon_emitted_number = config["photon_emitted_number"].asInt();
    photon_sampling_number = config["photon_sampling_number"].asInt();
    photon_tracing_max_depth = config["photon_tracing_max_depth"].asInt();
    ppm_alpha = config["ppm_alpha"].asDouble();
    ppm_initial_search_radius = config["ppm_initial_search_radius"].asDouble();
    ppm_iteration_depth = config["ppm_iteration_depth"].asInt();
    ppm_photon_emitted_number = config["ppm_photon_emitted_number"].asInt();
    raytracing_max_depth = config["raytracing_max_depth"].asInt();
    raytracing_min_weight = config["raytracing_min_weight"].asDouble();
    soft_shadow_samples = config["soft_shadow_samples"].asInt();
}

Json::Value Config::toJson()
{
    Json::Value config;
    config["anti_aliasing_edge_threshold"] = anti_aliasing_edge_threshold;
    config["anti_aliasing_edge_detection_mode"] = anti_aliasing_edge_detection_mode;
    config["anti_aliasing_samples"] = anti_aliasing_samples;
    config["depth_of_field_samples"] = depth_of_field_samples;
    config["enable_fresnel"] = enable_fresnel;
    config["enable_texture_filtering"] = enable_texture_filtering;
    config["hightlight_exponent"] = hightlight_exponent;
    config["newton_iteration_depth"] = newton_iteration_depth;
    config["output_refresh_interval"] = output_refresh_interval;
    config["photon_map_only"] = photon_map_only;
    config["photon_emitted_number"] = photon_emitted_number;
    config["photon_sampling_number"] = photon_sampling_number;
    config["photon_tracing_max_depth"] = photon_tracing_max_depth;
    config["ppm_alpha"] = ppm_alpha;
    config["ppm_initial_search_radius"] = ppm_initial_search_radius;
    config["ppm_iteration_depth"] = ppm_iteration_depth;
    config["ppm_photon_emitted_number"] = ppm_photon_emitted_number;
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
