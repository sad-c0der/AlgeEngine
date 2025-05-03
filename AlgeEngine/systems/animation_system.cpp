#include "animation_system.h"

void AnimationSystem::update(
    std::unordered_map<unsigned int, AnimationComponent>& animationComponents,
    float frameTime) {

    for (auto& [entity, animation] : animationComponents) {
        // Normalize frameTime to avoid large spikes
        frameTime = std::clamp(frameTime, 0.0f, 16.667f); // Clamp to ~30 FPS max

        // Update animation frame using integer math to avoid floating-point drift
        animation.frame += animation.speed * frameTime / 16.667f;

        // Wrap around the frame count
        if (animation.frame >= animation.frameCount) {
            animation.frame -= animation.frameCount;
        }
    }
}