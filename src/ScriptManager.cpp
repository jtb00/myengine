#include "Engine.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ECS.h"
#include "ScriptManager.h"
#include "SoundManager.h"

ScriptManager::ScriptManager() : lua() {}

bool ScriptManager::start()
{
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
	lua.new_enum<Key>("KEYBOARD", {
									  {"SPACE", Key::KEY_SPACE},
									  {"APOSTROPHE", Key::KEY_APOSTROPHE},
									  {"COMMA", Key::KEY_COMMA},
									  {"MINUS", Key::KEY_MINUS},
									  {"PERIOD", Key::KEY_PERIOD},
									  {"SLASH", Key::KEY_SLASH},
									  {"0", Key::KEY_0},
									  {"1", Key::KEY_1},
									  {"2", Key::KEY_2},
									  {"3", Key::KEY_3},
									  {"4", Key::KEY_4},
									  {"5", Key::KEY_5},
									  {"6", Key::KEY_6},
									  {"7", Key::KEY_7},
									  {"8", Key::KEY_8},
									  {"9", Key::KEY_9},
									  {"SEMICOLON", Key::KEY_SEMICOLON},
									  {"EQUAL", Key::KEY_EQUAL},
									  {"A", Key::KEY_A},
									  {"B", Key::KEY_B},
									  {"C", Key::KEY_C},
									  {"D", Key::KEY_D},
									  {"E", Key::KEY_E},
									  {"F", Key::KEY_F},
									  {"G", Key::KEY_G},
									  {"H", Key::KEY_H},
									  {"I", Key::KEY_I},
									  {"J", Key::KEY_J},
									  {"K", Key::KEY_K},
									  {"L", Key::KEY_L},
									  {"M", Key::KEY_M},
									  {"N", Key::KEY_N},
									  {"O", Key::KEY_O},
									  {"P", Key::KEY_P},
									  {"Q", Key::KEY_Q},
									  {"R", Key::KEY_R},
									  {"S", Key::KEY_S},
									  {"T", Key::KEY_T},
									  {"U", Key::KEY_U},
									  {"V", Key::KEY_V},
									  {"W", Key::KEY_W},
									  {"X", Key::KEY_X},
									  {"Y", Key::KEY_Y},
									  {"Z", Key::KEY_Z},
									  {"LEFT_BRACKET", Key::KEY_LEFT_BRACKET},
									  {"BACKSLASH", Key::KEY_BACKSLASH},
									  {"RIGHT_BRACKET", Key::KEY_RIGHT_BRACKET},
									  {"GRAVE", Key::KEY_GRAVE},
									  {"ESCAPE", Key::KEY_ESCAPE},
									  {"ENTER", Key::KEY_ENTER},
									  {"TAB", Key::KEY_TAB},
									  {"BACKSPACE", Key::KEY_BACKSPACE},
									  {"INSERT", Key::KEY_INSERT},
									  {"DELETE", Key::KEY_DELETE},
									  {"RIGHT", Key::KEY_RIGHT},
									  {"LEFT", Key::KEY_LEFT},
									  {"DOWN", Key::KEY_DOWN},
									  {"UP", Key::KEY_UP},
									  {"PAGE_UP", Key::KEY_PAGE_UP},
									  {"PAGE_DOWN", Key::KEY_PAGE_DOWN},
									  {"HOME", Key::KEY_HOME},
									  {"END", Key::KEY_END},
									  {"CAPS_LOCK", Key::KEY_CAPS_LOCK},
									  {"SCROLL_LOCK", Key::KEY_SCROLL_LOCK},
									  {"NUM_LOCK", Key::KEY_NUM_LOCK},
									  {"PRINT_SCREEN", Key::KEY_PRINT_SCREEN},
									  {"PAUSE", Key::KEY_PAUSE},
									  {"LEFT_SHIFT", Key::KEY_LEFT_SHIFT},
									  {"LEFT_CONTROL", Key::KEY_LEFT_CONTROL},
									  {"LEFT_ALT", Key::KEY_LEFT_ALT},
									  {"RIGHT_SHIFT", Key::KEY_RIGHT_SHIFT},
									  {"RIGHT_CONTROL", Key::KEY_RIGHT_CONTROL},
									  {"RIGHT_ALT", Key::KEY_RIGHT_ALT},
								  });
	lua.new_usertype<Physics>("Physics",
							  sol::constructors<Physics()>(),
							  "p", &Physics::p,
							  "v", &Physics::v,
							  "a", &Physics::a,
							  "g", &Physics::g);
	lua.new_usertype<Sprite>("Sprite",
							 sol::constructors<Sprite()>(),
							 "name", &Sprite::name,
							 "xPos", &Sprite::xPos,
							 "yPos", &Sprite::yPos,
							 "scale", &Sprite::scale,
							 "z", &Sprite::z);
	lua.new_usertype<Health>("Health",
							 sol::constructors<Health()>(),
							 "percent", &Health::percent);
	lua.new_usertype<Script>("Script",
							 sol::constructors<Script()>(),
							 "name", &Script::name);
	lua.new_usertype<glm::vec3>("vec3",
								sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
								"x", &glm::vec3::x,
								"y", &glm::vec3::y,
								"z", &glm::vec3::z,
								sol::meta_function::addition, sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3
																			{ return v1 + v2; }),
								sol::meta_function::subtraction, sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3
																			   { return v1 - v2; }),
								sol::meta_function::multiplication, sol::overload([](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3
																				  { return v1 * v2; },
																				  [](const glm::vec3 &v1, float f) -> glm::vec3
																				  { return v1 * f; },
																				  [](float f, const glm::vec3 &v1) -> glm::vec3
																				  { return f * v1; }));
	lua.new_usertype<glm::vec2>("vec2",
								sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
								"x", &glm::vec2::x,
								"y", &glm::vec2::y,
								sol::meta_function::addition, sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) -> glm::vec2
																			{ return v1 + v2; }),
								sol::meta_function::subtraction, sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) -> glm::vec2
																			   { return v1 - v2; }),
								sol::meta_function::multiplication, sol::overload([](const glm::vec2 &v1, const glm::vec2 &v2) -> glm::vec2
																				  { return v1 * v2; },
																				  [](const glm::vec2 &v1, float f) -> glm::vec2
																				  { return v1 * f; },
																				  [](float f, const glm::vec2 &v1) -> glm::vec2
																				  { return f * v1; }));
	lua.new_usertype<BoundingBox>("BoundingBox",
								  sol::constructors<BoundingBox()>(),
								  "x", &BoundingBox::x,
								  "y", &BoundingBox::y,
								  "width", &BoundingBox::width,
								  "height", &BoundingBox::height);
	lua.set_function("isKeyPressed", [&](const int keycode)
					 { return globalEngine.input.isKeyPressed(static_cast<Key>(keycode)); });
	lua.set_function("quit", [&](void)
					 { globalEngine.shutdown(); });
	lua.set_function("loadImg", [&](const std::string &name, const std::string &path)
					 { return globalEngine.graphics.loadImg(name, path); });
	lua.set_function("loadSound", [&](const std::string &name, const std::string &path)
					 { globalEngine.sound.loadSound(name, path); });
	lua.set_function("playSound", [&](const std::string &name)
					 { globalEngine.sound.playSound(name); });
	lua.set_function("createEntity", [&](void)
					 { return globalEngine.ecs.Create(); });
	lua.set_function("destroyEntity", [&](EntityID e)
					 { globalEngine.ecs.Destroy(e); });
	lua.set_function("getPhysics", [&](EntityID e) -> Physics &
					 { return globalEngine.ecs.Get<Physics>(e); });
	lua.set_function("getSprite", [&](EntityID e) -> Sprite &
					 { return globalEngine.ecs.Get<Sprite>(e); });
	lua.set_function("getHealth", [&](EntityID e) -> Health &
					 { return globalEngine.ecs.Get<Health>(e); });
	lua.set_function("getScript", [&](EntityID e) -> Script &
					 { return globalEngine.ecs.Get<Script>(e); });
	lua.set_function("getBoundingBox", [&](EntityID e) -> BoundingBox&
					{ return globalEngine.ecs.Get<BoundingBox>(e); });
	lua.set_function("getWidth", [&](const std::string& name)
					{ return globalEngine.graphics.nameToImage.at(name).width; });
	lua.set_function("getHeight", [&](const std::string& name)
					{ return globalEngine.graphics.nameToImage.at(name).height; });
	return true;
}

void ScriptManager::update()
{
	globalEngine.ecs.ForEach<Script>([&](EntityID e)
									 {
		Script& s = globalEngine.ecs.Get<Script>(e);
		sol::load_result& f = nameToLoadResult.at(s.name);
		f(); });
}

bool ScriptManager::LoadScript(const std::string &name, const std::string &path)
{
	nameToLoadResult.insert(std::pair(name, lua.load_file(path)));
	return true;
}

sol::load_result &ScriptManager::GetScript(const std::string &name)
{
	sol::load_result &result = nameToLoadResult.at(name);
	return result;
}