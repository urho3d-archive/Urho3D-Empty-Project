#include <Urho3D/Urho3DAll.h>
#include "Message.h"
#include "../Audio/AudioManagerDefs.h"
#include "../MyEvents.h"

/// Construct.
Message::Message(Context* context) :
    Object(context)
{
    Init();
}

Message::~Message()
{
}

void Message::Init()
{
    SubscribeToEvents();
}

bool Message::Create()
{
    if (_baseElement.Refs()) {
        URHO3D_LOGERROR("Another pop-up message is already active");
        return false;
    }
    UI* ui = GetSubsystem<UI>();
	Graphics* graphics = GetSubsystem<Graphics>();

    _baseElement = ui->GetRoot()->CreateChild("Menu");
	_baseElement->SetWidth(graphics->GetWidth());
	_baseElement->SetHeight(graphics->GetHeight());
	_baseElement->SetPosition(IntVector2(0, 0));
	_baseElement->SetColor(Color(0, 0, 0, 0.7));
	UIElement* messageContainer = _baseElement->CreateChild<UIElement>();
    File startButtonFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/UI/Message.xml", FILE_READ);
	messageContainer->LoadXML(startButtonFile);
    _okButton = static_cast<Button*>(_baseElement->GetChild("MessageOkButton", true));
    _title = static_cast<Text*>(_baseElement->GetChild("MessageTitle", true));
    _message = static_cast<Text*>(_baseElement->GetChild("MessageBody", true));
    SubscribeToEvents();

    return true;
}

void Message::SubscribeToEvents()
{
    SubscribeToEvent(_okButton, E_RELEASED, URHO3D_HANDLER(Message, HandleOkButton));
    SubscribeToEvent("ShowAlertMessage", URHO3D_HANDLER(Message, HandleShowMessage));
}

void Message::HandleShowMessage(StringHash eventType, VariantMap& eventData)
{
    String title = eventData["Title"].GetString();
    String message = eventData["Message"].GetString();
    if (Create() && _title.Refs() && _message.Refs()) {
        _title->SetText(title);
        _message->SetText(message);
    }
}

void Message::Dispose()
{
}

void Message::HandleOkButton(StringHash eventType, VariantMap& eventData)
{
    {
        using namespace AudioDefs;
		using namespace MyEvents::PlaySound;
		VariantMap data = GetEventDataMap();
		data[P_INDEX] = SOUND_EFFECTS::BUTTON_CLICK;
		data[P_TYPE] = SOUND_EFFECT;
		SendEvent(MyEvents::E_PLAY_SOUND, data);
	}
    _baseElement->SetVisible(false);
    _baseElement->Remove();
    _baseElement = nullptr;
    _title = nullptr;
    _message = nullptr;
}