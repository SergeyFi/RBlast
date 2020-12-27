#include "ButtonCustomLong.h"

void ButtonCustomLong::setPushTime(float time)
{
	timeRemaining = time;
}

ButtonCustomLong* ButtonCustomLong::create(Size expand, Size safe, float pushTime)
{
	ButtonCustomLong* button = new (std::nothrow) ButtonCustomLong;

	if (button)
	{
		button->init(expand, safe);

		button->autorelease();

		button->setPushTime(pushTime);

		return button;
	}

	CC_SAFE_DELETE(button);

	return nullptr;
}

bool ButtonCustomLong::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (touchInZone(pTouch, Zones::Expand))
	{
		setState(ButtonState::Pushed);

		startTimer();

		return true;
	}

	return false;
}

void ButtonCustomLong::startTimer()
{
	currentTime = 0.0f;

	schedule(schedule_selector(ButtonCustomLong::countDownTimer));
}

void ButtonCustomLong::stopTimer()
{
	unschedule(schedule_selector(ButtonCustomLong::countDownTimer));
}

void ButtonCustomLong::countDownTimer(float deltaTime)
{
	currentTime += deltaTime;

	if (currentTime >= timeRemaining)
	{
		setState(ButtonState::LongPush);

		stopTimer();
	}
}
