#pragma once
#include "Audio.h"

class AudioController
{
public:
	static void setMusicVolume(unsigned int volume);
	static unsigned int getMusicVolume();
	static void setSoundVolume(unsigned int volume);
	static unsigned int getSoundVolume();
	static void pauseAllMusic();
	
	/* GAME AUDIO */
	static Audio combat1Audio;
	static Audio introAudio;
	static Audio charselectAudio;
	static Audio navigationAudio;
	static Audio cancelAudio;
	static Audio confirmAudio;

	// Astar
	static Audio q_punch;
	static Audio neural_network;
	static Audio hill_climbing;
	static Audio perceptron;
	// BigO
	static Audio best_bount_fist;
	static Audio dual_simplex;
	static Audio graph_coloring;
	static Audio knapsack_hideout;
	// Clockboy
	static Audio branch_prediction;
	static Audio clock_pulse;
	static Audio overclock;
	static Audio pipeline;
	// Deadlock
	static Audio fork;
	static Audio mutex;
	static Audio preemption;
	static Audio thread_scheduling;
	// Hazard
	static Audio redundancy;
	static Audio rollback;
	static Audio rollforward;
	static Audio tmr;
	// New
	static Audio inheritance;
	static Audio override_;
	static Audio polymorphism;
	static Audio sprint_burst;
	// Norma
	static Audio automata_summon;
	static Audio non_determinism;
	static Audio pumping_up;
	static Audio turing_machine;
	// One
	static Audio axiom_one;
	static Audio tautology;
	static Audio true_endurance;
	static Audio truth_slash;
	// Qwerty
	static Audio alt;
	static Audio ctrl;
	static Audio del;
	static Audio esc;
	// Ray Tracey
	static Audio diffuse_reflection;
	static Audio dynamic_frustum;
	static Audio particle_rendering;
	static Audio rasterization;
	// Serial Keyller
	static Audio brute_force;
	static Audio buffer_overflow;
	static Audio ddos_attack;
	static Audio encryption;
	// Zero
	static Audio contradiction;
	static Audio false_rush;
	static Audio requiem_zero;
	static Audio void_barrier;
private:
	static unsigned int musicVolume;
	static unsigned int soundVolume;
};