#pragma once
class Button
{
public:
	Button(int centerX, int centerY, int hitWidth, int hitHeight, int imageHandle);

	void Update();
	void Draw() const;

	bool IsHovered() const { return isHover_; }

	//ÉNÉäÉbÉNÇ≥ÇÍÇΩèuä‘Ç©
	bool IsClicked() const; 

private:
	int centerX_;
	int centerY_;
	int hitWidth_;
	int hitHeight_;
	int imageHandle_;

	bool isHover_ = false;
	float scale_ = 1.0f;
};

