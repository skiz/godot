/*************************************************************************/
/*  plugin_config_dialog.cpp                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "plugin_config_dialog.h"
#include "core/io/config_file.h"
#include "core/os/dir_access.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "modules/gdscript/gdscript.h"
#include "scene/gui/grid_container.h"

void PluginImportDialog::_clear_fields() {
	source_edit->set_text("");
}

void PluginImportDialog::_on_confirmed() {
}

void PluginImportDialog::_on_source_confirmed() {
  //TODO generate path based on source
  /*
	String path = "res://addons/" + subfolder_edit->get_text();

	if (!_edit_mode) {
		DirAccess *d = DirAccess::create(DirAccess::ACCESS_RESOURCES);
		if (!d || d->make_dir_recursive(path) != OK)
			return;
	}

	Ref<ImportFile> cf = memnew(ImportFile);
	cf->set_value("plugin", "name", source_edit->get_text());
	cf->set_value("plugin", "description", desc_edit->get_text());
	cf->set_value("plugin", "author", author_edit->get_text());
	cf->set_value("plugin", "version", version_edit->get_text());
	cf->set_value("plugin", "script", script_edit->get_text());

	cf->save(path.plus_file("plugin.cfg"));

	if (!_edit_mode) {
		int lang_idx = script_option_edit->get_selected();
		String lang_name = ScriptServer::get_language(lang_idx)->get_name();

		Ref<Script> script;

		// TODO Use script templates. Right now, this code won't add the 'tool' annotation to other languages.
		// TODO Better support script languages with named classes (has_named_classes).

		if (lang_name == GDScriptLanguage::get_singleton()->get_name()) {
			// Hard-coded GDScript template to keep usability until we use script templates.
			Ref<GDScript> gdscript = memnew(GDScript);
			gdscript->set_source_code(
					"tool\n"
					"extends EditorPlugin\n"
					"\n"
					"func _enter_tree():\n"
					"\tpass\n"
					"\n"
					"func _exit_tree():\n"
					"\tpass\n");
			String script_path = path.plus_file(script_edit->get_text());
			gdscript->set_path(script_path);
			ResourceSaver::save(script_path, gdscript);
			script = gdscript;
		} else {
			String script_path = path.plus_file(script_edit->get_text());
			String class_name = script_path.get_file().get_basename();
			script = ScriptServer::get_language(lang_idx)->get_template(class_name, "EditorPlugin");
			script->set_path(script_path);
			ResourceSaver::save(script_path, script);
		}

		emit_signal("plugin_ready", script.operator->(), active_edit->is_pressed() ? subfolder_edit->get_text() : "");
	} else {
		EditorNode::get_singleton()->get_project_settings()->update_plugins();
	}
	_clear_fields();
  */
}

void PluginImportDialog::_on_cancelled() {
	_clear_fields();
}

void PluginImportDialog::_on_required_text_changed(const String &) {
	get_ok()->set_disabled(source_edit->get_text().empty());
}

void PluginImportDialog::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			connect("confirmed", this, "_on_confirmed");
			get_cancel()->connect("pressed", this, "_on_cancelled");
		} break;

		case NOTIFICATION_POST_POPUP: {
			source_edit->grab_focus();
		} break;
	}
}

void PluginImportDialog::config(const String &p_config_path) {
  _clear_fields();
  _edit_mode = false;
  set_title(TTR("Import a Plugin"));
	get_ok()->set_disabled(!_edit_mode);
	get_ok()->set_text("Import");
}

void PluginImportDialog::_bind_methods() {
	ClassDB::bind_method("_on_required_text_changed", &PluginImportDialog::_on_required_text_changed);
	ClassDB::bind_method("_on_confirmed", &PluginImportDialog::_on_confirmed);
	ClassDB::bind_method("_on_cancelled", &PluginImportDialog::_on_cancelled);
	// ADD_SIGNAL(MethodInfo("plugin_ready", PropertyInfo(Variant::STRING, "script_path", PROPERTY_HINT_NONE, ""), PropertyInfo(Variant::STRING, "activate_name")));
}

PluginImportDialog::PluginImportDialog() {
	get_ok()->set_disabled(true);
	set_hide_on_ok(true);

	GridContainer *grid = memnew(GridContainer);
	grid->set_columns(2);
	add_child(grid);

	Label *source_lb = memnew(Label);
	source_lb->set_text(TTR("Plugin Source:"));
	grid->add_child(source_lb);

	source_edit = memnew(LineEdit);
	source_edit->connect("text_changed", this, "_on_required_text_changed");
	source_edit->set_placeholder("https://github.com/foo/bar.git");
	grid->add_child(source_edit);
}

PluginImportDialog::~PluginImportDialog() {
}
