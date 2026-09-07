#pragma once
class Vector2
{
public:
	float x, y;
public:
	Vector2(float x, float y);							//コンストラクタ
	float Length()const;				//ベクトルの大きさ
	void Normalize();					//ベクトルの正規化
	Vector2 Normalized()const;			//大きさを1にしたベクトルを返す
	Vector2 operator-()const;			//ベクトルを反転したベクトルを返す
	void operator+=(const Vector2& val);//加算
	void operator-=(const Vector2& val);//減算
	void operator*=(const float scale);//ベクトルをn倍
	Vector2 operator+(const Vector2& val)const;//ベクトルの加算
	Vector2 operator-(const Vector2& val)const;//ベクトルの減算
	Vector2 operator*(float scale)const;//ベクトルの乗算
	float Lerp(float a, float b, float t)const;	//線形補間
};

