#include "player.h"
#include "../level/level.h"

Player::Player(glm::vec3 position) {
    this->position = position;
    this->position.y = -this->position.y;
    shader = Shader::BLOCK;
}

void Player::update() {
    if (Input::key_pressed(SDL_SCANCODE_W)) {
        position.x -= sin(Math::to_radians(rotation.y));
        position.z += cos(Math::to_radians(rotation.y));
    }
    if (Input::key_pressed(SDL_SCANCODE_S)) {
        position.x += sin(Math::to_radians(rotation.y));
        position.z -= cos(Math::to_radians(rotation.y));
    }
    if (Input::key_pressed(SDL_SCANCODE_A)) {
        position.x -= sin(Math::to_radians(rotation.y - 90.0f));
        position.z += cos(Math::to_radians(rotation.y - 90.0f));
    }
    if (Input::key_pressed(SDL_SCANCODE_D)) {
        position.x -= sin(Math::to_radians(rotation.y + 90.0f));
        position.z += cos(Math::to_radians(rotation.y + 90.0f));
    }
    position.y -= dy;
    if (position.y > -48.0f) position.y = -48.0f;
    if (jumping) {
        dy += 0.8f;
    }
    if (dy > JUMP_HEIGHT) {
        jumping = false;
    }
    if (!jumping && dy > -JUMP_HEIGHT) dy -= 0.2f;
    if (Input::key_pressed(SDL_SCANCODE_SPACE) && !jumping && position.y == -48.0f) {
        jumping = true;
    }
    if (Input::key_pressed(SDL_SCANCODE_F)) {
        light = true;
    } else if (Input::key_pressed(SDL_SCANCODE_G)) {
        light = false;
    }
    if (Input::mouse_grabbed()) {
        rotation.x -= Input::get_DY() * mouse_sensitivity;
        rotation.y -= Input::get_DX() * mouse_sensitivity;
    }
    if (rotation.x < -90.0f) rotation.x = -90.0f;
    if (rotation.x >  90.0f) rotation.x =  90.0f;
    
    unsigned short& id = level->get_intersecting_block(this);
    if (id != 0) id = 0;
    
//    glm::vec4 ray_clip = glm::vec4(Input::get_DX(), 0.0f, 1.0f, 1.0f);
//    glm::vec4 ray_eye = glm::inverse(Shader::pr_matsrix) * ray_clip;
//    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, 1.0f, 0.0f);
//    std::cout << "Ray Eye:" << ray_eye.x << std::endl;
//    
//    glm::vec4 ray = glm::normalize(glm::inverse(Shader::pr_matrix) * ray_eye);
//    level->get_block(glm::vec3(ray.x, ray.y, ray.z));
}

void Player::render() {
    shader->enable();
    glm::mat4 vw_matrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    vw_matrix *= glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    vw_matrix *= glm::translate(glm::mat4(1.0f), position);
    shader->set_uniform_matrix("vw_matrix", vw_matrix);
    shader->set_uniform_float3("player_pos", -position);
    if (light) shader->set_uniform_float1("light_on", 1);
    else shader->set_uniform_float1("light_on", 0);
    shader->disable();
}
