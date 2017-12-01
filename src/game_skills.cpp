
struct Skill_State {
	s32 requiem_duration;
	s32 axiom_one_duration;
	s32 ddos_attack_duration;
	s32 paricle_rendering_duration;
	s32 neural_network_duration;
	s32 tautology_active_target = -1;
};

static Skill_State skill_state = {  };

void deal_damage_to_target_enemy(int target_index, int damage, Skill_Damage dmg_type, Combat_State* combat_state) {
	combat_state->enemy.hp[target_index] = MAX(0, combat_state->enemy.hp[target_index] - damage);
	layout_set_enemy_hp(target_index, combat_state->enemy.max_hp[target_index], combat_state->enemy.hp[target_index]);
	if (combat_state->enemy.hp[target_index] == 0) {
		layout_enemy_die(target_index);
	}
}

s32 execute_skill(Skill_ID id, int target_index, int source_index, Combat_State* combat_state) {
	switch (id) {
		// Zero
		case SKILL_FALSE_RUSH: {
			// if requiem is still active, affect all enemies (AoE)
			if (skill_state.requiem_duration > 0) {
				for (int i = 0; i < NUM_ENEMIES; ++i)
					deal_damage_to_target_enemy(i, 20, SKILL_DMG_NORMAL, combat_state);
			} else {
				// Normal behavior
				deal_damage_to_target_enemy(target_index, 20, SKILL_DMG_NORMAL, combat_state);
			}
		}break;
		case SKILL_CONTRADICTION: {
			// @todo
		}break;
		case SKILL_REQUIEM_ZERO: {
			skill_state.requiem_duration = 3;
		}break;

		// One
		case SKILL_TRUTH_SLASH: {
			// @todo account for reductions
			deal_damage_to_target_enemy(target_index, 30, SKILL_DMG_NORMAL, combat_state);
		}break;
		case SKILL_TAUTOLOGY: {
			// @todo account for reductions
			deal_damage_to_target_enemy(target_index, 15, SKILL_DMG_NORMAL, combat_state);
			skill_state.tautology_active_target = target_index;
		}break;

		// Serial Keyller
		case SKILL_BRUTE_FORCE: {
			int damage = 20;
			if (combat_state->enemy.reduction[target_index] == SKILL_DEF_INVULNERABILITY)
				damage = 10;
			deal_damage_to_target_enemy(target_index, damage, SKILL_DMG_NORMAL, combat_state);	// @todo when invul do 10
		}break;
		case SKILL_BUFFER_OVERFLOW: {
			deal_damage_to_target_enemy(target_index, 15, SKILL_DMG_PIERCING, combat_state);
			// @todo account for counters?
			combat_state->enemy.status[target_index] |= SKILL_CONDITION_STUN;
			combat_state->enemy.status_duration[target_index][SKILL_CONDITION_STUN] = 1;
		}break;
		case SKILL_DDOS_ATTACK: {
			// @todo account for counters?
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				combat_state->enemy.status[i] |= SKILL_CONDITION_PARALYZE;
				combat_state->enemy.status_duration[i][SKILL_CONDITION_PARALYZE] = 3;
			}
		}break;

		// Ray Tracey
		case SKILL_PARTICLE_RENDERING: {
			deal_damage_to_target_enemy(target_index, 15, SKILL_DMG_NORMAL, combat_state);
			combat_state->player.reduction[source_index] = SKILL_DEF_INVULNERABILITY;
			combat_state->player.reduction_type[target_index] = SKILL_TYPE_PHYSICAL;
			combat_state->player.reduction_duration[target_index][SKILL_DEF_INVULNERABILITY] = 1;
		}break;
		case SKILL_DIFFUSE_REFLECTION: {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				combat_state->player.reduction[i] = SKILL_DEF_RELECTION;
				combat_state->player.reduction_type[i] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
				combat_state->player.reduction_duration[i][SKILL_DEF_RELECTION] = 1;
			}
		}break;
		case SKILL_DYNAMIC_FRUSTUM_ATTACK: {
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				deal_damage_to_target_enemy(i, 35, SKILL_DMG_NORMAL, combat_state);
			}
		}break;

		// A-Star
		case SKILL_Q_PUNCH: {
			// @todo add check for neural network
			int damage = 20;
			int cumulative_damage = 5;
			int num_accumulated = combat_state->enemy.cumulative_skill[target_index][SKILL_Q_PUNCH];
			cumulative_damage *= num_accumulated;
			deal_damage_to_target_enemy(target_index, damage + cumulative_damage, SKILL_DMG_NORMAL, combat_state);
			combat_state->enemy.cumulative_skill[target_index][SKILL_Q_PUNCH] += 1;
		}break;
		case SKILL_PERCEPTRON: {
			deal_damage_to_target_enemy(target_index, 25, SKILL_DMG_CRUSHING, combat_state);
		}break;
		case SKILL_NEURAL_NETWORK: {
			skill_state.neural_network_duration = 4;

			combat_state->player.reduction[source_index] = SKILL_DEF_ABSORPTION;
			combat_state->player.reduction_type[source_index] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
			combat_state->player.reduction_points[source_index][SKILL_DMG_NORMAL] = 30;	// @ ask absorption is normal dmg?
		}break;
			
		// Deadlock
		case SKILL_PREEMPTION: {
			// @ ask wtf is CONTROL
			// Caso ele tenha alguma habilidade com duração CONTROL, elimina a habilidade e este sofre status SLEEP por 2 turnos.
			deal_damage_to_target_enemy(target_index, 25, SKILL_DMG_NORMAL, combat_state);
		}break;
		case SKILL_MUTEX: {
			// @todo UNIQUE
			// Atinge todos os adversários e, no próximo turno, apenas um pode utilizar alguma habilidade.
		}break;
		case SKILL_THREAD_SCHEDULING: {
			// @todo replace random number
			int random = rand() % (NUM_ENEMIES + 1);
			combat_state->enemy.status[random] |= SKILL_CONDITION_PARALYZE;
			combat_state->enemy.status_duration[random][SKILL_CONDITION_PARALYZE] = 3;
		}break;

		// Norma
		case SKILL_PUMPING_UP: {
			deal_damage_to_target_enemy(target_index, 25, SKILL_DMG_PIERCING, combat_state);
			// @todo replace random
			int random = rand() % 10000;
			if (random % 2 == 0) {
				// cause burn for 3 turns
				combat_state->enemy.status[target_index] |= SKILL_CONDITION_BURN;
				combat_state->enemy.status_duration[target_index][SKILL_CONDITION_BURN] = 3;
			}
		}break;
		case SKILL_AUTOMATA_SUMMON: {
			// @todo
		}break;
		case SKILL_TURING_MACHINE: {
			// @todo check automata summon
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				deal_damage_to_target_enemy(i, 30, SKILL_DMG_NORMAL, combat_state);
			}
		}break;

		// Hazard
		case SKILL_TMR: {
			// @todo cause 5 to each attack that succeeded
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				deal_damage_to_target_enemy(target_index, 15, SKILL_DMG_NORMAL, combat_state);
			}
		}break;
		case SKILL_REDUNDANCY: {
			for (int i = 0; i < NUM_ALLIES; ++i) {
				combat_state->player.reduction[i] = SKILL_DEF_REDUCTION;
				combat_state->player.reduction_type[i] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
				combat_state->player.reduction_points[i][SKILL_DEF_REDUCTION] = 15;
				combat_state->player.reduction_duration[i][SKILL_DEF_REDUCTION] = 3;
			}
		}break;
		case SKILL_ROLLBACK: {
			// @ todo
		}break;

		// Qwerty
		case SKILL_ALT: {
			combat_state->player.reduction[source_index] = SKILL_DEF_REDUCTION;
			combat_state->player.reduction_type[source_index] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
			combat_state->player.reduction_points[source_index][SKILL_DEF_REDUCTION] = 40;
			combat_state->player.reduction_duration[source_index][SKILL_DEF_REDUCTION] = 4;
		}break;
		case SKILL_CTRL: {
			// @todo UNIQUE HARD SKILL
		}break;
		case SKILL_DELETE: {
			// @todo check for control
			combat_state->enemy.hp[target_index] = 0;
			layout_enemy_die(target_index);
			layout_set_enemy_hp(target_index, combat_state->enemy.max_hp[target_index], 0);
		}break;
		
		// Big O
		case SKILL_BEST_BOUND_FIRST: {
			/*
			Ataca o oponente com menor HP e realiza 20 de dano. Por 2 turnos, o alvo fica com status BURN.
			Se o alvo já possui status BURN, este sofre 10 de dano crushing adicional.
			*/
			int max = INT_MIN;
			int index = -1;
			for (int i = 0; i < NUM_ENEMIES; ++i) {
				if (combat_state->enemy.hp[i] > max) {
					max = combat_state->enemy.hp[i];
					index = i;
				}
			}
			deal_damage_to_target_enemy(target_index, 20, SKILL_DMG_NORMAL, combat_state);
			if(combat_state->enemy.status[index] & SKILL_CONDITION_BURN)
				deal_damage_to_target_enemy(target_index, 10, SKILL_DMG_CRUSHING, combat_state);

			combat_state->enemy.status[index] |= SKILL_CONDITION_BURN;
			combat_state->enemy.status_duration[index][SKILL_CONDITION_BURN] = 2;
		}break;
		case SKILL_DUAL_SIMPLEX: {
			/*
			Se utilizado em um adversário, gera status BURN por 3 turnos. Se, em um 
			aliado, recupera 25 de energia e atribui status FROZEN por 2 turnos.
			*/
			// @todo target
		}break;
		case SKILL_GRAPH_COLORING: {
			// No próximo turno, recebe 4 orbs de energia : uma de cada categoria.
			// @todo complex
		}break;

		// New
		case SKILL_SPRINT_BURST: {
			deal_damage_to_target_enemy(target_index, 25, SKILL_DMG_NORMAL, combat_state);
			combat_state->player.reduction[source_index] = SKILL_DEF_REDUCTION;
			combat_state->player.reduction_type[source_index] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
			combat_state->player.reduction_points[source_index][SKILL_DEF_REDUCTION] = 10;
			combat_state->player.reduction_duration[source_index][SKILL_DEF_REDUCTION] = 1;
		}break;
		case SKILL_INHERITANCE: {
			// @todo HARD SHIT
		}break;
		case SKILL_OVERRIDE: {
			// @todo
		}break;

		// Clockboy
		case SKILL_CLOCK_PULSE: {
			deal_damage_to_target_enemy(target_index, 20, SKILL_DMG_NORMAL, combat_state);
			combat_state->enemy.status[target_index] |= SKILL_CONDITION_PARALYZE;
			combat_state->enemy.status_duration[target_index][SKILL_CONDITION_BURN] = 2;
		}break;
		case SKILL_PIPELINE: {
			// @todo wierd shit
		}break;
		case SKILL_OVERCLOCK: {
			combat_state->player.reduction[source_index] = SKILL_DEF_INVULNERABILITY;
			combat_state->player.reduction_type[source_index] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
			combat_state->player.reduction_duration[source_index][SKILL_DEF_INVULNERABILITY] = 2;
			int half_max_hp = combat_state->player.max_hp[source_index] / 2;
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
			combat_state->player.reduction[source_index] = SKILL_DEF_INVULNERABILITY;
			combat_state->player.reduction_type[source_index] = SKILL_TYPE_MENTAL | SKILL_TYPE_PHYSICAL | SKILL_TYPE_VIRTUAL;
			combat_state->player.reduction_duration[source_index][SKILL_DEF_INVULNERABILITY] = 1;
		} break;
	}
	return 0;
}

void update_skill_state(Combat_State* combat_state) {
	if(skill_state.requiem_duration > 0)
		skill_state.requiem_duration -= 1;
	if(skill_state.axiom_one_duration > 0)
		skill_state.axiom_one_duration -= 1;
	if (skill_state.ddos_attack_duration > 0)
		skill_state.ddos_attack_duration -= 1;
	if (skill_state.paricle_rendering_duration > 0)
		skill_state.paricle_rendering_duration -= 1;
	if (skill_state.neural_network_duration > 0)
		skill_state.neural_network_duration -= 1;

	skill_state.tautology_active_target = -1;
}
