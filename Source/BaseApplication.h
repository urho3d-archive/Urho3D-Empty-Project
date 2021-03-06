#pragma once

#include <Urho3D/Urho3DAll.h>
#include "LevelManager.h"
#include "Messages/Message.h"
#include "Messages/Achievements.h"
#include "Messages/Notifications.h"
#include "Mods/ModLoader.h"
#include "UI/WindowManager.h"
#include "Config/ConfigManager.h"

using namespace Urho3D;

class BaseApplication : public Application
{
    URHO3D_OBJECT(BaseApplication, Application);

public:
    /// Construct.
    BaseApplication(Context* context);

    /// Setup before engine initialization. Verify that a script file has been specified.
    virtual void Setup() override;
    /// Setup after engine initialization. Load the script and execute its start function.
    virtual void Start() override;
    /// Cleanup after the main loop. Run the script's stop function if it exists.
    virtual void Stop() override;

private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

	void LoadINIConfig(String filename);
    void LoadConfig(String filename, String prefix = "", bool isMain = false);
    void SaveConfig();
    void HandleLoadConfig(StringHash eventType, VariantMap& eventData);
    void HandleSaveConfig(StringHash eventType, VariantMap& eventData);
    void HandleAddConfig(StringHash eventType, VariantMap& eventData);

	void HandleConsoleGlobalVariableChange(StringHash eventType, VariantMap& eventData);

    void RegisterConsoleCommands();

    void SubscribeToEvents();

    void SetEngineParameter(String parameter, Variant value);

    void HandleExit(StringHash eventType, VariantMap& eventData);

    SharedPtr<Message> _alertMessage;
    SharedPtr<Notifications> _notifications;
    SharedPtr<Achievements> _achievements;
    SharedPtr<WindowManager> _windowManager;
    VariantMap _globalSettings;

    /**
	 * Loaded configuration file
	 */
	SharedPtr<ConfigManager> _configManager;

    String _configurationFile;
};
