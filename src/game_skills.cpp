
struct Skill_State {
	s32 requiem_duration;
	s32 axiom_one_duration;
	s32 ddos_attack_duration;
	s32 paricle_rendering_duration;
	s32 neural_network_duration;
	s32 automata_summon_duration;
	s32 inheritance_duration;
	s32 overclock_duration;

	bool graph_coloring = false;

	Skill_ID inheritance_copy = SKILL_NONE;

	Skill_ID cooldowns[SKILL_NUMBER];
};

struct Skill_Counter {
	s32 contradiction_target = -1;
	s32 contradiction_zero_index = -1;
	s32 tautology_target = -1;
	s32 automata_summon_norma_index = -1;
	s32 inheritante_target = -1;
	s32 inheritance_new_index = -1;
};

static Skill_State skill_state_ally = { };
static Skill_State skill_state_enemy = { };

static Skill_Counter skill_counter_ally = { };
static Skill_Counter skill_counter_enemy = {};

s32 calculate_absorption(s32 initial, s32 absorption, s32* extra_on_hp) {
	*extra_on_hp = 0;
	if (initial > absorption) {
		*extra_on_hp = initial - absorption;
		return absorption;
	} else {
		return initial;
	}
}

s32 calculate_reduction(s32 initial, s32 reduction, bool percent) {
	if (percent) {
		return initial - ((initial * reduction) / 100);
	}
	if (initial > reduction) {
		return initial - reduction;
	} else {
		return 0;
	}
}

// return negative damage value on reflex
s32 calculate_enemy_damage_reduction(int target_index, int damage, Skill_Damage dmg_type, Skill_ID id, Combat_State* combat_state) {
	s32 dmg = damage;
	u32 reduction = combat_state->enemy.reduction[target_index];
	if (reduction != 0) {
		if (reduction & SKILL_DEF_RELECTION)
			return -damage;
		if (reduction & SKILL_DEF_INVULNERABILITY) {
			u32 type = combat_state->enemy.reduction_type[target_index];
			if (id == SKILL_BRUTE_FORCE) {
				return 10;
			}
			if (type & SKILL_TYPE_PHYSICAL && skill_groups[target_index * NUM_CHARS].type == SKILL_TYPE_PHYSICAL)
				return 0;
			if (type & SKILL_TYPE_MENTAL && skill_groups[target_index * NUM_CHARS].type == SKILL_TYPE_MENTAL)
				return 0;
			if (type & SKILL_TYPE_VIRTUAL && skill_groups[target_index * NUM_CHARS].type == SKILL_TYPE_VIRTUAL)
				return 0;
		}

		if (dmg_type == SKILL_DMG_NORMAL) {
			// reduction absorption reflex
			if(reduction & SKILL_DEF_REDUCTION)
				dmg = calculate_reduction(dmg, combat_state->enemy.reduction_points[target_index][SKILL_DEF_REDUCTION],
					combat_state->enemy.reduction_percentile[target_index][SKILL_DEF_REDUCTION]);
			if (reduction & SKILL_DEF_ABSORPTION) {
				s32 extra_on_hp = 0;
				s32 absorp_pts = combat_state->enemy.reduction_points[target_index][SKILL_DEF_ABSORPTION];
				dmg = calculate_absorption(dmg, absorp_pts, &extra_on_hp);
				combat_state->enemy.reduction_points[target_index][SKILL_DEF_ABSORPTION] = MAX(0, absorp_pts - dmg);
				dmg = extra_on_hp;
			}
		} else if (dmg_type == SKILL_DMG_PIERCING) {
			// absorption reflex
			if (reduction & SKILL_DEF_ABSORPTION) {
				s32 extra_on_hp = 0;
				s32 absorp_pts = combat_state->enemy.reduction_points[target_index][SKILL_DEF_ABSORPTION];
				dmg = calculate_absorption(dmg, absorp_pts, &extra_on_hp);
				combat_state->enemy.reduction_points[target_index][SKILL_DEF_ABSORPTION] = MAX(0, absorp_pts - dmg);
				dmg = extra_on_hp;
			}
		} else if (dmg_type == SKILL_DMG_CRUSHING) {
			// reflex
		}
	}
	return dmg;
}

s32 calculate_ally_damage_reduction(int target_index, int damage, Skill_Damage dmg_type, Skill_ID id, Combat_State* combat_state) {
	s32 dmg = damage;
	u32 reduction = combat_state->player.reduction[target_index];
	if (reduction != 0) {
		if (reduction & SKILL_DEF_RELECTION)
			return -damage;
		if (reduction & SKILL_DEF_INVULNERABILITY) {
			u32 type = combat_state->player.reduction_type[target_index];
			Skill_Group sgroup;
			if (type & SKILL_TYPE_PHYSICAL && skill_groups[id].type == SKILL_TYPE_PHYSICAL)
				return 0;
			if (type & SKILL_TYPE_MENTAL && skill_groups[id].type == SKILL_TYPE_MENTAL)
				return 0;
			if (type & SKILL_TYPE_VIRTUAL && skill_groups[id].type == SKILL_TYPE_VIRTUAL)
				return 0;
		}

		if (dmg_type == SKILL_DMG_NORMAL) {
			// reduction absorption reflex
			if (reduction & SKILL_DEF_REDUCTION)
				dmg = calculate_reduction(dmg, combat_state->player.reduction_points[target_index][SKILL_DEF_REDUCTION],
					combat_state->player.reduction_percentile[target_index][SKILL_DEF_REDUCTION]);
			if (reduction & SKILL_DEF_ABSORPTION) {
				s32 extra_on_hp = 0;
				s32 absorp_pts = combat_state->player.reduction_points[target_index][SKILL_DEF_ABSORPTION];
				dmg = calculate_absorption(dmg, absorp_pts, &extra_on_hp);
				combat_state->player.reduction_points[target_index][SKILL_DEF_ABSORPTION] = MAX(0, absorp_pts - dmg);
				dmg = extra_on_hp;
			}
		}
		else if (dmg_type == SKILL_DMG_PIERCING) {
			// absorption reflex
			if (reduction & SKILL_DEF_ABSORPTION) {
				s32 extra_on_hp = 0;
				s32 absorp_pts = combat_state->player.reduction_points[target_index][SKILL_DEF_ABSORPTION];
				dmg = calculate_absorption(dmg, absorp_pts, &extra_on_hp);
				combat_state->player.reduction_points[target_index][SKILL_DEF_ABSORPTION] = MAX(0, absorp_pts - dmg);
				dmg = extra_on_hp;
			}
		}
		else if (dmg_type == SKILL_DMG_CRUSHING) {
			// reflex
		}
	}
	return dmg;
}

s32 deal_damage_to_target_ally(int target_index, int source_index, int damage, Skill_Damage dmg_type, Skill_ID skill_id, Combat_State* combat_state);

// if source_index is -1 stop the reflection to not go infinite
// returns the damage done
s32 deal_damage_to_target_enemy(int target_index, int source_index, int damage, Skill_Damage dmg_type, Skill_ID skill_id, Combat_State* combat_state) {
	s32 dmg = calculate_enemy_damage_reduction(target_index, damage, dmg_type, skill_id, combat_state);
	if (dmg < 0 && source_index != -1) {
		// suffered reflex
		deal_damage_to_target_ally(source_index, -1, -dmg, dmg_type, skill_id, combat_state);
		return dmg;
	}

	combat_state->enemy.hp[target_index] = MAX(0, combat_state->enemy.hp[target_index] - dmg);
	layout_set_enemy_hp(target_index, combat_state->enemy.max_hp[target_index], combat_state->enemy.hp[target_index]);
	if (combat_state->enemy.hp[target_index] == 0) {
		layout_enemy_die(target_index);
	}
	return dmg;
}

// if source_index is -1 stop the reflection to not go infinite
s32 deal_damage_to_target_ally(int target_index, int source_index, int damage, Skill_Damage dmg_type, Skill_ID skill_id, Combat_State* combat_state) {
	s32 dmg = calculate_ally_damage_reduction(target_index, damage, dmg_type, skill_id, combat_state);
	if (dmg < 0 && source_index != -1) {
		// suffered reflex
		deal_damage_to_target_enemy(source_index, -1, -dmg, dmg_type, skill_id, combat_state);
		return dmg;
	}

	combat_state->player.hp[target_index] = MAX(0, combat_state->player.hp[target_index] - dmg);
	layout_set_ally_hp(target_index, combat_state->player.max_hp[target_index], combat_state->player.hp[target_index]);
	if (combat_state->player.hp[target_index] == 0) {
		layout_ally_die(target_index);
	}
	return dmg;
}

struct Skill_Target {
	s32 number;
	bool ally;
	bool enemy;
	bool all;
	bool self;
};

static bool is_enemy_targetable_by_skill(Skill_ID skill, s32 enemy_index, Combat_State* combat_state) {
	if (combat_state->enemy.hp[enemy_index] <= 0) return false;
	if (combat_state->enemy.reduction[enemy_index] & SKILL_DEF_INVULNERABILITY) {
		if (skill == SKILL_BRUTE_FORCE)
			return true;
		if (skill_groups[skill].type & combat_state->enemy.reduction_type[enemy_index])
			return false;
	}
	return true;
}

static bool is_ally_targetable_by_skill(Skill_ID skill, s32 ally_index, Combat_State* combat_state) {
	if (combat_state->player.hp[ally_index] <= 0) return false;
	if (combat_state->player.reduction[ally_index] & SKILL_DEF_INVULNERABILITY) {
		if (skill == SKILL_BRUTE_FORCE)
			return true;
		if (skill_groups[skill].type & combat_state->player.reduction_type[ally_index])
			return false;
	}
	return true;
}

Skill_Target skill_need_targeting(Skill_ID id, Combat_State* combat_state) {
	Skill_Target target = { 0 };
	target.number = 0;
	target.enemy = true;
	target.all = false;
	target.self = false;
	switch (id) {
		case SKILL_FALSE_RUSH: {
			// if requiem is still active, affect all enemies (AoE)
			if (skill_state_ally.requiem_duration > 0) {
				target.number = 0;
				target.all = true;
			} else {
				target.number = 1;
			}
		}break;
		case SKILL_CONTRADICTION: {
			if (skill_state_ally.requiem_duration > 0) {
				target.number = 0;
				target.all = true;
			} else {
				target.number = 1;
			}
		}break;
		case SKILL_REQUIEM_ZERO:  target.number = 0; target.self = true; break;
		case SKILL_TRUTH_SLASH:   target.number = 1; break;
		case SKILL_TAUTOLOGY:     target.number = 1; break;
		case SKILL_AXIOM_ONE:     target.number = 0; target.self = true; break;
		case SKILL_BRUTE_FORCE:   target.number = 1; break;
		case SKILL_BUFFER_OVERFLOW:    target.number = 1; break;
		case SKILL_DDOS_ATTACK:   target.number = 0; target.all = true; break;
		case SKILL_PARTICLE_RENDERING: target.number = 1; break;
		case SKILL_DIFFUSE_REFLECTION: target.number = 0; target.enemy = false; target.ally = true; target.all = true; break;
		case SKILL_DYNAMIC_FRUSTUM_ATTACK: target.number = 0; target.all = true; break;
		case SKILL_Q_PUNCH:       target.number = 1; break;
		case SKILL_PERCEPTRON:    target.number = 1; break;
		case SKILL_NEURAL_NETWORK: target.number = 0; target.all = true; break;
		case SKILL_PREEMPTION:    target.number = 1; break;
		case SKILL_MUTEX:         target.number = 0; target.all = true; break;
		case SKILL_THREAD_SCHEDULING: target.number = 0; break;
		case SKILL_PUMPING_UP:    target.number = 1; break;
		case SKILL_AUTOMATA_SUMMON: target.number = 0; target.self = true; break;
		case SKILL_TURING_MACHINE: target.number = 0; target.all = true; break;
		case SKILL_TMR:           target.number = 0; target.all = true; break;
		case SKILL_REDUNDANCY:    target.number = 0; target.all = true; target.ally = true; target.enemy = false; break;
		case SKILL_ROLLBACK:      target.number = 1; target.ally = true; target.enemy = false; break;
		case SKILL_ALT:           target.number = 0; target.self = true; break;
		case SKILL_CTRL:          target.number = 1; break;
		case SKILL_DELETE:        target.number = 1; break;	// @check ctrl effect
		case SKILL_BEST_BOUND_FIST: target.number = 1; break;
		case SKILL_DUAL_SIMPLEX:  target.number = 1; target.ally = true; break;
		case SKILL_GRAPH_COLORING: target.number = 0; target.self = true; target.enemy = false; break;
		case SKILL_SPRINT_BURST:  target.number = 1; break;
		case SKILL_INHERITANCE:   target.number = 1; break;
		case SKILL_OVERRIDE:      target.number = 1; target.ally = true; target.enemy = false; break;
		case SKILL_CLOCK_PULSE:   target.number = 1; break;
		case SKILL_PIPELINE:      target.number = 0; target.self = true; break;
		case SKILL_OVERCLOCK:     target.number = 0; target.self = true; break;

			// Invulnerability skills
		case SKILL_BRANCH_DAMAGE:
		case SKILL_POLIMORPHISM:
		case SKILL_KNAPSACK_HIDEOUT:
		case SKILL_ESC:
		case SKILL_ROLLFORWARD:
		case SKILL_NON_DETERMINISM:
		case SKILL_FORK:
		case SKILL_HILL_CLIMBING:
		case SKILL_RASTERIZATION:
		case SKILL_ENCRYPTION:
		case SKILL_TRUE_ENDURANCE:
		case SKILL_VOID_BARRIER: {
			target.number = 0;
			target.self = true;
		} break;
	}
	return target;
}

// This holds the state of the status in the layout this struct does not have
// and cannot have any effect on the game state
struct Status_Layout {
	Skill_Condition ally_status[NUM_ALLIES][MAX_STATUS];
	Skill_Condition enemy_status[NUM_ENEMIES][MAX_STATUS];

	Texture* status_images[SKILL_CONDITION_NUMBER];
};
Status_Layout g_layout_status = {};

void apply_status_to_enemy(s32 target_index, Skill_Condition status, s32 duration, Combat_State* combat_state) {
	if (combat_state->enemy.hp[target_index] <= 0)
		return;
	combat_state->enemy.status[target_index] |= status;
	combat_state->enemy.status_duration[target_index][status] = duration;

	// Layout
	for (int i = 0; i < MAX_STATUS; ++i) {
		if (g_layout_status.enemy_status[target_index][i] == SKILL_CONDITION_NONE) {
			g_layout_status.enemy_status[target_index][i] = status;
			layout_apply_status_enemy(target_index, i, g_layout_status.status_images[status]);
			break;
		}
	}
}

void remove_status_from_enemy(s32 target_index, Skill_Condition status, Combat_State* combat_state) {
	combat_state->enemy.status[target_index] &= !status;
	combat_state->enemy.status_duration[target_index][status] = 0;

	// Layout
	bool cascade_back = false;
	for (int i = 0; i < MAX_STATUS; ++i) {
		if (cascade_back && g_layout_status.enemy_status[target_index][i]) {
			assert(i >= 1);
			g_layout_status.enemy_status[target_index][i - 1] = g_layout_status.enemy_status[target_index][i];
			layout_apply_status_enemy(target_index, i - 1, g_layout_status.status_images[g_layout_status.enemy_status[target_index][i]]);
			layout_apply_status_enemy(target_index, i, 0);	
		} else if (cascade_back) {
			g_layout_status.enemy_status[target_index][i - 1] = SKILL_CONDITION_NONE;
			break;
		}
		if (g_layout_status.enemy_status[target_index][i] & status) {
			g_layout_status.enemy_status[target_index][i] = SKILL_CONDITION_NONE;
			layout_apply_status_enemy(target_index, i, 0);
			cascade_back = true;
		}
	}
}

void apply_status_to_ally(s32 target_index, Skill_Condition status, s32 duration, Combat_State* combat_state) {
	if (combat_state->player.hp[target_index] <= 0)
		return;
	combat_state->player.status[target_index] |= status;
	combat_state->player.status_duration[target_index][status] = duration;

	// Layout
	for (int i = 0; i < MAX_STATUS; ++i) {
		if (g_layout_status.ally_status[target_index][i] == SKILL_CONDITION_NONE) {
			g_layout_status.ally_status[target_index][i] = status;
			layout_apply_status_ally(target_index, i, g_layout_status.status_images[status]);
			break;
		}
	}
}

void remove_status_from_ally(s32 target_index, Skill_Condition status, Combat_State* combat_state) {
	combat_state->player.status[target_index] &= !status;
	combat_state->player.status_duration[target_index][status] = 0;

	// Layout
	bool cascade_back = false;
	for (int i = 0; i < MAX_STATUS; ++i) {
		if (cascade_back && g_layout_status.ally_status[target_index][i]) {
			assert(i >= 1);
			g_layout_status.ally_status[target_index][i - 1] = g_layout_status.ally_status[target_index][i];
			layout_apply_status_ally(target_index, i - 1, g_layout_status.status_images[g_layout_status.ally_status[target_index][i]]);
			layout_apply_status_ally(target_index, i, 0);
		} else if (cascade_back) {
			g_layout_status.ally_status[target_index][i - 1] = SKILL_CONDITION_NONE;
			break;
		}
		if (g_layout_status.ally_status[target_index][i] & status) {
			g_layout_status.ally_status[target_index][i] = SKILL_CONDITION_NONE;
			layout_apply_status_ally(target_index, i, 0);
			cascade_back = true;
		}
	}
}

void gain_absorption_ally(s32 points, s32 ally_index, u32 skill_type, Combat_State* combat_state) {
	combat_state->player.reduction[ally_index] = SKILL_DEF_ABSORPTION;
	combat_state->player.reduction_type[ally_index] = skill_type;
	combat_state->player.reduction_points[ally_index][SKILL_DEF_ABSORPTION] += points;
	combat_state->player.reduction_duration[ally_index][SKILL_DEF_ABSORPTION] = INT_MAX;		// @todo check if absorption is forever
}

void gain_absorption_enemy(s32 points, s32 enemy_index, u32 skill_type, Combat_State* combat_state) {
	combat_state->enemy.reduction[enemy_index] = SKILL_DEF_ABSORPTION;
	combat_state->enemy.reduction_type[enemy_index] = skill_type;
	combat_state->enemy.reduction_points[enemy_index][SKILL_DEF_ABSORPTION] += points;
	combat_state->enemy.reduction_duration[enemy_index][SKILL_DEF_ABSORPTION] = INT_MAX;			// @todo check if absorption is forever
}

void gain_reduction_ally(s32 points, s32 duration, bool as_percentage, s32 ally_index, u32 skill_type, Combat_State* combat_state) {
	combat_state->player.reduction[ally_index] = SKILL_DEF_REDUCTION;
	combat_state->player.reduction_type[ally_index] = skill_type;
	combat_state->player.reduction_duration[ally_index][SKILL_DEF_REDUCTION] = duration + 1;
	combat_state->player.reduction_points[ally_index][SKILL_DEF_REDUCTION] += points;
	combat_state->player.reduction_percentile[ally_index][SKILL_DEF_REDUCTION] = as_percentage;
}

void gain_reduction_enemy(s32 points, s32 duration, bool as_percentage, s32 enemy_index, u32 skill_type, Combat_State* combat_state) {
	combat_state->enemy.reduction[enemy_index] = SKILL_DEF_REDUCTION;
	combat_state->enemy.reduction_type[enemy_index] = skill_type;
	combat_state->enemy.reduction_duration[enemy_index][SKILL_DEF_REDUCTION] = duration + 1;
	combat_state->enemy.reduction_points[enemy_index][SKILL_DEF_REDUCTION] += points;
	combat_state->enemy.reduction_percentile[enemy_index][SKILL_DEF_REDUCTION] = as_percentage;
}

void gain_invulnerability_ally(s32 ally_index, s32 duration, u32 skill_type, Combat_State* combat_state) {
	combat_state->player.reduction[ally_index] = SKILL_DEF_INVULNERABILITY;
	combat_state->player.reduction_type[ally_index] = skill_type;
	combat_state->player.reduction_duration[ally_index][SKILL_DEF_INVULNERABILITY] = duration + 1;
}

void gain_invulnerability_enemy(s32 enemy_index, s32 duration, u32 skill_type, Combat_State* combat_state) {
	combat_state->enemy.reduction[enemy_index] = SKILL_DEF_INVULNERABILITY;
	combat_state->enemy.reduction_type[enemy_index] = skill_type;
	combat_state->enemy.reduction_duration[enemy_index][SKILL_DEF_INVULNERABILITY] = duration + 1;
}

void gain_reflection_ally(s32 ally_index, s32 duration, u32 skill_type, Combat_State* combat_state) {
	combat_state->player.reduction[ally_index] = SKILL_DEF_RELECTION;
	combat_state->player.reduction_type[ally_index] = skill_type;
	combat_state->player.reduction_duration[ally_index][SKILL_DEF_RELECTION] = duration + 1;
}

void gain_reflection_enemy(s32 enemy_index, s32 duration, u32 skill_type, Combat_State* combat_state) {
	combat_state->enemy.reduction[enemy_index] = SKILL_DEF_RELECTION;
	combat_state->enemy.reduction_type[enemy_index] = skill_type;
	combat_state->enemy.reduction_duration[enemy_index][SKILL_DEF_RELECTION] = duration + 1;
}

//
//
//		TODO WARNING IMPORTANT CHECK SKILLS THAT CAN BE USED IN ALLIES AND ENEMIES (from_enemy, on_enemy) are important
//
//

s32 execute_skill(Skill_ID id, int target_index, int source_index, Combat_State* combat_state, bool from_enemy, bool on_enemy) {
	Skill_State* skill_state = 0;
	s32(*deal_damage_to_target)(int, int, int, Skill_Damage, Skill_ID, Combat_State*) = 0;// (void(*)(int, int, Skill_Damage, Skill_ID, Combat_State*))0;
	bool(*is_targetable_by_skill)(Skill_ID skill, s32 index, Combat_State* combat_state) = 0;
	if (!from_enemy) {	// on_enemy
		skill_state = &skill_state_ally;
		deal_damage_to_target = deal_damage_to_target_enemy;
		is_targetable_by_skill = is_enemy_targetable_by_skill;
	} else {
		skill_state = &skill_state_enemy;
		deal_damage_to_target = deal_damage_to_target_ally;
		is_targetable_by_skill = is_ally_targetable_by_skill;
	}

	Skill_Target needs_targets = skill_need_targeting(id, combat_state);

	// Counter
	if (from_enemy) {
		// if the counter comes from enemy, source_index gotta be checked against skill_counter_enemy
		if (source_index == skill_counter_enemy.contradiction_target && target_index == skill_counter_enemy.contradiction_zero_index) {
			// enemy source receives 20 dmg, do nothing and receive status paralyze
			printf("enemy countered by contradiction!\n");
			const int extra = 1;	// needed because it updates at the start of the player turn
			deal_damage_to_target_enemy(source_index, target_index, 20, skill_groups[id].damage, id, combat_state);
			apply_status_to_enemy(source_index, SKILL_CONDITION_PARALYZE, 1 + extra, combat_state);
			return 0;
		}
		if (source_index == skill_counter_enemy.tautology_target) {
			// sofre 15 de dano crushing
			printf("enemy countered by tautology!\n");
			deal_damage_to_target_enemy(source_index, target_index, 15, SKILL_DMG_CRUSHING, id, combat_state);
		}
		if ((needs_targets.all && skill_counter_ally.automata_summon_norma_index != -1)|| skill_counter_ally.automata_summon_norma_index == target_index) {
			printf("enemy countered by automata summon!\n");
			apply_status_to_enemy(source_index, SKILL_CONDITION_POISON, 2, combat_state);
			skill_counter_ally.automata_summon_norma_index = -1;
		}
		if (source_index == skill_counter_enemy.inheritante_target) {
			printf("enemy countered by inheritance");
			printf("copied skill of id %d\n", id);
			if (skill_groups[id].unique == SKILL_NOT_UNIQUE) {
				skill_state_ally.inheritance_copy = id;
				skill_state_ally.inheritance_duration = 1;
				linked::Button* b = gw.allies_skills[skill_counter_enemy.inheritance_new_index * NUM_SKILLS + 1]->divs[0]->getButtons()[0];
				b->setAllBGTexture(skill_textures[id]);
			}
		}
	} else {
		// if the counter comes from ally, source_index gotta be checked against skill_counter_ally
		if (source_index == skill_counter_ally.contradiction_target&& target_index == skill_counter_ally.contradiction_zero_index) {
			// ally source receives 20 dmg, do nothing and receive status paralyze
			printf("ally countered by contradiction!\n");
			const int extra = 1;	// needed because it updates at the start of the player turn
			deal_damage_to_target_ally(source_index, target_index, 20, skill_groups[id].damage, id, combat_state);
			apply_status_to_ally(source_index, SKILL_CONDITION_PARALYZE, 1 + extra, combat_state);
			return 0;
		}
		if (source_index == skill_counter_ally.tautology_target) {
			// sofre 15 de dano crushing
			printf("ally countered by tatology!\n");
			deal_damage_to_target_ally(source_index, target_index, 15, SKILL_DMG_CRUSHING, id, combat_state);
		}
		if ((needs_targets.all && skill_counter_enemy.automata_summon_norma_index != -1) || skill_counter_enemy.automata_summon_norma_index == target_index) {
			printf("ally countered! by automata summon\n");
			apply_status_to_ally(source_index, SKILL_CONDITION_POISON, 2, combat_state);
			skill_counter_enemy.automata_summon_norma_index = -1;
		}
		if (source_index == skill_counter_ally.inheritante_target) {
			printf("ally countered by inheritance\n");
			printf("copied skill of id %d\n", id);
			if (skill_groups[id].unique == SKILL_NOT_UNIQUE) {
				skill_state_enemy.inheritance_copy = id;
				skill_state_enemy.inheritance_duration = 1;
			}
		}
	}

	// Execute
	switch (id) {
		// Zero
		case SKILL_FALSE_RUSH: {
			// if requiem is still active, affect all enemies (AoE)
			if (skill_state->requiem_duration > 0) {
				int num_targets = (from_enemy) ? NUM_ENEMIES : NUM_ALLIES;
				for (int i = 0; i < num_targets; ++i) {
					if (is_targetable_by_skill(SKILL_FALSE_RUSH, i, combat_state))
						deal_damage_to_target(i, source_index, 20, SKILL_DMG_NORMAL, id, combat_state);
				}
			} else {
				// Normal behavior
				deal_damage_to_target(target_index, source_index, 20, SKILL_DMG_NORMAL, id, combat_state);
			}
		}break;
		case SKILL_CONTRADICTION: {
			// mark enemy but ally is marked when receiving this
			if (from_enemy) {
				skill_counter_ally.contradiction_target = target_index;
				skill_counter_ally.contradiction_zero_index = source_index;
			} else {
				skill_counter_enemy.contradiction_target = target_index;
				skill_counter_enemy.contradiction_zero_index = source_index;
			}
		}break;
		case SKILL_REQUIEM_ZERO: {
			skill_state->requiem_duration = 3 + 1;
		}break;

		// One
		case SKILL_TRUTH_SLASH: {
			deal_damage_to_target(target_index, source_index, 30, SKILL_DMG_NORMAL, id, combat_state);
		}break;
		case SKILL_TAUTOLOGY: {
			deal_damage_to_target(target_index, source_index, 15, SKILL_DMG_PIERCING, id, combat_state);
			if (from_enemy) {
				skill_counter_ally.tautology_target = target_index;
			} else {
				skill_counter_enemy.tautology_target = target_index;
			}
		}break;
		case SKILL_AXIOM_ONE: {
			skill_state->axiom_one_duration = 3 + 1;
			if (from_enemy) {
				gain_reduction_enemy(15, 3, true, source_index, SKILL_TYPE_PHYSICAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				gain_reduction_ally(15, 3, true, source_index, SKILL_TYPE_PHYSICAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		} break;

		// Serial Keyller
		case SKILL_BRUTE_FORCE: {
			int damage = 20;
			deal_damage_to_target(target_index, source_index, damage, SKILL_DMG_NORMAL, id, combat_state);
		}break;
		case SKILL_BUFFER_OVERFLOW: {
			deal_damage_to_target(target_index, source_index, 15, SKILL_DMG_PIERCING, id, combat_state);
			if (from_enemy) {
				apply_status_to_ally(target_index, SKILL_CONDITION_STUN, 1, combat_state);
			} else {
				apply_status_to_enemy(target_index, SKILL_CONDITION_STUN, 1, combat_state);
			}
		}break;
		case SKILL_DDOS_ATTACK: {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (!is_targetable_by_skill(SKILL_DDOS_ATTACK, i, combat_state))
					continue;
				if (from_enemy) {
					apply_status_to_ally(i, SKILL_CONDITION_PARALYZE, 3, combat_state);
				} else {
					apply_status_to_enemy(i, SKILL_CONDITION_PARALYZE, 3, combat_state);
				}
			}
		}break;

		// Ray Tracey
		case SKILL_PARTICLE_RENDERING: {
			deal_damage_to_target(target_index, source_index, 15, SKILL_DMG_NORMAL, id, combat_state);
			if (from_enemy) {
				gain_invulnerability_enemy(source_index, 1, SKILL_TYPE_PHYSICAL, combat_state);
			} else {
				gain_invulnerability_ally(source_index, 1, SKILL_TYPE_PHYSICAL, combat_state);
			}
		}break;
		case SKILL_DIFFUSE_REFLECTION: {
			if (from_enemy) {
				for (int i = 0; i < NUM_ENEMIES; ++i) {
					if (is_targetable_by_skill(SKILL_DIFFUSE_REFLECTION, i, combat_state))
						gain_reflection_enemy(i, 1, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
				}
			} else {
				for (int i = 0; i < NUM_ALLIES; ++i) {
					if (is_targetable_by_skill(SKILL_DIFFUSE_REFLECTION, i, combat_state))
						gain_reflection_ally(i, 1, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
				}
			}
		}break;
		case SKILL_DYNAMIC_FRUSTUM_ATTACK: {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (is_targetable_by_skill(SKILL_DYNAMIC_FRUSTUM_ATTACK, i, combat_state))
					deal_damage_to_target(i, source_index, 35, SKILL_DMG_NORMAL, id, combat_state);
			}
		}break;

		// A-Star
		case SKILL_Q_PUNCH: {
			int damage = 20;
			int cumulative_damage = 5;
			if (from_enemy) {
				int num_accumulated = combat_state->player.cumulative_skill[target_index][SKILL_Q_PUNCH];
				cumulative_damage *= num_accumulated;
				deal_damage_to_target(target_index, source_index, damage + cumulative_damage, SKILL_DMG_NORMAL, id, combat_state);
				combat_state->player.cumulative_skill[target_index][SKILL_Q_PUNCH] += 1;
			} else {
				int num_accumulated = combat_state->enemy.cumulative_skill[target_index][SKILL_Q_PUNCH];
				cumulative_damage *= num_accumulated;
				deal_damage_to_target(target_index, source_index, damage + cumulative_damage, SKILL_DMG_NORMAL, id, combat_state);
				combat_state->enemy.cumulative_skill[target_index][SKILL_Q_PUNCH] += 1;
			}
		}break;
		case SKILL_PERCEPTRON: {
			// @todo add check for neural network
			deal_damage_to_target(target_index, source_index, 25, SKILL_DMG_CRUSHING, id, combat_state);
		}break;
		case SKILL_NEURAL_NETWORK: {
			skill_state->neural_network_duration = 4 + 1;
			if (from_enemy) {
				gain_absorption_enemy(30, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				gain_absorption_ally(30, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		}break;
			
		// Deadlock
		case SKILL_PREEMPTION: {
			// @ ask wtf is CONTROL
			// Caso ele tenha alguma habilidade com duração CONTROL, elimina a habilidade e este sofre status SLEEP por 2 turnos.
			deal_damage_to_target(target_index, source_index, 25, SKILL_DMG_NORMAL, id, combat_state);
		}break;
		case SKILL_MUTEX: {
			// @todo UNIQUE
			// Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.
		}break;
		case SKILL_THREAD_SCHEDULING: {
			if (from_enemy) {
				int random = rand() % (NUM_ALLIES + 1);
				apply_status_to_ally(random, SKILL_CONDITION_PARALYZE, 3, combat_state);
			} else {
				int random = rand() % (NUM_ENEMIES + 1);
				apply_status_to_enemy(random, SKILL_CONDITION_PARALYZE, 3, combat_state);
			}
		}break;

		// Norma
		case SKILL_PUMPING_UP: {
			deal_damage_to_target(target_index, source_index, 25, SKILL_DMG_PIERCING, id, combat_state);
			// @todo replace random
			int random = rand() % 10000;
			if (random % 2 == 0) {
				// cause burn for 3 turns
				if (from_enemy) {
					apply_status_to_ally(target_index, SKILL_CONDITION_BURN, 3, combat_state);
				} else {
					apply_status_to_enemy(target_index, SKILL_CONDITION_BURN, 3, combat_state);
				}
			}
		}break;
		case SKILL_AUTOMATA_SUMMON: {
			skill_state->automata_summon_duration = 1 + 1;
			if (from_enemy) {
				skill_counter_enemy.automata_summon_norma_index = source_index;
				gain_absorption_enemy(15, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				skill_counter_ally.automata_summon_norma_index = source_index;
				gain_absorption_ally(15, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		}break;
		case SKILL_TURING_MACHINE: {
			// @todo check automata summon
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (is_targetable_by_skill(SKILL_TURING_MACHINE, i, combat_state))
					deal_damage_to_target(i, source_index, 30, SKILL_DMG_NORMAL, id, combat_state);
			}
		}break;

		// Hazard
		case SKILL_TMR: {
			int num_targets = 0;
			if (from_enemy) num_targets = NUM_ALLIES;
			else num_targets = NUM_ENEMIES;

			for (int i = 0; i < num_targets; ++i) {
				if (!is_targetable_by_skill(SKILL_TMR, i, combat_state))
					continue;

				s32 dmg = deal_damage_to_target(i, source_index, 15, SKILL_DMG_NORMAL, id, combat_state);
				if (dmg > 0) {
					// attack succeeded gain absorption
					if (from_enemy) {
						gain_absorption_enemy(5, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
					} else {
						gain_absorption_ally(5, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
					}
				}
			}
		}break;
		case SKILL_REDUNDANCY: {
			if (from_enemy) {
				for (int i = 0; i < NUM_ENEMIES; ++i) {
					if(is_targetable_by_skill(SKILL_REDUNDANCY, i, combat_state))
						gain_reduction_enemy(15, 3, false, i, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
				}
			} else {
				for (int i = 0; i < NUM_ALLIES; ++i) {
					if (is_targetable_by_skill(SKILL_REDUNDANCY, i, combat_state))
						gain_reduction_ally(15, 3, false, i, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
				}
			}
		}break;
		case SKILL_ROLLBACK: {
			u32 status = SKILL_CONDITION_NORMAL | SKILL_CONDITION_BURN | SKILL_CONDITION_FREEZE | SKILL_CONDITION_POISON |
				SKILL_CONDITION_PARALYZE | SKILL_CONDITION_SLEEP | SKILL_CONDITION_STUN;
			if (from_enemy) {
				remove_status_from_enemy(target_index, (Skill_Condition)status, combat_state);
			} else {
				remove_status_from_ally(target_index, (Skill_Condition)status, combat_state);
			}
		}break;

		// Qwerty
		case SKILL_ALT: {
			if (from_enemy) {
				gain_reduction_enemy(40, 4, false, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				gain_reduction_ally(40, 4, false, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		}break;
		case SKILL_CTRL: {
			// @todo UNIQUE HARD SKILL
		}break;
		case SKILL_DELETE: {
			// @todo check for control
			if (from_enemy) {
				combat_state->player.hp[target_index] = 0;
				layout_ally_die(target_index);
				layout_set_ally_hp(target_index, combat_state->player.max_hp[target_index], 0);
			} else {
				combat_state->enemy.hp[target_index] = 0;
				layout_enemy_die(target_index);
				layout_set_enemy_hp(target_index, combat_state->enemy.max_hp[target_index], 0);
			}
		}break;
		
		// Big O
		case SKILL_BEST_BOUND_FIST: {
			/*
			Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.
			Se o alvo já possui status BURN, este sofre 10 de dano crushing adicional.
			*/
			if (from_enemy) {
				int max = INT_MIN;
				int index = -1;
				for (int i = 0; i < NUM_ALLIES; ++i) {
					// @todo add a random chance if the hp is equal?
					if (combat_state->player.hp[i] > max) {
						max = combat_state->player.hp[i];
						index = i;
					}
				}
				deal_damage_to_target(index, source_index, 20, SKILL_DMG_NORMAL, id, combat_state);
				if (combat_state->player.status[index] & SKILL_CONDITION_BURN)
					deal_damage_to_target(index, source_index, 10, SKILL_DMG_CRUSHING, id, combat_state);

				apply_status_to_ally(index, SKILL_CONDITION_BURN, 2, combat_state);
			} else {
				int max = INT_MIN;
				int index = -1;
				for (int i = 0; i < NUM_ENEMIES; ++i) {
					// @todo add a random chance if the hp is equal?
					if (combat_state->enemy.hp[i] > max) {
						max = combat_state->enemy.hp[i];
						index = i;
					}
				}
				deal_damage_to_target(index, source_index, 20, SKILL_DMG_NORMAL, id, combat_state);
				if (combat_state->enemy.status[index] & SKILL_CONDITION_BURN)
					deal_damage_to_target(index, source_index, 10, SKILL_DMG_CRUSHING, id, combat_state);

				apply_status_to_enemy(index, SKILL_CONDITION_BURN, 2, combat_state);
			}
			
		}break;
		case SKILL_DUAL_SIMPLEX: {
			/*
			Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um 
			aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.
			*/
			if (from_enemy) {
				if (on_enemy) {
					apply_status_to_ally(target_index, SKILL_CONDITION_BURN, 3, combat_state);
				} else {
					s32 newhp = MIN(combat_state->enemy.hp[target_index] + 25, combat_state->enemy.max_hp[target_index]);
					combat_state->enemy.hp[target_index] = newhp;
					layout_set_enemy_hp(target_index, combat_state->enemy.max_hp[target_index], newhp);
					apply_status_to_enemy(target_index, SKILL_CONDITION_FREEZE, 2, combat_state);
				}
			} else {
				if (on_enemy) {
					apply_status_to_enemy(target_index, SKILL_CONDITION_BURN, 3, combat_state);
				} else {
					s32 newhp = MIN(combat_state->player.hp[target_index] + 25, combat_state->player.max_hp[target_index]);
					combat_state->player.hp[target_index] = newhp;
					layout_set_ally_hp(target_index, combat_state->player.max_hp[target_index], newhp);
					apply_status_to_ally(target_index, SKILL_CONDITION_FREEZE, 2, combat_state);
				}
			}
		}break;
		case SKILL_GRAPH_COLORING: {
			// No próximo turno, recebe 4 orbs de energia : uma de cada categoria.
			if(!from_enemy)
				skill_state_ally.graph_coloring = true;
		}break;

		// New
		case SKILL_SPRINT_BURST: {
			deal_damage_to_target(target_index, source_index, 25, SKILL_DMG_NORMAL, id, combat_state);
			if (from_enemy) {
				gain_reduction_enemy(10, 1, false, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				gain_reduction_ally(10, 1, false, source_index, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		}break;
		case SKILL_INHERITANCE: {
			if (from_enemy) {
				skill_counter_ally.inheritante_target = target_index;
				skill_counter_ally.inheritance_new_index = source_index;
			} else {
				skill_counter_enemy.inheritante_target = target_index;
				skill_counter_enemy.inheritance_new_index = source_index;
			}
		}break;
		case SKILL_OVERRIDE: {
			u32 status = SKILL_CONDITION_NORMAL | SKILL_CONDITION_BURN | SKILL_CONDITION_FREEZE | SKILL_CONDITION_POISON |
				SKILL_CONDITION_PARALYZE | SKILL_CONDITION_SLEEP | SKILL_CONDITION_STUN;
			if (from_enemy) {
				remove_status_from_enemy(target_index, (Skill_Condition)status, combat_state);
			} else {
				remove_status_from_ally(target_index, (Skill_Condition)status, combat_state);
			}
		}break;

		// Clockboy
		case SKILL_CLOCK_PULSE: {
			s32 dmg = 20;
			if (skill_state->overclock_duration > 0) {
				dmg *= 5;
			}
			deal_damage_to_target(target_index, source_index, dmg, SKILL_DMG_NORMAL, id, combat_state);
			if (from_enemy) {
				apply_status_to_ally(target_index, SKILL_CONDITION_PARALYZE, 2, combat_state);
			} else {
				apply_status_to_enemy(target_index, SKILL_CONDITION_PARALYZE, 2, combat_state);
			}
		}break;
		case SKILL_PIPELINE: {
			// @todo wierd shit
		}break;
		case SKILL_OVERCLOCK: {
			if (from_enemy) {
				int half_max_hp = combat_state->enemy.max_hp[source_index] / 2;
				combat_state->enemy.hp[source_index] = half_max_hp;
				layout_set_enemy_hp(source_index, combat_state->enemy.max_hp[source_index], half_max_hp);
				gain_invulnerability_enemy(source_index, 2, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				int half_max_hp = combat_state->player.max_hp[source_index] / 2;
				combat_state->player.hp[source_index] = half_max_hp;
				layout_set_ally_hp(source_index, combat_state->player.max_hp[source_index], half_max_hp);
				gain_invulnerability_ally(source_index, 2, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
			skill_state->overclock_duration = 2 + 1;
			// @todo make attacks cause 500% dmg
		}break;

		// Invulnerability skills
		case SKILL_BRANCH_DAMAGE:
		case SKILL_POLIMORPHISM:
		case SKILL_KNAPSACK_HIDEOUT:
		case SKILL_ESC:
		case SKILL_ROLLFORWARD:
		case SKILL_NON_DETERMINISM:
		case SKILL_FORK:
		case SKILL_HILL_CLIMBING:
		case SKILL_RASTERIZATION:
		case SKILL_ENCRYPTION:
		case SKILL_TRUE_ENDURANCE:
		case SKILL_VOID_BARRIER: {
			if (from_enemy) {
				gain_invulnerability_enemy(source_index, 1, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			} else {
				gain_invulnerability_ally(source_index, 1, SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL, combat_state);
			}
		} break;
	}
	return 0;
}

void update_skill_state_end_enemy_turn(Combat_State* combat_state) {
	if (skill_state_enemy.requiem_duration > 0)
		skill_state_enemy.requiem_duration -= 1;
	if (skill_state_enemy.axiom_one_duration > 0)
		skill_state_enemy.axiom_one_duration -= 1;
	if (skill_state_enemy.ddos_attack_duration > 0)
		skill_state_enemy.ddos_attack_duration -= 1;
	if (skill_state_enemy.paricle_rendering_duration > 0)
		skill_state_enemy.paricle_rendering_duration -= 1;
	if (skill_state_enemy.neural_network_duration > 0)
		skill_state_enemy.neural_network_duration -= 1;
	if (skill_state_enemy.automata_summon_duration > 0)
		skill_state_enemy.automata_summon_duration -= 1;
	if (skill_state_enemy.inheritance_duration > 0) {
		skill_state_enemy.inheritance_duration -= 1;
		if (skill_state_enemy.inheritance_duration == 0) {
			skill_state_enemy.inheritance_copy = SKILL_NONE;
		}
	}
	if (skill_state_enemy.overclock_duration > 0)
		skill_state_enemy.overclock_duration -= 1;

	for (int i = 0; i < NUM_ENEMIES; ++i) {
		for (int k = 0; k < SKILL_DEF_NUMBER; ++k) {
			if (combat_state->enemy.reduction_duration[i][k] > 0) {
				combat_state->enemy.reduction_duration[i][k]--;
				if (combat_state->enemy.reduction_duration[i][k] == 0) {
					combat_state->enemy.reduction[i] &= ~k;
					combat_state->enemy.reduction_points[i][k] = 0;
					combat_state->enemy.reduction_percentile[i][k] = false;
				}
			}
		}
	}

	skill_counter_enemy.tautology_target = -1;
	skill_counter_enemy.contradiction_target = -1;
	skill_counter_enemy.contradiction_zero_index = -1;
	skill_counter_enemy.inheritante_target = -1;
}

void update_skill_state_end_turn(Combat_State* combat_state) {
	if (skill_state_ally.requiem_duration > 0)
		skill_state_ally.requiem_duration -= 1;
	if (skill_state_ally.axiom_one_duration > 0)
		skill_state_ally.axiom_one_duration -= 1;
	if (skill_state_ally.ddos_attack_duration > 0)
		skill_state_ally.ddos_attack_duration -= 1;
	if (skill_state_ally.paricle_rendering_duration > 0)
		skill_state_ally.paricle_rendering_duration -= 1;
	if (skill_state_ally.neural_network_duration > 0)
		skill_state_ally.neural_network_duration -= 1;
	if (skill_state_ally.automata_summon_duration > 0)
		skill_state_ally.automata_summon_duration -= 1;
	if (skill_state_ally.inheritance_duration > 0) {
		skill_state_ally.inheritance_duration -= 1;
		if (skill_state_ally.inheritance_duration == 0) {
			skill_state_ally.inheritance_copy = SKILL_NONE;
			linked::Button* b = gw.allies_skills[skill_counter_enemy.inheritance_new_index * NUM_SKILLS + 1]->divs[0]->getButtons()[0];
			b->setAllBGTexture(skill_textures[SKILL_INHERITANCE]);
		}
	}
	if (skill_state_ally.overclock_duration > 0)
		skill_state_ally.overclock_duration -= 1;

	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int k = 0; k < SKILL_DEF_NUMBER; ++k) {
			if (combat_state->player.reduction_duration[i][k] > 0) {
				combat_state->player.reduction_duration[i][k]--;
				if (combat_state->player.reduction_duration[i][k] == 0) {
					combat_state->player.reduction[i] &= ~k;
					combat_state->player.reduction_points[i][k] = 0;
					combat_state->player.reduction_percentile[i][k] = false;
				}
			}
		}
	}
	skill_counter_ally.tautology_target = -1;
	skill_counter_ally.contradiction_target = -1;
	skill_counter_ally.contradiction_zero_index = -1;
	skill_counter_ally.inheritante_target = -1;
}

static void add_orb(Orb_ID orb_type, s32 count);

void update_status_end_enemy_turn(Combat_State* combat_state) {
	// Graph coloring here
	if (skill_state_ally.graph_coloring) {
		skill_state_ally.graph_coloring = false;
		/*
			ORB_HARD = 0,
			ORB_SOFT = 1,
			ORB_VR = 2,
			ORB_BIOS = 3,
		*/
		add_orb(ORB_HARD, 1);
		add_orb(ORB_SOFT, 1);
		add_orb(ORB_VR, 1);
		add_orb(ORB_BIOS, 1);
	}

	for (int i = 0; i < NUM_ENEMIES; ++i) {
		for (int j = SKILL_CONDITION_BURN; j < SKILL_CONDITION_NUMBER; j <<= 1) {
			if (combat_state->enemy.status_duration[i][j] > 0)
				combat_state->enemy.status_duration[i][j] -= 1;
			if (combat_state->enemy.status_duration[i][j] == 0) {
				remove_status_from_enemy(i, (Skill_Condition)j, combat_state);
			}
		}
	}
}

void update_status_end_turn(Combat_State* combat_state) {
	for (int i = 0; i < NUM_ALLIES; ++i) {
		for (int j = SKILL_CONDITION_BURN; j < SKILL_CONDITION_NUMBER; j <<= 1) {
			if (combat_state->player.status_duration[i][j] > 0)
				combat_state->player.status_duration[i][j] -= 1;
			if (combat_state->player.status_duration[i][j] == 0) {
				remove_status_from_ally(i, (Skill_Condition)j, combat_state);
			}
		}
	}
}