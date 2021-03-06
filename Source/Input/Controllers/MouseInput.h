#pragma once

#include <Urho3D/Urho3DAll.h>
#include "BaseInput.h"
#include "../../Config/ConfigFile.h"

class MouseInput : public BaseInput
{
    URHO3D_OBJECT(MouseInput, BaseInput);

public:
    /// Construct.
    MouseInput(Context* context);

    virtual ~MouseInput();
	virtual String GetActionKeyName(int action);

protected:
    virtual void Init();

private:
    void SubscribeToEvents();

	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleKeyUp(StringHash eventType, VariantMap& eventData);
	void HandleMouseMove(StringHash eventType, VariantMap& eventData);
};