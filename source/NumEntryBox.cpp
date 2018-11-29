#include "NumEntryBox.h"

NumEntryBox::NumEntryBox()
{
	presence.x = 0;
	presence.y = 0;
	presence.w = 64;
	presence.h = 64;
	setValue(0);

	visible = true;
	active = true;

	_editing = false;

	//realign();
}

NumEntryBox::~NumEntryBox()
{
}

void NumEntryBox::update()
{
	_btn.update();

	if (active)
	{
		if (_btn.getClick(leftClick) == clickUp)
		{
			_editing = true;
		}
		else
		{
			if (mouse->getClick(leftClick) == clickDown && _editing)
			{
				_editing = false;
			}
		}

		//if focused on this entry box
		if (_editing)
		{
			if (keyboard->getKey(SDLK_BACKSPACE) == keyUp)
			{
				value = value / 10;
				_tb.setText(value);
			}
			else
			{
				for (int i = 0; i < 10; i++)
				{
					if (keyboard->getKey(char(48 + i)) == keyUp)
					{
						//printf("\nnumber - %d: key - %d\n", 0, (int)'0');
						value *= 10;
						value += i;
						_tb.setText(value);
					}
				}
			}
		}
	}
	else _editing = false;
}

void NumEntryBox::draw()
{
	_tb.draw();
}

void NumEntryBox::setValue(int n)
{
	value = n;
	_tb.setText(n);
	presence = _tb.presence;
}

void NumEntryBox::realign()
{
	_btn.setRect(presence);
	_tb.setRect(presence);
}