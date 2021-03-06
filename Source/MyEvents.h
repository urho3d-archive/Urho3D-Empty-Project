#pragma once

#include <Urho3D/Urho3DAll.h>

/// User defined event
namespace MyEvents
{
    // Start new level
    URHO3D_EVENT(E_SET_LEVEL, SetLevel)
    {
        URHO3D_PARAM(P_NAME, Name); // string - level object name
        URHO3D_PARAM(P_MESSAGE, Message); // string - pop-up message content, can be empty
    }

    // Open UI Window
    URHO3D_EVENT(E_OPEN_WINDOW, OpenWindow)
    {
        URHO3D_PARAM(P_NAME, Name); // string - window object name
    }
    // Close UI Window
    URHO3D_EVENT(E_CLOSE_WINDOW, CloseWindow)
    {
        URHO3D_PARAM(P_NAME, Name); // string - window object name
    }
    // When specific UI window is closed
    URHO3D_EVENT(E_WINDOW_CLOSED, WindowClosed)
    {
        URHO3D_PARAM(P_NAME, Name); // string - close window object name
    }
    // Close all active UI Windows
    URHO3D_EVENT(E_CLOSE_ALL_WINDOWS, CloseAllWindows)
    {
    }

    // Add new console command
	URHO3D_EVENT(E_CONSOLE_COMMAND_ADD, ConsoleCommandAdd)
	{
		URHO3D_PARAM(P_NAME, ConsoleCommandName); // string - command name
		URHO3D_PARAM(P_EVENT, ConsoleCommandEvent); // string - event to call when command entered
		URHO3D_PARAM(P_DESCRIPTION, ConsoleCommandDescription); // string - short description of the command
	}

    // Change global system variable
	URHO3D_EVENT(E_CONSOLE_GLOBAL_VARIABLE_CHANGE, ConsoleGlobalVariableChange)
	{
		URHO3D_PARAM(P_NAME, GlobalVariableName); // string - global variable name
		URHO3D_PARAM(P_VALUE, GlobalVariableValue); // string - new value
	}

    // When global variable is changed
	URHO3D_EVENT(E_CONSOLE_GLOBAL_VARIABLE_CHANGED, ConsoleGlobalVariableChanged)
	{
		URHO3D_PARAM(P_NAME, GlobalVariableName); // string - global variable name
		URHO3D_PARAM(P_VALUE, GlobalVariableValue); // string - new value
	}

    // Save configuration JSON file
    URHO3D_EVENT(E_SAVE_CONFIG, SaveConfig)
    {
    }

    // Add new global variable which should be saved in the config files
    URHO3D_EVENT(E_ADD_CONFIG, AddConfig)
    {
        URHO3D_PARAM(P_NAME, Name); // string - global variable name
    }

    // Load configuration file
    URHO3D_EVENT(E_LOAD_CONFIG, LoadConfig)
    {
        URHO3D_PARAM(P_FILEPATH, Filepath); // string - filepath + filename, relative to executable
        URHO3D_PARAM(P_PREFIX, Prefix); // string - prefix, which will be added to loaded configuration variables, can be empty
    }

    // Start mapping key to specific action
	URHO3D_EVENT(E_START_INPUT_MAPPING, StartInputMapping)
	{
		URHO3D_PARAM(P_CONTROL_ACTION, ControlAction); // string or int - action name or ID
	}

    // When mapping was finished
    URHO3D_EVENT(E_INPUT_MAPPING_FINISHED, InputMappingFinished)
	{
        URHO3D_PARAM(P_CONTROLLER, Controller); // string - keyboard, mouse, joystick - which controller was used to do the mapping
		URHO3D_PARAM(P_CONTROL_ACTION, ControlAction); // int - action ID
        URHO3D_PARAM(P_ACTION_NAME, ActionName); // string - action name
        URHO3D_PARAM(P_KEY, Key); // int - key ID, relative to P_CONTROLLEr
        URHO3D_PARAM(P_KEY_NAME, KeyName); // string - key name
	}

	// New controller/joystick added
	URHO3D_EVENT(E_CONTROLLER_ADDED, ControllerAdded)
	{
		URHO3D_PARAM(P_INDEX, Index); // string or int - controller id
	}

	// controller/joystick removed
	URHO3D_EVENT(E_CONTROLLER_REMOVED, ControllerRemoved)
	{
		URHO3D_PARAM(P_INDEX, Index); // string or int - controller id
	}

    // play sound
	URHO3D_EVENT(E_PLAY_SOUND, PlaySound)
	{
		URHO3D_PARAM(P_INDEX, Index); // string or int - sound id
        URHO3D_PARAM(P_TYPE, Type); // string - sound type - Master / Effect / Ambient / Voice / Music
        URHO3D_PARAM(P_SOUND_FILE, SoundFile); // string - full path to sound file, not needed when Index is used
	}

    // stop sound
	URHO3D_EVENT(E_STOP_SOUND, StopSound)
	{
		URHO3D_PARAM(P_INDEX, Index); // int - sound id
        URHO3D_PARAM(P_TYPE, Type); // string - sound type - Master / Effect / Ambient / Voice / Music
	}

    // stop all sounds in progress
	URHO3D_EVENT(E_STOP_ALL_SOUNDS, StopAllSounds)
	{
	}
}