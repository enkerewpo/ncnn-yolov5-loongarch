#include "assess.h"

namespace iS2ROS {

void assess(std::vector<std::shared_ptr<Capability>> capabilities,
            std::shared_ptr<EnvSimulator> env) {
  LOG_F(INFO, "[START ASSESS]");
  LOG_F(INFO, "Current environment: TIME = %s, TEMPERATURE = %f",
        env->get_datetime().c_str(), env->temperature);
  // LOG_F(INFO, "Number of capabilities to assess: %d", capabilities.size());
  for (auto cap : capabilities) {
    LOG_F(INFO, "Assessing capability [%s]", cap->name.c_str());
    // if cam is type 2, and the temperature is above 20, then it is not very
    // idea beacuse the image quality will be affected by the noise caused by
    // the heat
    // so we calculate the score of this capability based on the temp2noise
    // curve of the camera
    auto device = cap->device;
    auto noise_level = device->get_noise_level_by_temp(env->temperature);
    // the assess score is 0 to 100, the default requested threshold is 75
    // we give the score based on this noise level(0 to 100)
    // the higher the noise level, the lower the score
    // LOG_F(INFO, "Noise level of this capability: %f", noise_level);
    double score = 100 - noise_level * 100;
    LOG_F(INFO, "Assess score of this capability: %f", score);
    cap->access_score = score;
    cap->device->set_t(env->temperature);
  }
  LOG_F(INFO, "[END ASSESS]");
}

} // namespace iS2ROS