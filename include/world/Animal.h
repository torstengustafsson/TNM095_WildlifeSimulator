#pragma once

#include "Object.h"
#include "../core/RandomGenerator.h"
#include "../core/Utilities.h"

#include <vector>
#include <functional>

using namespace utilities;

class Animal : public Object
{
public:
	virtual ~Animal() = default;
	Animal( Texture* _tex, string _name, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 },
			float _max_age = 1000.0, float _max_speed = 2.0, float stamina = 0.5, float _attack_power = 0.5 );

	float get_age();

	void set_stage(int val);

	virtual void update() = 0;
protected:
	RandomGenerator generateRand;

	float age = 0.0; //in seconds
	float hunger_level = 0.5; //decreases with time. 0.0 = death. 1.0 = full.
	bool alive = true; //becomes false when animal dies

	float size; //the size of the animal
	float food_value; //amount of hunger animal restores when eaten
	float prefer_company; //higher means more likely to flock with others of its kind
	
	//this function decides which action to take
	void scan_area();

	//the current state which is executed every frame.
	function<void()> current_state;

	//states/actions
	function<void()> idle;
	function<void()> find_food();
	function<void()> find_mate();
	function<void()> flee();
	function<void()> attack();
	function<void()> fight(); //shared state
	function<void()> mate(); //shared state
	function<void()> eat(); //special shared state (must be with another dead animal, or grass)
	function<void()> dead();

	const float scan_radius = 50.0;
	const float max_age; //age at which the animal dies
	const float max_speed; //maximum running speed
	const float stamina; //determines how fast hunger_level decreases in special draining states
	const float attack_power; //higher means more likely to win a fight

	//Used by the Case-based Reasoning machine to determine which action to take when in contact with another animal
	struct Species
	{
		unsigned int type;
		float danger_level;
		float hunt_level;
	};

	vector<Species> discovered_species;

	//State specific variables. Should probably be moved to separate class or something
	vec2 goal = { 0.0, 0.0 };
	void move(vec2 from, vec2 to);
};