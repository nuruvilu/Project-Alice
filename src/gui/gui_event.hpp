#pragma once

#include <variant>
#include "gui_element_types.hpp"

namespace ui {

//
// National events
//
typedef std::variant<
    dcon::national_event_id,
    dcon::free_national_event_id
> national_event_data_wrapper;
class national_event_option_button : public generic_settable_element<button_element_base, national_event_data_wrapper> {
public:
    uint8_t index = 0;
    void on_update(sys::state& state) noexcept override {
        dcon::text_sequence_id name{};
        if(std::holds_alternative<dcon::national_event_id>(content))
            name = state.world.national_event_get_options(std::get<dcon::national_event_id>(content))[index].name;
        else if(std::holds_alternative<dcon::free_national_event_id>(content))
            name = state.world.free_national_event_get_options(std::get<dcon::free_national_event_id>(content))[index].name;
        if(bool(name)) {
            set_button_text(state, text::produce_simple_string(state, name));
            set_visible(state, true);
        } else {
            set_visible(state, false);
        }
    }

    void button_action(sys::state& state) noexcept override {
        sys::event_option option{};
        if(std::holds_alternative<dcon::national_event_id>(content))
            option = state.world.national_event_get_options(std::get<dcon::national_event_id>(content))[index];
        else if(std::holds_alternative<dcon::free_national_event_id>(content))
            option = state.world.free_national_event_get_options(std::get<dcon::free_national_event_id>(content))[index];
        Cyto::Any payload = option;
        parent->impl_set(state, payload);
    }
};
class national_event_image : public generic_settable_element<image_element_base, national_event_data_wrapper> {
public:
    void on_update(sys::state& state) noexcept override {
        if(std::holds_alternative<dcon::national_event_id>(content))
            base_data.data.image.gfx_object = state.world.national_event_get_image(std::get<dcon::national_event_id>(content));
        else if(std::holds_alternative<dcon::free_national_event_id>(content))
            base_data.data.image.gfx_object = state.world.free_national_event_get_image(std::get<dcon::free_national_event_id>(content));
    }
};
class national_event_desc_text : public generic_settable_element<multiline_text_element_base, national_event_data_wrapper> {
public:
    void on_update(sys::state& state) noexcept override {
        auto contents = text::create_endless_layout(
            internal_layout,
            text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), base_data.data.text.font_handle, 0, text::alignment::left, text::text_color::black }
        );

        dcon::text_sequence_id description{};
        if(std::holds_alternative<dcon::national_event_id>(content))
            description = state.world.national_event_get_description(std::get<dcon::national_event_id>(content));
        else if(std::holds_alternative<dcon::free_national_event_id>(content))
            description = state.world.free_national_event_get_description(std::get<dcon::free_national_event_id>(content));
        
        auto box = text::open_layout_box(contents);
        text::add_to_layout_box(contents, state, box, description, text::substitution_map{});
        text::close_layout_box(contents, box);
    }
};
class national_event_name_text : public generic_settable_element<multiline_text_element_base, national_event_data_wrapper> {
public:
    void on_update(sys::state& state) noexcept override {
        auto contents = text::create_endless_layout(
            internal_layout,
            text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), base_data.data.text.font_handle, 0, text::alignment::center, text::text_color::black }
        );

        dcon::text_sequence_id name{};
        if(std::holds_alternative<dcon::national_event_id>(content))
            name = state.world.national_event_get_name(std::get<dcon::national_event_id>(content));
        else if(std::holds_alternative<dcon::free_national_event_id>(content))
            name = state.world.free_national_event_get_name(std::get<dcon::free_national_event_id>(content));
        
        auto box = text::open_layout_box(contents);
        text::add_to_layout_box(contents, state, box, name, text::substitution_map{});
        text::close_layout_box(contents, box);
    }
};
template<bool IsMajor>
class national_event_window : public window_element_base {
    national_event_data_wrapper content{};
    void select_event_option(sys::state& state, const sys::event_option opt) {
        auto ptr = state.ui_state.root->remove_child(this);
        if(IsMajor)
            state.ui_state.spare_major_event_subwindows.push_back(std::move(ptr));
        else
            state.ui_state.spare_national_event_subwindows.push_back(std::move(ptr));
    }
public:
    void on_create(sys::state& state) noexcept override {
        window_element_base::on_create(state);
        auto s1 = IsMajor ? "event_major_option_start" : "event_country_option_start";
        auto s2 = IsMajor ? "event_major_option_offset" : "event_country_option_offset";
        auto s3 = IsMajor ? "event_major_optionbutton" : "event_country_optionbutton";

        xy_pair cur_offset = state.ui_defs.gui[state.ui_state.defs_by_name.find(s1)->second.definition].position;
        xy_pair offset = state.ui_defs.gui[state.ui_state.defs_by_name.find(s2)->second.definition].position;
        for(size_t i = 0; i < size_t(sys::max_event_options); ++i) {
            auto ptr = make_element_by_type<national_event_option_button>(state, state.ui_state.defs_by_name.find(s3)->second.definition);
            ptr->base_data.position = cur_offset;
            ptr->index = uint8_t(i);
            add_child_to_front(std::move(ptr));
            cur_offset.x += offset.x;
            cur_offset.y += offset.y;
        }
        set_visible(state, false);
    }
    std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
        if(name == "title") {
            return make_element_by_type<national_event_name_text>(state, id);
        } else if(name == "description") {
            return make_element_by_type<national_event_desc_text>(state, id);
        } else if(name == "image") {
            return make_element_by_type<national_event_image>(state, id);
        } else if(name == "date") {
            return make_element_by_type<simple_text_element_base>(state, id);
        } else {
            // Handling for the case of "major national" flavour
            // TODO: correct rotation for this flag
            if(name == "country_flag1") {
                auto ptr = make_element_by_type<flag_button>(state, id);
                ptr->base_data.flags &= ui::element_data::rotation_mask;
                ptr->base_data.flags |= uint8_t(ui::rotation::r90_left);
                ptr->base_data.position.x += 21;
                ptr->base_data.position.y -= 23;
                ptr->base_data.size.x -= 32;
                ptr->base_data.size.y += 42;
                return ptr;
            } else if(name == "country_flag2") {
                auto ptr = make_element_by_type<flag_button>(state, id);
                ptr->base_data.position.x += 21;
                ptr->base_data.position.y -= 23;
                ptr->base_data.size.x -= 32;
                ptr->base_data.size.y += 42;
                return ptr;
            }
            return nullptr;
        }
    }
    message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
        if(payload.holds_type<national_event_data_wrapper>()) {
            content = any_cast<national_event_data_wrapper>(payload);
            set_visible(state, true);
            for(auto& child : children)
                child->impl_set(state, payload);
            on_update(state);
            return message_result::consumed;
        } else if(payload.holds_type<sys::event_option>()) {
            const auto opt = any_cast<sys::event_option>(payload);
            select_event_option(state, opt);
            set_visible(state, false);
            return message_result::consumed;
        }
        return message_result::unseen;
    }
    message_result get(sys::state& state, Cyto::Any& payload) noexcept override {
        if(payload.holds_type<dcon::nation_id>()) {
            payload.emplace<dcon::nation_id>(state.local_player_nation);
            return message_result::consumed;
        }
        return message_result::unseen;
    }
};

//
// Provincial events
//
typedef std::variant<
    dcon::provincial_event_id,
    dcon::free_provincial_event_id
> provincial_event_data_wrapper;
class provincial_event_option_button : public generic_settable_element<button_element_base, provincial_event_data_wrapper> {
public:
    uint8_t index = 0;
    void on_update(sys::state& state) noexcept override {
        dcon::text_sequence_id name{};
        if(std::holds_alternative<dcon::provincial_event_id>(content))
            name = state.world.provincial_event_get_options(std::get<dcon::provincial_event_id>(content))[index].name;
        else if(std::holds_alternative<dcon::free_provincial_event_id>(content))
            name = state.world.free_provincial_event_get_options(std::get<dcon::free_provincial_event_id>(content))[index].name;
        if(bool(name)) {
            set_button_text(state, text::produce_simple_string(state, name));
            set_visible(state, true);
        } else {
            set_visible(state, false);
        }
    }

    void button_action(sys::state& state) noexcept override {
        sys::event_option option{};
        if(std::holds_alternative<dcon::provincial_event_id>(content))
            option = state.world.provincial_event_get_options(std::get<dcon::provincial_event_id>(content))[index];
        else if(std::holds_alternative<dcon::free_provincial_event_id>(content))
            option = state.world.free_provincial_event_get_options(std::get<dcon::free_provincial_event_id>(content))[index];
        Cyto::Any payload = option;
        parent->impl_set(state, payload);
    }
};
class provincial_event_desc_text : public generic_settable_element<multiline_text_element_base, provincial_event_data_wrapper> {
public:
    void on_update(sys::state& state) noexcept override {
        auto contents = text::create_endless_layout(
            internal_layout,
            text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), base_data.data.text.font_handle, 0, text::alignment::left, text::text_color::black }
        );

        dcon::text_sequence_id description{};
        if(std::holds_alternative<dcon::provincial_event_id>(content))
            description = state.world.provincial_event_get_description(std::get<dcon::provincial_event_id>(content));
        else if(std::holds_alternative<dcon::free_provincial_event_id>(content))
            description = state.world.free_provincial_event_get_description(std::get<dcon::free_provincial_event_id>(content));
        
        auto box = text::open_layout_box(contents);
        text::add_to_layout_box(contents, state, box, description, text::substitution_map{});
        text::close_layout_box(contents, box);
    }
};
class provincial_event_name_text : public generic_settable_element<multiline_text_element_base, provincial_event_data_wrapper> {
public:
    void on_update(sys::state& state) noexcept override {
        auto contents = text::create_endless_layout(
            internal_layout,
            text::layout_parameters{ 0, 0, static_cast<int16_t>(base_data.size.x), static_cast<int16_t>(base_data.size.y), base_data.data.text.font_handle, 0, text::alignment::center, text::text_color::black }
        );

        dcon::text_sequence_id name{};
        if(std::holds_alternative<dcon::provincial_event_id>(content))
            name = state.world.provincial_event_get_name(std::get<dcon::provincial_event_id>(content));
        else if(std::holds_alternative<dcon::free_provincial_event_id>(content))
            name = state.world.free_provincial_event_get_name(std::get<dcon::free_provincial_event_id>(content));
        
        auto box = text::open_layout_box(contents);
        text::add_to_layout_box(contents, state, box, name, text::substitution_map{});
        text::close_layout_box(contents, box);
    }
};
class provincial_event_window : public window_element_base {
    provincial_event_data_wrapper content{};
    void select_event_option(sys::state& state, const sys::event_option opt) {
        auto ptr = state.ui_state.root->remove_child(this);
        state.ui_state.spare_provincial_event_subwindows.push_back(std::move(ptr));
    }
public:
    void on_create(sys::state& state) noexcept override {
        window_element_base::on_create(state);

        xy_pair cur_offset = state.ui_defs.gui[state.ui_state.defs_by_name.find("event_province_option_start")->second.definition].position;
        xy_pair offset = state.ui_defs.gui[state.ui_state.defs_by_name.find("event_province_option_offset")->second.definition].position;
        for(size_t i = 0; i < size_t(sys::max_event_options); ++i) {
            auto ptr = make_element_by_type<provincial_event_option_button>(state, state.ui_state.defs_by_name.find("event_province_optionbutton")->second.definition);
            ptr->base_data.position = cur_offset;
            ptr->index = uint8_t(i);
            add_child_to_front(std::move(ptr));

            cur_offset.x += offset.x;
            cur_offset.y += offset.y;
        }
        set_visible(state, false);
    }
    std::unique_ptr<element_base> make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept override {
        if(name == "title") {
            return make_element_by_type<provincial_event_name_text>(state, id);
        } else if(name == "description") {
            return make_element_by_type<provincial_event_desc_text>(state, id);
        } else if(name == "date") {
            return make_element_by_type<simple_text_element_base>(state, id);
        } else {
            return nullptr;
        }
    }
    message_result set(sys::state& state, Cyto::Any& payload) noexcept override {
        if(payload.holds_type<provincial_event_data_wrapper>()) {
            content = any_cast<provincial_event_data_wrapper>(payload);
            set_visible(state, true);
            for(auto& child : children)
                child->impl_set(state, payload);
            on_update(state);
            return message_result::consumed;
        } else if(payload.holds_type<sys::event_option>()) {
            const auto opt = any_cast<sys::event_option>(payload);
            select_event_option(state, opt);
            set_visible(state, false);
            return message_result::consumed;
        }
        return message_result::unseen;
    }
};

void fire_event(sys::state& state, const dcon::national_event_id event_id);
void fire_event(sys::state& state, const dcon::free_national_event_id event_id);
void fire_event(sys::state& state, const dcon::provincial_event_id event_id);
void fire_event(sys::state& state, const dcon::free_provincial_event_id event_id);

}

