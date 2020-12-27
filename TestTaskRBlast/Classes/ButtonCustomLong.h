#ifndef ButtonLong_h
#define ButtonLong_h

#include "ButtonCustom.h"

class ButtonCustomLong : public ButtonCustom
{

public:

	void setPushTime(float time);

	static ButtonCustomLong* create(Size expand, Size safe, float pushTime);

protected:
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent) override;

private:

	using ButtonCustom::create;

	void startTimer();

	void stopTimer();

	void countDownTimer(float deltaTime);

	float timeRemaining;

	float currentTime;
};

#endif // ButtonLong_h