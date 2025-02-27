#pragma once
#include "dcon_generated.hpp"
#include "common_types.hpp"
#include "events.hpp"
#include "diplomatic_messages.hpp"

namespace command {

enum class command_type : uint8_t {
	invalid = 0,
	change_nat_focus = 1,
	start_research = 2,
	make_leader = 3,
	begin_province_building_construction = 4,
	increase_relations = 5,
	decrease_relations = 6,
	begin_factory_building_construction = 7,
	begin_unit_construction = 8,
	cancel_unit_construction = 9,
	change_factory_settings = 10,
	delete_factory = 11,
	make_vassal = 12,
	release_and_play_nation = 13,
	war_subsidies = 14,
	cancel_war_subsidies = 15,
	change_budget = 16,
	start_election = 17,
	change_influence_priority = 18,
	discredit_advisors = 19,
	expel_advisors = 20,
	ban_embassy = 21,
	increase_opinion = 22,
	decrease_opinion = 23,
	add_to_sphere = 24,
	remove_from_sphere = 25,
	upgrade_colony_to_state = 26,
	invest_in_colony = 27,
	abandon_colony = 28,
	finish_colonization = 29,
	intervene_in_war = 30,
	suppress_movement = 31,
	civilize_nation = 32,
	appoint_ruling_party = 33,
	change_issue_option = 34,
	change_reform_option = 35,
	become_interested_in_crisis = 36,
	take_sides_in_crisis = 37,
	// back_crisis_acceptance = 38, -- rolled these into respond_to_diplomatic_message
	// back_crisis_decline = 39,
	change_stockpile_settings = 40,
	take_decision = 41,
	make_n_event_choice = 42,
	make_f_n_event_choice = 43,
	make_p_event_choice = 44,
	make_f_p_event_choice = 45,
	fabricate_cb = 46,
	cancel_cb_fabrication = 47,
	ask_for_military_access = 48,
	ask_for_alliance = 49,
	call_to_arms = 50,
	respond_to_diplomatic_message = 51,
	cancel_military_access = 52,
	cancel_alliance = 53,
	cancel_given_military_access = 54,
};

struct national_focus_data {
	dcon::state_instance_id target_state;
	dcon::national_focus_id focus;
};

struct start_research_data {
	dcon::technology_id tech;
};

struct make_leader_data {
	bool is_general;
};

struct province_building_data {
	dcon::province_id location;
	economy::province_building_type type;
};

struct factory_building_data {
	dcon::state_instance_id location;
	dcon::factory_type_id type;
	bool is_upgrade;
};

struct diplo_action_data {
	dcon::nation_id target;
};

struct unit_construction_data {
	dcon::province_id location;
	dcon::unit_type_id type;
};

struct factory_data {
	dcon::province_id location;
	dcon::factory_type_id type;
	uint8_t priority;
	bool subsidize;
};

struct tag_target_data {
	dcon::national_identity_id ident;
};

struct influence_action_data {
	dcon::nation_id influence_target;
	dcon::nation_id gp_target;
};

struct influence_priority_data {
	dcon::nation_id influence_target;
	uint8_t priority;
};

struct generic_location_data {
	dcon::province_id prov;
};

struct movement_data {
	dcon::issue_option_id iopt;
	dcon::national_identity_id tag;
};

struct political_party_data {
	dcon::political_party_id p;
};

struct reform_selection_data {
	dcon::reform_option_id r;
};

struct issue_selection_data {
	dcon::issue_option_id r;
};

struct budget_settings_data {
	int8_t education_spending;
	int8_t military_spending;
	int8_t administrative_spending;
	int8_t social_spending;
	int8_t land_spending;
	int8_t naval_spending;
	int8_t construction_spending;
	int8_t poor_tax;
	int8_t middle_tax;
	int8_t rich_tax;
	int8_t tariffs;
};

struct war_target_data {
	dcon::war_id war;
	bool for_attacker;
};

struct crisis_join_data {
	bool join_attackers;
};

struct stockpile_settings_data {
	float amount;
	dcon::commodity_id c;
	bool draw_on_stockpiles;
};

struct decision_data {
	dcon::decision_id d;
};

struct message_data {
	dcon::nation_id from;
	diplomatic_message::type type;
	bool accept;
};

struct call_to_arms_data {
	dcon::nation_id target;
	dcon::war_id war;
};

struct pending_human_n_event_data {
	uint32_t r_lo = 0;
	uint32_t r_hi = 0;
	int32_t primary_slot;
	int32_t from_slot;
	dcon::national_event_id e;
	sys::date date;
	uint8_t opt_choice;
};
struct pending_human_f_n_event_data {
	uint32_t r_lo = 0;
	uint32_t r_hi = 0;
	dcon::free_national_event_id e;
	sys::date date;
	uint8_t opt_choice;
};
struct pending_human_p_event_data {
	uint32_t r_lo = 0;
	uint32_t r_hi = 0;
	int32_t from_slot;
	dcon::provincial_event_id e;
	dcon::province_id p;
	sys::date date;
	uint8_t opt_choice;
};
struct pending_human_f_p_event_data {
	uint32_t r_lo = 0;
	uint32_t r_hi = 0;
	dcon::free_provincial_event_id e;
	dcon::province_id p;
	sys::date date;
	uint8_t opt_choice;
};

struct cb_fabrication_data {
	dcon::nation_id target;
	dcon::cb_type_id type;
};

struct payload {
	union dtype {
		national_focus_data nat_focus;
		start_research_data start_research;
		make_leader_data make_leader;
		province_building_data start_province_building;
		diplo_action_data diplo_action;
		factory_building_data start_factory_building;
		unit_construction_data unit_construction;
		tag_target_data tag_target;
		factory_data factory;
		budget_settings_data budget_data;
		influence_action_data influence_action;
		influence_priority_data influence_priority;
		generic_location_data generic_location;
		war_target_data war_target;
		movement_data movement;
		political_party_data political_party;
		reform_selection_data reform_selection;
		issue_selection_data issue_selection;
		crisis_join_data crisis_join;
		stockpile_settings_data stockpile_settings;
		decision_data decision;
		pending_human_n_event_data pending_human_n_event;
		pending_human_f_n_event_data pending_human_f_n_event;
		pending_human_p_event_data pending_human_p_event;
		pending_human_f_p_event_data pending_human_f_p_event;
		cb_fabrication_data cb_fabrication;
		message_data message;
		call_to_arms_data call_to_arms;

		dtype() {}
	} data;
	dcon::nation_id source;
	command_type type = command_type::invalid;

	payload() {}
};

void set_national_focus(sys::state& state, dcon::nation_id source, dcon::state_instance_id target_state, dcon::national_focus_id focus);
bool can_set_national_focus(sys::state& state, dcon::nation_id source, dcon::state_instance_id target_state, dcon::national_focus_id focus);

void start_research(sys::state& state, dcon::nation_id source, dcon::technology_id tech);
bool can_start_research(sys::state& state, dcon::nation_id source, dcon::technology_id tech);

void make_leader(sys::state& state, dcon::nation_id source, bool general);
bool can_make_leader(sys::state& state, dcon::nation_id source, bool general);

void decrease_relations(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_decrease_relations(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void begin_province_building_construction(sys::state& state, dcon::nation_id source, dcon::province_id p, economy::province_building_type type);
bool can_begin_province_building_construction(sys::state& state, dcon::nation_id source, dcon::province_id p, economy::province_building_type type);

void begin_factory_building_construction(sys::state& state, dcon::nation_id source, dcon::state_instance_id location, dcon::factory_type_id type, bool is_upgrade);
bool can_begin_factory_building_construction(sys::state& state, dcon::nation_id source, dcon::state_instance_id location, dcon::factory_type_id type, bool is_upgrade);

void start_unit_construction(sys::state& state, dcon::nation_id source, dcon::province_id location, dcon::unit_type_id type);
bool can_start_unit_construction(sys::state& state, dcon::nation_id source, dcon::province_id location, dcon::unit_type_id type);

void cancel_unit_construction(sys::state& state, dcon::nation_id source, dcon::province_id location, dcon::unit_type_id type);
bool can_cancel_unit_construction(sys::state& state, dcon::nation_id source, dcon::province_id location, dcon::unit_type_id type);

void delete_factory(sys::state& state, dcon::nation_id source, dcon::factory_id f);
bool can_delete_factory(sys::state& state, dcon::nation_id source, dcon::factory_id f);

void change_factory_settings(sys::state& state, dcon::nation_id source, dcon::factory_id f, uint8_t priority, bool subsidized);
bool can_change_factory_settings(sys::state& state, dcon::nation_id source, dcon::factory_id f, uint8_t priority, bool subsidized);

void make_vassal(sys::state& state, dcon::nation_id source, dcon::national_identity_id t);
bool can_make_vassal(sys::state& state, dcon::nation_id source, dcon::national_identity_id t);

void release_and_play_as(sys::state& state, dcon::nation_id source, dcon::national_identity_id t);
bool can_release_and_play_as(sys::state& state, dcon::nation_id source, dcon::national_identity_id t);

void give_war_subsidies(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_give_war_subsidies(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void cancel_war_subsidies(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_cancel_war_subsidies(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void increase_relations(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_increase_relations(sys::state& state, dcon::nation_id source, dcon::nation_id target);

inline budget_settings_data make_empty_budget_settings() {
	return budget_settings_data{ int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127), int8_t(-127) };
}
// when sending new budget settings, leaving any value as int8_t(-127) will cause it to be ignored, leaving the setting the same
// You can use the function above to easily make an instance of the settings struct that will change no values
// Also, in consideration for future networking performance, do not send this command as the slider moves; only send it when the
// player has stopped dragging the slider, in the case of drag, or maybe even only when the window closes / a day passes while the window
// is open, if you think we can get away with it. In any case, we want to try to minimize how many times the command is sent per
// average interaction with the budget.
void change_budget_settings(sys::state& state, dcon::nation_id source, budget_settings_data const& values);
inline bool can_change_budget_settings(sys::state& state, dcon::nation_id source, budget_settings_data const& values) {
	return true;
}

void start_election(sys::state& state, dcon::nation_id source);
bool can_start_election(sys::state& state, dcon::nation_id source);

void change_influence_priority(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, uint8_t priority);
bool can_change_influence_priority(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, uint8_t priority);

void discredit_advisors(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);	// Implemented in GUI :3
bool can_discredit_advisors(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);

void expel_advisors(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);		// Implemented in GUI :3
bool can_expel_advisors(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);

void ban_embassy(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);		// Implemented in GUI :3
bool can_ban_embassy(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);

void increase_opinion(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target);					// Implemented in GUI :3
bool can_increase_opinion(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target);

void decrease_opinion(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);	// Implemented in GUI :3
bool can_decrease_opinion(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);

void add_to_sphere(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target);					// Implemented in GUI :3
bool can_add_to_sphere(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target);

void remove_from_sphere(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);	// Implemented in GUI :3
bool can_remove_from_sphere(sys::state& state, dcon::nation_id source, dcon::nation_id influence_target, dcon::nation_id affected_gp);

void upgrade_colony_to_state(sys::state& state, dcon::nation_id source, dcon::state_instance_id si);		// Implemented in GUI
bool can_upgrade_colony_to_state(sys::state& state, dcon::nation_id source, dcon::state_instance_id si);

void invest_in_colony(sys::state& state, dcon::nation_id source, dcon::province_id p);
bool can_invest_in_colony(sys::state& state, dcon::nation_id source, dcon::province_id p);

void abandon_colony(sys::state& state, dcon::nation_id source, dcon::province_id p);				// Added in GUI Need QA
bool can_abandon_colony(sys::state& state, dcon::nation_id source, dcon::province_id p);

void finish_colonization(sys::state& state, dcon::nation_id source, dcon::province_id p);
bool can_finish_colonization(sys::state& state, dcon::nation_id source, dcon::province_id p);

void intervene_in_war(sys::state& state, dcon::nation_id source, dcon::war_id w, bool for_attacker);		// Implemented in GUI
bool can_intervene_in_war(sys::state& state, dcon::nation_id source, dcon::war_id w, bool for_attacker);

void suppress_movement(sys::state& state, dcon::nation_id source, dcon::movement_id m);				// Implemented in GUI
bool can_suppress_movement(sys::state& state, dcon::nation_id source, dcon::movement_id m);

void civilize_nation(sys::state& state, dcon::nation_id source);						// Implemented in GUI
bool can_civilize_nation(sys::state& state, dcon::nation_id source);

void appoint_ruling_party(sys::state& state, dcon::nation_id source, dcon::political_party_id p);		// Added in GUI
bool can_appoint_ruling_party(sys::state& state, dcon::nation_id source, dcon::political_party_id p);

void enact_reform(sys::state& state, dcon::nation_id source, dcon::reform_option_id r);				// Added in GUI Need QA
bool can_enact_reform(sys::state& state, dcon::nation_id source, dcon::reform_option_id r);

void enact_issue(sys::state& state, dcon::nation_id source, dcon::issue_option_id i);				// Added in GUI Need QA
bool can_enact_issue(sys::state& state, dcon::nation_id source, dcon::issue_option_id i);

void become_interested_in_crisis(sys::state& state, dcon::nation_id source);
bool can_become_interested_in_crisis(sys::state& state, dcon::nation_id source);

void take_sides_in_crisis(sys::state& state, dcon::nation_id source, bool join_attacker);
bool can_take_sides_in_crisis(sys::state& state, dcon::nation_id source, bool join_attacker);

void change_stockpile_settings(sys::state& state, dcon::nation_id source, dcon::commodity_id c, float target_amount, bool draw_on_stockpiles);
bool can_change_stockpile_settings(sys::state& state, dcon::nation_id source, dcon::commodity_id c, float target_amount, bool draw_on_stockpiles) {
	return true;
}

void take_decision(sys::state& state, dcon::nation_id source, dcon::decision_id d);
bool can_take_decision(sys::state& state, dcon::nation_id source, dcon::decision_id d);

void make_event_choice(sys::state& state, event::pending_human_n_event const& e, uint8_t option_id);
void make_event_choice(sys::state& state, event::pending_human_f_n_event const& e, uint8_t option_id);
void make_event_choice(sys::state& state, event::pending_human_p_event const& e, uint8_t option_id);
void make_event_choice(sys::state& state, event::pending_human_f_p_event const& e, uint8_t option_id);

void fabricate_cb(sys::state& state, dcon::nation_id source, dcon::nation_id target, dcon::cb_type_id type);
bool can_fabricate_cb(sys::state& state, dcon::nation_id source, dcon::nation_id target, dcon::cb_type_id type);

void cancel_cb_fabrication(sys::state& state, dcon::nation_id source);
bool can_cancel_cb_fabrication(sys::state& state, dcon::nation_id source) {
	return true;
}

void ask_for_military_access(sys::state& state, dcon::nation_id asker, dcon::nation_id target);
bool can_ask_for_access(sys::state& state, dcon::nation_id asker, dcon::nation_id target);

void ask_for_alliance(sys::state& state, dcon::nation_id asker, dcon::nation_id target);
bool can_ask_for_alliance(sys::state& state, dcon::nation_id asker, dcon::nation_id target);

void call_to_arms(sys::state& state, dcon::nation_id asker, dcon::nation_id target, dcon::war_id w);
bool can_call_to_arms(sys::state& state, dcon::nation_id asker, dcon::nation_id target, dcon::war_id w);

void respond_to_diplomatic_message(sys::state& state, dcon::nation_id source, dcon::nation_id from, diplomatic_message::type type, bool accept);

void cancel_military_access(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_cancel_military_access(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void cancel_alliance(sys::state& state, dcon::nation_id source, dcon::nation_id target);
bool can_cancel_alliance(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void cancel_given_military_access(sys::state& state, dcon::nation_id source, dcon::nation_id target); // this is for cancelling the access someone has with you
bool can_cancel_given_military_access(sys::state& state, dcon::nation_id source, dcon::nation_id target);

void execute_pending_commands(sys::state& state);

}
