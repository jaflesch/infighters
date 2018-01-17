#include "AudioController.h"

Audio AudioController::introAudio(std::string("../../../res/music/intro.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::charselectAudio(std::string("../../../res/music/char-select.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::combat1Audio(std::string("../../../res/music/BIODRIVE-Neon-City.wav"), AudioType::MUSIC, AUDIO_DEFAULT_VOLUME);
Audio AudioController::navigationAudio(std::string("../../../res/music/navigation2.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::cancelAudio(std::string("../../../res/music/cancel.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::confirmAudio(std::string("../../../res/music/confirm.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

// Character skills
Audio AudioController::q_punch(std::string("../../../res/skills_sfx/astar/q-punch.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::neural_network(std::string("../../../res/skills_sfx/astar/neural-network.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::perceptron(std::string("../../../res/skills_sfx/astar/perceptron.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::hill_climbing(std::string("../../../res/skills_sfx/astar/hill-climbing.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::best_bount_fist(std::string("../../../res/skills_sfx/big_o/best-bound-fist.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::dual_simplex(std::string("../../../res/skills_sfx/big_o/dual-simplex.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::graph_coloring(std::string("../../../res/skills_sfx/big_o/graph-coloring.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::knapsack_hideout(std::string("../../../res/skills_sfx/big_o/knapsack-hideout.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::branch_prediction(std::string("../../../res/skills_sfx/clockboy/branch-prediction.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::clock_pulse(std::string("../../../res/skills_sfx/clockboy/clock-pulse.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::overclock(std::string("../../../res/skills_sfx/clockboy/overclock.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::pipeline(std::string("../../../res/skills_sfx/clockboy/overclock.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::fork(std::string("../../../res/skills_sfx/deadlock/fork.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::mutex(std::string("../../../res/skills_sfx/deadlock/mutex.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::preemption(std::string("../../../res/skills_sfx/deadlock/preemption.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::thread_scheduling(std::string("../../../res/skills_sfx/deadlock/thread-scheduling.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::redundancy(std::string("../../../res/skills_sfx/hazard/redundancy.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::rollback(std::string("../../../res/skills_sfx/hazard/rollback.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::rollforward(std::string("../../../res/skills_sfx/hazard/rollforward.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::tmr(std::string("../../../res/skills_sfx/hazard/tmr.flac"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::inheritance(std::string("../../../res/skills_sfx/new/inheritance.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::override_(std::string("../../../res/skills_sfx/new/override.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::polymorphism(std::string("../../../res/skills_sfx/new/polymorphism.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::sprint_burst(std::string("../../../res/skills_sfx/new/sprint-burst.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::automata_summon(std::string("../../../res/skills_sfx/norma/automata-summon.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::non_determinism(std::string("../../../res/skills_sfx/norma/non-determinism.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::pumping_up(std::string("../../../res/skills_sfx/norma/pumping-up.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::turing_machine(std::string("../../../res/skills_sfx/norma/turing-machine.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::axiom_one(std::string("../../../res/skills_sfx/one/axion-one.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::tautology(std::string("../../../res/skills_sfx/one/tautology.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::true_endurance(std::string("../../../res/skills_sfx/one/true-endurance.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::truth_slash(std::string("../../../res/skills_sfx/one/truth-slash.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::alt(std::string("../../../res/skills_sfx/qwerty/alt.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::ctrl(std::string("../../../res/skills_sfx/qwerty/ctrl.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::del(std::string("../../../res/skills_sfx/qwerty/del.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::esc(std::string("../../../res/skills_sfx/qwerty/esc.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::diffuse_reflection(std::string("../../../res/skills_sfx/ray_tracey/diffuse-reflection.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::dynamic_frustum(std::string("../../../res/skills_sfx/ray_tracey/dynamic-frustum.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::particle_rendering(std::string("../../../res/skills_sfx/ray_tracey/particle-rendering.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::rasterization(std::string("../../../res/skills_sfx/ray_tracey/rasterization.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::brute_force(std::string("../../../res/skills_sfx/serial_keyller/brute-force.flac"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::buffer_overflow(std::string("../../../res/skills_sfx/serial_keyller/buffer-overflow.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::ddos_attack(std::string("../../../res/skills_sfx/serial_keyller/ddos-attack.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::encryption(std::string("../../../res/skills_sfx/serial_keyller/encryption.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

Audio AudioController::contradiction(std::string("../../../res/skills_sfx/zero/contradiction.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::false_rush(std::string("../../../res/skills_sfx/zero/false-rush.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::void_barrier(std::string("../../../res/skills_sfx/zero/void-barrier.ogg"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);
Audio AudioController::requiem_zero(std::string("../../../res/skills_sfx/zero/zero-requiem.wav"), AudioType::SOUND, AUDIO_DEFAULT_VOLUME);

unsigned int AudioController::musicVolume = AUDIO_DEFAULT_VOLUME;
unsigned int AudioController::soundVolume = AUDIO_DEFAULT_VOLUME;

void AudioController::setMusicVolume(unsigned int volume)
{
	AudioController::introAudio.setVolume(volume);
	AudioController::charselectAudio.setVolume(volume);
	AudioController::combat1Audio.setVolume(volume);
	AudioController::musicVolume = volume;
}

void AudioController::pauseAllMusic() {
	AudioController::introAudio.pause();
	AudioController::charselectAudio.pause();
	AudioController::combat1Audio.pause();
}

unsigned int AudioController::getMusicVolume()
{
	return AudioController::musicVolume;
}

void AudioController::setSoundVolume(unsigned int volume)
{
	// REMEMBER TO REFRESH WHEN ADDING NEW SOUNDS !
	//AudioController::themeAudio.setVolume(volume);
	AudioController::navigationAudio.setVolume(volume);
	AudioController::cancelAudio.setVolume(volume);
	AudioController::confirmAudio.setVolume(volume);
}

unsigned int AudioController::getSoundVolume()
{
	return AudioController::soundVolume;
}