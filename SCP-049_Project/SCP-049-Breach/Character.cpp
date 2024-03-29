#include "Character.h"

Character::Character()
	:	health(100),
		position(0, 0),
		walkSpeed(200.0f),
		stamina(100.0f),
		m_state(Character::Walk),
		m_attackDmg(49.0f),
		m_attackSpeed(30.0f),
		m_attackCooldown(0.0f),
		m_gotHit(0),
		m_enabled(true)
{
}

Character::~Character()
{

}

bool Character::setSprite(const char* path)
{
	if (!m_texture.loadFromFile(path))
		return false;
	m_texture.setRepeated(false);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, (m_texture.getSize().y / 2.0f));
	m_sprite.setPosition(position);

	updateBoundingSize();

	return true;
}

bool Character::setSprite(sf::Texture& tex)
{
	m_texture = tex;
	m_texture.setRepeated(false);

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
	m_sprite.setOrigin(m_texture.getSize().x / 2.0f, (m_texture.getSize().y / 2.0f));
	m_sprite.setPosition(position);

	updateBoundingSize();

	return true;
}

sf::Vector2f Character::getPosition() const
{
	return position;
}

void Character::setPosition(sf::Vector2f InPos)
{
	position = InPos;
	m_sprite.setPosition(position);
}

void Character::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

float Character::getRotation() const
{
	return m_sprite.getRotation();
}

void Character::setRotation(float angle)
{
	m_sprite.setRotation(angle);
}

sf::Sprite Character::getSprite() const
{
	return m_sprite;
}

void Character::setScale(float s)
{
	m_sprite.setScale(s, s);
	updateBoundingSize();
}

void Character::move(sf::Vector2f moveV, float dt)
{
	moveV = normalize(moveV);
	direction = moveV;
	position += (moveV*getWalkSpeed(dt));
	m_sprite.setPosition(position);
}

bool Character::collide(std::vector<std::shared_ptr<ltbl::LightShape>>& list)
{
	
	for (int i = 0; i < list.size(); ++i)
	{
		if (getBoundingBox().intersects(list[i]->_shape.getGlobalBounds()))
		{
			return true;
		}
	}
	return false;
}

bool Character::collide(std::vector<Character>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if (getBoundingBox().intersects(list[i].getBoundingBox()))
		{
			return true;
		}
	}
	return false;
}

bool Character::collide(Character& c)
{
	if (getBoundingBox().intersects(c.getBoundingBox()))
	{
		return true;
	}
	return false;
}

void Character::attack(Character& c)
{
	if (m_attackCooldown < 10.0f)
	{
		m_attackCooldown += m_attackSpeed;
		c.damage(m_attackDmg);
	}
}

bool Character::damage(float dmg)
{
	if (health > 0)
	{
		health -= dmg;
		m_gotHit = 0.15f;
	}

	if (health < 0)
	{
		setState(Character::Dead);
		return false; //Dead
	}

	return true; //Alive
}

float Character::gotHit()
{
	return m_gotHit;
}

float Character::getHealth() const
{
	return health;
}

void Character::update(float dt)
{
	update_internal(dt);
}

void Character::update_internal(float)
{
}

sf::FloatRect Character::getBoundingBox()
{
	sf::Vector2f halfSize(boundingSize*0.5f, boundingSize*0.5f);
	return sf::FloatRect(position-halfSize, sf::Vector2f(boundingSize, boundingSize));
}

void Character::updateBoundingSize()
{
	float	h = m_sprite.getLocalBounds().height * m_sprite.getScale().x,
		w = m_sprite.getLocalBounds().width * m_sprite.getScale().x;

	if (h < w)
		boundingSize = h;
	else
		boundingSize = w;
}

float Character::getStamina() const
{
	return stamina;
}

float Character::getWalkSpeed(float dt)
{
	switch (m_state)
	{
	case Character::Walk:
			if (stamina < 100.0f)
				stamina += 10.0f * dt;
			return walkSpeed * dt;
		break;
	case Character::Run:
			if (stamina > 0)
			{
				stamina -= 30.0f * dt;
				return walkSpeed * 3.0f * dt;
			}
			else
				return walkSpeed * dt;
		break;
	}

	return 0;
}

sf::Vector2f Character::getDirection()
{
	return direction;
}

void Character::setState(charState newState)
{
	m_state = newState;
}

int Character::getState()
{
	return m_state;
}

float Character::getAttackDmg()
{
	return m_attackDmg;
}

void Character::enable(bool b)
{
	m_enabled = b;
}

bool Character::isEnabled()
{
	return m_enabled;
}
