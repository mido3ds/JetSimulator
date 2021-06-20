#include <Models/Jet.hpp>
#include <App/App.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <sstream>

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

Jet::Jet() :Model(JET_MODEL_PATH) {}

void Jet::update(float dt) {
    float dYaw=0, dPitch=0, dRoll=0;

    auto& app = App::getApp();

    // rotation
    if (app.isKeyPressed(KEY_W)) {
        dPitch = +ROTATE_SPEED * dt;
    } else if (app.isKeyPressed(KEY_S)) {
        dPitch = -ROTATE_SPEED * dt;
    }
    if (app.isKeyPressed(KEY_D)) {
        dRoll = +ROTATE_SPEED * dt;
    } else if (app.isKeyPressed(KEY_A)) {
        dRoll = -ROTATE_SPEED * dt;
    }

    // translation
    if (app.isKeyPressed(KEY_UP)) {
        pos += dt * speed * front;
    } else if (app.isKeyPressed(KEY_DOWN)) {
        pos -= dt * speed * front;
    } 
    if (app.isKeyPressed(KEY_RIGHT)) {
        pos += dt * speed * right;
    } else if (app.isKeyPressed(KEY_LEFT)) {
        pos -= dt * speed * right;
    }

    // transformation
    glm::mat3 rRoll(glm::rotate(dRoll, front));
    right = rRoll * right;
    glm::mat3 rPitch(glm::rotate(dPitch, right));
    up = rPitch * rRoll * up;
    glm::mat3 rYaw(glm::rotate(dYaw, up));
    right = rYaw * right;
    front = glm::cross(up, right);
    
    rootNode->transform = glm::mat4(
        right.x, right.y, right.z, 0.0f,
        front.x, front.y, front.z, 0.0f,
        up.x,    up.y,    up.z,    0.0f,
        pos.x,   pos.y,   pos.z,   1.0f
    );

    // wrap angles
    yaw = yaw+dYaw;
    pitch = glm::wrapAngle(pitch+dPitch);
    roll = glm::wrapAngle(roll+dRoll);

    for (auto& missile: missiles) {
        missile.update(dt);
    }
}

void Jet::render(PhongShader& shader) {
    Model::render(shader);
    for (auto& missile: missiles) missile.render(shader);
}

void Jet::fireMissile() {
    if (missiles.size() < NUM_MISSILES) {
        auto missileName = string(MISSILE_NAME) + to_string(missiles.size());

        auto node = findNodeByName(missileName);
        assert(node.has_value() && "missile not found!");
        auto uniqueNode = node.value().get().disattachFromParent();

        missiles.push_back(
            Missile(
                move(uniqueNode),
                MISSILE_TIME,
                // TODO: this is just a hack, missile speed problem is not solved yet
                (MISSILE_SPD + speed) * 20
            )
        );
    }
}
