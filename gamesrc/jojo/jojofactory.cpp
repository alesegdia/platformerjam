#include "jojofactory.h"
#include "jojoconfig.h"

namespace jojo {

JojoFactory::JojoFactory(secs::Engine &world, int playerIndex)
    : BaseEntityFactory(world),
      m_playerIndex(playerIndex)
{
    aether::graphics::AsepriteAnimationLoader animloader;
    m_playerAnim = animloader.load("assets/jojo/bicho.json");
    m_ballEnemyAnim = animloader.load("assets/jojo/enemyball.json");
}

secs::Entity JojoFactory::makePlayer(float x, float y)
{
    secs::Entity player = world().processor().addEntity();

    addBasicTilemapEntity(player, x, y, 20, 16);

    auto& render_comp = addComponent<RenderComponent>(player);
    render_comp.render_offset.set(0, 0); //-(256)/2.0f, -256/2.0f);
    render_comp.renderOrder = m_playerIndex;

    auto& animation_comp = addComponent<AnimationComponent>(player);
    animation_comp.animation = m_playerAnim.anims["stand"].get();
    render_comp.texture = *m_playerAnim.sheet->getFrame(0);

    addComponent<PlayerComponent>(player);
    
    auto& aic = addComponent<AgentInputComponent>(player);
    aic.horizontal_speed = jojo::JojoConfig::instance().playerSpeed;
    aic.jump_force = jojo::JojoConfig::instance().playerJumpForce;
    
    addComponent<JumperAgentComponent>(player);
    auto& atrc = addComponent<AnimatorComponent>(player);
    atrc.groundStandAnimation = m_playerAnim.anims["stand"].get();
    atrc.groundWalkAnimation = m_playerAnim.anims["walk"].get();
    atrc.airAnimation = m_playerAnim.anims["jump"].get();

    auto& gc = addComponent<GravityComponent>(player);
    gc.gravityFactor = jojo::JojoConfig::instance().playerGravityFactor;
    gc.fallingVelocityCap = jojo::JojoConfig::instance().playerFallingCap;

    return player;
}

secs::Entity JojoFactory::makeBallEnemy(float x, float y)
{
    auto entity = world().processor().addEntity();

    addBasicTilemapEntity(entity, x, y, 20, 16);

    auto& render_comp = addComponent<RenderComponent>(entity);
    render_comp.render_offset.set(0, 0);
    render_comp.renderOrder = m_playerIndex;

    auto& animation_comp = addComponent<AnimationComponent>(entity);
    animation_comp.animation = m_ballEnemyAnim.anims["walk"].get();
    render_comp.texture = *m_ballEnemyAnim.sheet->getFrame(0);

    auto& aic = addComponent<AgentInputComponent>(entity);
    aic.horizontal_speed = 0.5f;
    aic.x_axis = 1;

    auto& atrc = addComponent<AnimatorComponent>(entity);
    atrc.groundStandAnimation = m_ballEnemyAnim.anims["walk"].get();
    atrc.groundWalkAnimation = m_ballEnemyAnim.anims["walk"].get();
    atrc.airAnimation = m_ballEnemyAnim.anims["walk"].get();

    addComponent<AIAgentDumbWalkerComponent>(entity);

    return entity;

}


}
