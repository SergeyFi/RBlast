#include "ButtonCustom.h"

ButtonCustom::ButtonCustom()
{
	currentButtonState = ButtonState::Idle;
}

ButtonCustom::~ButtonCustom()
{
	for (auto& element : elements)
	{
		element.element->removeFromParent();
	}

	elements.clear();
}

ButtonCustom* ButtonCustom::create(Size expand, Size safe)
{
	ButtonCustom* button = new (std::nothrow) ButtonCustom;

	if (button)
	{
		button->init(expand, safe);

		button->autorelease();

		return button;
	}

	CC_SAFE_DELETE(button);

	return nullptr;
}

void ButtonCustom::setZone(Size zone, Zones zoneType)
{
	if (zone.width < getContentSize().width || zone.height < getContentSize().height)
	{
		zone = getContentSize();
	}


	if (zoneType == Zones::Expand)
	{
		expandZone = zone;
	}
	else
	{
		safeZone = zone;
	}
}

void ButtonCustom::addElement(Node* item, std::set<ButtonState> states)
{
	if (item)
	{
		elements.push_back({ item, states });
		
		if (!states.empty())
		{
			item->setVisible(states.count(currentButtonState));
		}

		addChild(item);
	}
}

void ButtonCustom::setAction(std::function<void()> customAction)
{
	action = customAction;
}

void ButtonCustom::init(Size expand, Size safe)
{
	setZone(expand, Zones::Expand);
	setZone(safe, Zones::Safe);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ButtonCustom::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ButtonCustom::onTouchMoved, this);	
	listener->onTouchEnded = CC_CALLBACK_2(ButtonCustom::onTouchEnded, this);

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool ButtonCustom::touchInZone(Touch* pTouch, Zones zone)
{
	const auto btnPosition = getPosition();

	Size currentZoneSize;

	if (zone == Zones::Expand)
	{
		currentZoneSize = expandZone;
	}
	else
	{
		currentZoneSize = safeZone;
	}

	auto currentZone = Rect(btnPosition.x - currentZoneSize.width / 2, btnPosition.y - currentZoneSize.height / 2,
		currentZoneSize.width, currentZoneSize.height);

	return currentZone.containsPoint(pTouch->getLocation());
}

bool ButtonCustom::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (touchInZone(pTouch, Zones::Expand))
	{
		setState(ButtonState::Pushed);

		return true;
	}

	return false;
}

void ButtonCustom::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	if (currentButtonState != ButtonState::Idle)
	{
		if (touchInZone(pTouch, Zones::Expand))
		{
			setState(ButtonState::Pushed);
			return;
		}
		else if (touchInZone(pTouch, Zones::Safe))
		{
			setState(ButtonState::Dragout);
			return;
		}
	}

	setState(ButtonState::Idle);
}

void ButtonCustom::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	if (currentButtonState == ButtonState::Pushed || currentButtonState == ButtonState::LongPush)
	{
		if (action)
		{
			action();
		}

		createPushEvent();
	}

	setState(ButtonState::Idle);
}

void ButtonCustom::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
	setState(ButtonState::Idle);
}

void ButtonCustom::setState(ButtonState state)
{
	if (currentButtonState != state)
	{
		currentButtonState = state;

		for (const auto& element : elements)
		{
			if (!element.states.empty())
			{
				element.element->setVisible(element.states.count(state));
			}
		}
	}
}

void ButtonCustom::createPushEvent()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	EventCustom event("CustomButtonEvent");
	EventData eventData{ this, currentButtonState };
	event.setUserData(&eventData);

	dispatcher->dispatchEvent(&event);
}
