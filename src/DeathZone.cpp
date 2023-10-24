#include "DeathZone.hpp"

DeathZone::DeathZone(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath):
Object(*shape, position, texturePath) {
}