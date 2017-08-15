#pragma once

class CInput
{
public:
	CInput() {};

	HWND hwCSGO;

	template< class T, class Y >
	static T Clamp(T const &val, Y const &minVal, Y const &maxVal);

	POINT GetMousePos();
	void GetClicked();
	bool Holding(int, int, int, int);
	bool Hovering(int, int, int, int);
	bool Clicked(int, int, int, int);

private:
	bool bClicked, bMouseD;
}; extern CInput* Input;