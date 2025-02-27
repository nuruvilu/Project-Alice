#include "parsers_declarations.hpp"
#include "military.hpp"

namespace parsers {

void register_cb_type(std::string_view name, token_generator& gen, error_handler& err, scenario_building_context& context) {
	auto existing_it = context.map_of_cb_types.find(std::string(name));

	auto id = [&]() {
		if(existing_it != context.map_of_cb_types.end()) {
			existing_it->second.generator_state = gen;
			return existing_it->second.id;
		}
		auto new_id = context.state.world.create_cb_type();
		context.map_of_cb_types.insert_or_assign(std::string(name), pending_cb_content{ gen, new_id });
		return new_id;
	}();

	auto name_id = text::find_or_add_key(context.state, name);
	auto name_desc = std::string(name) + "_desc";
	auto name_setup = std::string(name) + "_setup";
	context.state.world.cb_type_set_name(id, name_id);
	context.state.world.cb_type_set_short_desc(id, text::find_or_add_key(context.state, name_desc));
	context.state.world.cb_type_set_long_desc(id, text::find_or_add_key(context.state, name_setup));

	uint32_t special_flags = 0;
	if(is_fixed_token_ci(name.data(), name.data() + name.length(), "free_peoples"))
		special_flags |= military::cb_flag::po_liberate;
	else if(is_fixed_token_ci(name.data(), name.data() + name.length(), "liberate_country"))
		special_flags |= military::cb_flag::po_liberate;
	else if(is_fixed_token_ci(name.data(), name.data() + name.length(), "take_from_sphere"))
		special_flags |= military::cb_flag::po_take_from_sphere;

	context.state.world.cb_type_set_type_bits(id, special_flags);

	gen.discard_group();
}

void make_trait(std::string_view name, token_generator& gen, error_handler& err, scenario_building_context& context) {

	dcon::leader_trait_id new_id = context.state.world.create_leader_trait();
	auto name_id = text::find_or_add_key(context.state, name);

	context.state.world.leader_trait_set_name(new_id, name_id);
	context.map_of_leader_traits.insert_or_assign(std::string(name), new_id);

	trait_context new_context{ context , new_id };
	parse_trait(gen, err, new_context);
}
void personality_traits_set(token_generator& gen, error_handler& err, scenario_building_context& context) {
	parse_traits_set(gen, err, context);
}
void background_traits_set(token_generator& gen, error_handler& err, scenario_building_context& context) {
	context.state.military_definitions.first_background_trait = dcon::leader_trait_id(dcon::leader_trait_id::value_base_t(context.state.world.leader_trait_size()));
	parse_traits_set(gen, err, context);
}

void make_base_units(scenario_building_context& context) {
	{
		dcon::unit_type_id army_base_id = dcon::unit_type_id(dcon::unit_type_id::value_base_t(context.state.military_definitions.unit_base_definitions.size()));
		context.state.military_definitions.unit_base_definitions.emplace_back();

		auto name_id = text::find_or_add_key(context.state, "army_base");
		context.state.military_definitions.unit_base_definitions.back().name = name_id;
		context.state.military_definitions.unit_base_definitions.back().is_land = true;
		context.state.military_definitions.unit_base_definitions.back().active = false;
		context.map_of_unit_types.insert_or_assign(std::string("army_base"), army_base_id);
		context.state.military_definitions.base_army_unit = army_base_id;
	}
	{
		dcon::unit_type_id navy_base_id = dcon::unit_type_id(dcon::unit_type_id::value_base_t(context.state.military_definitions.unit_base_definitions.size()));
		context.state.military_definitions.unit_base_definitions.emplace_back();

		auto name_id = text::find_or_add_key(context.state, "navy_base");
		context.state.military_definitions.unit_base_definitions.back().name = name_id;
		context.state.military_definitions.unit_base_definitions.back().is_land = false;
		context.state.military_definitions.unit_base_definitions.back().active = false;
		context.map_of_unit_types.insert_or_assign(std::string("navy_base"), navy_base_id);
		context.state.military_definitions.base_naval_unit = navy_base_id;
	}
}

void make_unit(std::string_view name, token_generator& gen, error_handler& err, scenario_building_context& context) {
	dcon::unit_type_id new_id = dcon::unit_type_id(dcon::unit_type_id::value_base_t(context.state.military_definitions.unit_base_definitions.size()));
	context.state.military_definitions.unit_base_definitions.emplace_back();

	auto name_id = text::find_or_add_key(context.state, name);
	
	context.state.military_definitions.unit_base_definitions.back() = parsers::parse_unit_definition(gen, err, context);
	context.state.military_definitions.unit_base_definitions.back().name = name_id;

	context.map_of_unit_types.insert_or_assign(std::string(name), new_id);
}

dcon::trigger_key cb_allowed_states(token_generator& gen, error_handler& err, individual_cb_context& context) {
	trigger_building_context t_context{ context.outer_context, trigger::slot_contents::state, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_trigger(gen, err, t_context);
}
dcon::trigger_key cb_allowed_crisis_states(token_generator& gen, error_handler& err, individual_cb_context& context) {
	trigger_building_context t_context{ context.outer_context, trigger::slot_contents::state, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_trigger(gen, err, t_context);
}
dcon::trigger_key cb_allowed_substates(token_generator& gen, error_handler& err, individual_cb_context& context) {
	trigger_building_context t_context{ context.outer_context, trigger::slot_contents::state, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_trigger(gen, err, t_context);
}
dcon::trigger_key cb_allowed_countries(token_generator& gen, error_handler& err, individual_cb_context& context) {
	trigger_building_context t_context{ context.outer_context, trigger::slot_contents::nation, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_trigger(gen, err, t_context);
}
dcon::trigger_key cb_can_use(token_generator& gen, error_handler& err, individual_cb_context& context) {
	trigger_building_context t_context{ context.outer_context, trigger::slot_contents::nation, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_trigger(gen, err, t_context);
}
dcon::effect_key cb_on_add(token_generator& gen, error_handler& err, individual_cb_context& context) {
	effect_building_context t_context{ context.outer_context, trigger::slot_contents::nation, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_effect(gen, err, t_context);
}
dcon::effect_key cb_on_po_accepted(token_generator& gen, error_handler& err, individual_cb_context& context) {
	effect_building_context t_context{ context.outer_context, trigger::slot_contents::nation, trigger::slot_contents::nation, trigger::slot_contents::nation };
	return make_effect(gen, err, t_context);
}

void make_oob_army(token_generator& gen, error_handler& err, oob_file_context& context) {
	auto id = context.outer_context.state.world.create_army();
	context.outer_context.state.world.force_create_army_control(id, context.nation_for);
	oob_file_army_context new_context{ context.outer_context, id, context.nation_for };
	parse_oob_army(gen, err, new_context);
}
void make_oob_navy(token_generator& gen, error_handler& err, oob_file_context& context) {
	auto id = context.outer_context.state.world.create_navy();
	context.outer_context.state.world.force_create_navy_control(id, context.nation_for);
	oob_file_navy_context new_context{ context.outer_context, id, context.nation_for };
	parse_oob_navy(gen, err, new_context);
}
void make_oob_regiment(token_generator& gen, error_handler& err, oob_file_army_context& context) {
	auto id = context.outer_context.state.world.create_regiment();
	context.outer_context.state.world.regiment_set_strength(id, 1.0f);
	context.outer_context.state.world.force_create_army_membership(id, context.id);
	oob_file_regiment_context new_context{ context.outer_context, id };
	parse_oob_regiment(gen, err, new_context);
}
void make_oob_ship(token_generator& gen, error_handler& err, oob_file_navy_context& context) {
	auto id = context.outer_context.state.world.create_ship();
	context.outer_context.state.world.ship_set_strength(id, 1.0f);
	context.outer_context.state.world.force_create_navy_membership(id, context.id);
	oob_file_ship_context new_context{ context.outer_context, id };
	parse_oob_ship(gen, err, new_context);
}
oob_leader make_army_leader(token_generator& gen, error_handler& err, oob_file_army_context& context) {
	oob_file_context new_context{ context.outer_context, context.nation_for };
	return parse_oob_leader(gen, err, new_context);
}

}

