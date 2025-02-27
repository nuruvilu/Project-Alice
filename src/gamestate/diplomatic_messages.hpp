#pragma once

#include "dcon_generated.hpp"
#include "container_types.hpp"

namespace diplomatic_message {

enum class type_t : uint8_t {
	none = 0,
	access_request = 1,
	alliance_request = 2,
	call_ally_request = 3,
	be_crisis_primary_defender = 4,
	be_crisis_primary_attacker = 5
};

struct message {
	dcon::nation_id from;
	dcon::nation_id to;
	sys::date when;
	union dtype {
		dcon::war_id war;

		dtype() { }
	} data;
	type_t type;

	message() : type(diplomatic_message::type_t::none) { }
};

using type = type_t;

void decline_message(sys::state& state, message const& m);
void accept_message(sys::state& state, message const& m);

void post_message(sys::state& state, message const& m);
void update_pending_messages(sys::state& state);

}
