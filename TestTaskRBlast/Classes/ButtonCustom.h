#ifndef Button_h
#define Button_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <set>

using namespace cocos2d;

enum class ButtonState
{
	Idle,
	Pushed,
	LongPush,
	Dragout
};

enum class Zones
{
	Safe,
	Expand
};

struct Element
{
	Node* element;
	std::set<ButtonState> states;
};

struct EventData
{
	void* button;
	ButtonState buttonState;
};

class ButtonCustom : public ui::Widget
{
public:

	ButtonCustom();

	~ButtonCustom();

	static ButtonCustom* create(Size expand, Size safe);

	void setZone(Size zoneSize, Zones zoneType);

	void addElement(Node* item, std::set<ButtonState> states = {});

	void setAction(std::function<void()> customAction);

protected:

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent) override;

	virtual void onTouchMoved(Touch* pTouch, Event* pEvent) override;

	virtual void onTouchEnded(Touch* pTouch, Event* pEvent) override;

	virtual void onTouchCancelled(Touch* pTouch, Event* pEvent) override;

	bool touchInZone(Touch* pTouch, Zones zone);

	void setState(ButtonState state);

	void init(Size expand, Size safe);

private:

	void createPushEvent();

	Size expandZone;
	Size safeZone;
	ButtonState currentButtonState;

	std::vector<Element> elements;

	std::function<void()> action;
};


#endif // Button_h
