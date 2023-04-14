#include "GameObject.hpp"

GameObject::GameObject(Renderer &renderer) : renderer(&renderer)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
{
  this->pos = pos;
  this->size = size;
  this->color = color;
}

void GameObject::Draw()
{
  renderer->DrawQuad(pos, size, color);
}
