#include <Urho3D/Urho3DAll.h>
#include "ControllerInput.h"
#include "../MyEvents.h"
#include "../Global.h"

/// Construct.
ControllerInput::ControllerInput(Context* context) :
    Object(context),
	_activeAction(0),
	_mouseSensitivity(0.0f)
{
	_controlMapNames[CTRL_FORWARD] = "CTRL_FORWARD";
	_controlMapNames[CTRL_BACK] = "CTRL_BACK";
	_controlMapNames[CTRL_LEFT] = "CTRL_LEFT";
	_controlMapNames[CTRL_RIGHT] = "CTRL_RIGHT";
	_controlMapNames[CTRL_JUMP] = "CTRL_JUMP";
	_controlMapNames[CTRL_ACTION] = "CTRL_ACTION";
	_controlMapNames[CTRL_SPRINT] = "CTRL_SPRINT";
	_controlMapNames[CTRL_UP] = "CTRL_UP";

	Init();
}

ControllerInput::~ControllerInput()
{
}

void ControllerInput::Init()
{
    // Subscribe to global events for camera movement
    SubscribeToEvents();

	LoadConfig();
}

void ControllerInput::LoadConfig()
{
	auto* cache = GetSubsystem<ResourceCache>();
	_configFile = cache->GetResource<ConfigFile>("Config/controls.cfg");

	for (auto it = _controlMapNames.Begin(); it != _controlMapNames.End(); ++it) {
		String controlName = (*it).second_;
		int controlCode = (*it).first_;
		if (_configFile->GetInt("keyboard", controlName, -1) != -1) {
			int key = _configFile->GetInt("keyboard", controlName, 0);
			_mappedKeyboardControlsToKeys[controlCode] = key;
			_mappedKeyboardKeysToControls[key] = controlCode;
			URHO3D_LOGINFO("Keyboard control " + controlName + " => " + String(key));
		}
		if (_configFile->GetInt("mouse", controlName, -1) != -1) {
			int key = _configFile->GetInt("mouse", controlName, 0);
			_mappedMouseControlsToKeys[controlCode] = key;
			_mappedMouseKeysToControls[key] = controlCode;
			URHO3D_LOGINFO("Mouse control " + controlName + " => " + String(key));
		}
	}

	_mouseSensitivity = _configFile->GetFloat("mouse", "Sensitivity", 0.1f);

	CreateConfigMaps();
}

void ControllerInput::CreateConfigMaps()
{
	// _mappedKeyboardControlsToKeys.Clear();
	// _mappedKeyboardKeysToControls.Clear();

	// _mappedMouseControlsToKeys.Clear();
	// _mappedMouseKeysToControls.Clear();
}

void ControllerInput::SaveConfig()
{
	for (auto it = _controlMapNames.Begin(); it != _controlMapNames.End(); ++it) {
		String controlName = (*it).second_;
		int controlCode = (*it).first_;
		_configFile->Set("keyboard", controlName, "-1");
		_configFile->Set("mouse", controlName, "-1");
	}
	for (auto it = _mappedKeyboardControlsToKeys.Begin(); it != _mappedKeyboardControlsToKeys.End(); ++it) {
		int controlCode = (*it).first_;
		int keyCode = (*it).second_;
		if (_controlMapNames.Contains(controlCode) && !_controlMapNames[controlCode].Empty()) {
			String controlName = _controlMapNames[controlCode];
			String value = String(keyCode);
			_configFile->Set("keyboard", controlName, value);
			URHO3D_LOGINFO(">>>>>>>> Setting keyboard : " + controlName + " => " + value);
		}
	}

	for (auto it = _mappedMouseControlsToKeys.Begin(); it != _mappedMouseControlsToKeys.End(); ++it) {
		int controlCode = (*it).first_;
		int keyCode = (*it).second_;
		if (_controlMapNames.Contains(controlCode) && !_controlMapNames[controlCode].Empty()) {
			String controlName = _controlMapNames[controlCode];
			String value = String(keyCode);
			_configFile->Set("mouse", controlName, value);
			URHO3D_LOGINFO(">>>>>>>> Setting keyboard : " + controlName + " => " + value);
		}
	}
	_configFile->Set("mouse", "Sensitivity", String(_mouseSensitivity));

	Urho3D::File file(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Config/controls.cfg", Urho3D::FILE_WRITE);
	_configFile->Save(file, true);
	file.Close();
}

void ControllerInput::SubscribeToEvents()
{
	SubscribeToEvent(MyEvents::E_START_INPUT_MAPPING, URHO3D_HANDLER(ControllerInput, HandleStartInputListening));

	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(ControllerInput, HandleKeyDown));
	SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(ControllerInput, HandleKeyUp));

	SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(ControllerInput, HandleMouseButtonDown));
	SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(ControllerInput, HandleMouseButtonUp));

	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(ControllerInput, HandleUpdate));
	SubscribeToEvent("StartInputMappingConsole", URHO3D_HANDLER(ControllerInput, HandleStartInputListeningConsole));
	RegisterConsoleCommands();
}

void ControllerInput::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();

	if (_activeAction > 0 && _timer.GetMSec(false) > 100) {
		SetConfiguredKey(_activeAction, key, "keyboard");
		_activeAction = 0;
		return;
	}

	auto* input = GetSubsystem<Input>();
	_controls.Set(_mappedKeyboardKeysToControls[key], true);
}

void ControllerInput::HandleKeyUp(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyUp;
	int key = eventData[P_KEY].GetInt();

	if (_activeAction > 0) {
		return;
	}

	auto* input = GetSubsystem<Input>();
	_controls.Set(_mappedKeyboardKeysToControls[key], false);
}

void ControllerInput::HandleMouseButtonDown(StringHash eventType, VariantMap& eventData)
{
	using namespace MouseButtonDown;
	int key = eventData[P_BUTTON].GetInt();

	if (_activeAction > 0 && _timer.GetMSec(false) > 100) {
		SetConfiguredKey(_activeAction, key, "mouse");
		_activeAction = 0;
		return;
	}

	auto* input = GetSubsystem<Input>();
	_controls.Set(_mappedMouseKeysToControls[key], true);
}

void ControllerInput::HandleMouseButtonUp(StringHash eventType, VariantMap& eventData)
{
	using namespace MouseButtonDown;
	int key = eventData[P_BUTTON].GetInt();

	if (_activeAction > 0) {
		return;
	}

	auto* input = GetSubsystem<Input>();
	_controls.Set(_mappedMouseKeysToControls[key], false);
}

void ControllerInput::ReleaseConfiguredKey(int key, int action)
{
	// Release key if used
	for (auto it = _mappedKeyboardKeysToControls.Begin(); it != _mappedKeyboardKeysToControls.End(); ++it) {
		int keyCode = (*it).first_;
		int actionCode = (*it).second_;
		if (key == keyCode) {
			_mappedKeyboardKeysToControls.Erase(keyCode);
		}
		if (action == actionCode) {
			_mappedKeyboardKeysToControls.Erase(keyCode);
		}
	}

	for (auto it = _mappedKeyboardControlsToKeys.Begin(); it != _mappedKeyboardControlsToKeys.End(); ++it) {
		int keyCode = (*it).second_;
		int actionCode = (*it).first_;
		if (key == keyCode) {
			_mappedKeyboardControlsToKeys.Erase(actionCode);
		}
		if (action == actionCode) {
			_mappedKeyboardControlsToKeys.Erase(actionCode);
		}
	}

	// Release key if used
	for (auto it = _mappedMouseKeysToControls.Begin(); it != _mappedMouseKeysToControls.End(); ++it) {
		int keyCode = (*it).first_;
		int actionCode = (*it).second_;
		if (key == keyCode) {
			_mappedMouseKeysToControls.Erase(keyCode);
		}
		if (action == actionCode) {
			_mappedMouseKeysToControls.Erase(keyCode);
		}
	}

	for (auto it = _mappedMouseControlsToKeys.Begin(); it != _mappedMouseControlsToKeys.End(); ++it) {
		int keyCode = (*it).second_;
		int actionCode = (*it).first_;
		if (key == keyCode) {
			_mappedMouseControlsToKeys.Erase(actionCode);
		}
		if (action == actionCode) {
			_mappedMouseControlsToKeys.Erase(actionCode);
		}
	}

}

void ControllerInput::SetConfiguredKey(int action, int key, String controller)
{
	ReleaseConfiguredKey(key, action);
	auto* input = GetSubsystem<Input>();
	URHO3D_LOGINFO("Setting " + controller + " key " + input->GetKeyName(key) + "[" + String(key) + "] to action " + String(action));
	if (controller == "keyboard") {
		_mappedKeyboardControlsToKeys[action] = key;
		_mappedKeyboardKeysToControls[key] = action;
	}
	if (controller == "mouse") {
		_mappedMouseControlsToKeys[action] = key;
		_mappedMouseKeysToControls[key] = action;
	}

	using namespace MyEvents::InputMappingFinished;
	VariantMap data = GetEventDataMap();
	data[P_CONTROLLER] = controller;
	data[P_CONTROL_ACTION] = action;
	data[P_ACTION_NAME] = _controlMapNames[action];
	data[P_KEY] = key;
	data[P_KEY_NAME] = input->GetKeyName(key);
	SendEvent(MyEvents::E_INPUT_MAPPING_FINISHED, data);

	SaveConfig();
}

void ControllerInput::HandleStartInputListening(StringHash eventType, VariantMap& eventData)
{
	using namespace MyEvents::StartInputMapping;
	if (eventData[P_CONTROL_ACTION].GetType() == VAR_INT) {
		_activeAction = eventData[P_CONTROL_ACTION].GetInt();
		URHO3D_LOGINFO("Starting input listener!");
		URHO3D_LOGINFO("Control: " + _mappedKeyboardControlsToKeys[_activeAction]);
	}
	if (eventData[P_CONTROL_ACTION].GetType() == VAR_STRING) {
		String control = eventData[P_CONTROL_ACTION].GetString();
		for (auto it = _controlMapNames.Begin(); it != _controlMapNames.End(); ++it) {
			if ((*it).second_ == control) {
				_activeAction = (*it).first_;
				URHO3D_LOGINFO("Starting input listener!");
				URHO3D_LOGINFO("Control: " + control);
			}
		}
	}

	_timer.Reset();
}

void ControllerInput::RegisterConsoleCommands()
{
	VariantMap data = GetEventDataMap();
    data["ConsoleCommandName"] = "map_input";
    data["ConsoleCommandEvent"] = "StartInputMappingConsole";
    data["ConsoleCommandDescription"] = "Listening for keystroke";
    SendEvent("ConsoleCommandAdd", data);
}

void ControllerInput::HandleStartInputListeningConsole(StringHash eventType, VariantMap& eventData)
{
	StringVector parameters = eventData["Parameters"].GetStringVector();
	if (parameters.Size() == 2) {
		using namespace MyEvents::StartInputMapping;
		VariantMap data = GetEventDataMap();
		data[P_CONTROL_ACTION] = parameters[1];
		SendEvent(MyEvents::E_START_INPUT_MAPPING, data);
		return;
	}

	URHO3D_LOGERROR("Invalid number of parameters!");
}

Controls ControllerInput::GetControls()
{
	return _controls;
}

void ControllerInput::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	auto* input = GetSubsystem<Input>();
    // Mouse sensitivity as degrees per pixel

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    IntVector2 mouseMove = input->GetMouseMove();
    _controls.yaw_ += _mouseSensitivity * mouseMove.x_;
    _controls.pitch_ += _mouseSensitivity * mouseMove.y_;
    _controls.pitch_ = Clamp(_controls.pitch_, -90.0f, 90.0f);
}

HashMap<int, String> ControllerInput::GetControlNames()
{
	return _controlMapNames;
}

String ControllerInput::GetActionKeyName(int action)
{
	if (_mappedKeyboardControlsToKeys.Contains(action)) {
		auto* input = GetSubsystem<Input>();
		return input->GetKeyName(_mappedKeyboardControlsToKeys[action]);
	}
	if (_mappedMouseControlsToKeys.Contains(action)) {
		auto* input = GetSubsystem<Input>();
		int key = _mappedMouseControlsToKeys[action];
		if (key == MOUSEB_LEFT) {
			return "MOUSEB_LEFT";
		}
		if (key == MOUSEB_MIDDLE) {
			return "MOUSEB_MIDDLE";
		}
		if (key == MOUSEB_RIGHT) {
			return "MOUSEB_RIGHT";
		}
	}

	return String::EMPTY;
}