#include "RGBA.h"

RGBA::RGBA(BYTE val) :B(val), G(val), R(val), A(255) {}
RGBA::RGBA(BYTE b, BYTE g, BYTE r, BYTE a) :B(b), G(g), R(r), A(a) {}
RGBA::RGBA(const RGBA& b) :B(b.B), G(b.G), R(b.R), A(b.A) {}

RGBA RGBA::Set(const BYTE& val){
	return RGBA(val);
}

void RGBA::Swap(RGBA& a, RGBA& b){
    std::swap(a.B, b.B);
	std::swap(a.G, b.G);
    std::swap(a.R, b.R);
    std::swap(a.A, b.A);
}

RGBA RGBA::White(){
    return RGBA(255,255,255);
}

RGBA RGBA::Black(){
    return RGBA();
}
void RGBA::Gray(){
    R = G = B = (BYTE)round((float)R * 299 / 1000 + (float)G * 587 / 1000 + (float)B * 114 / 1000);
}

RGBA RGBA::operator+(const RGBA& b){
    return RGBA(B + b.B, G + b.G, R + b.R, A + b.A);
}

RGBA RGBA::operator-(const RGBA& b){
    return RGBA(B - b.B, G - b.G, R - b.R, A - b.A);
}

RGBA RGBA::operator*(const RGBA& b) {
    return RGBA(B * b.B, G * b.G, R * b.R, A * b.A);
}

RGBA RGBA::operator/(const RGBA& b) {
    return RGBA(b.B == 0 ? 0 : B / b.B, b.G == 0 ? 0 : G / b.G, b.R == 0 ? 0 : R / b.R, b.A == 0 ? 0 : A / b.A);
}

RGBA& RGBA::operator=(const RGBA& b){
	RGBA temp(b);
	Swap(* this, temp);
	return *this;
}

RGBA& RGBA::operator+=(const RGBA& b) {
	return *this = *this + b;
}

RGBA& RGBA::operator-=(const RGBA& b)
{
	return *this = *this - b;
}

bool RGBA::operator==(const RGBA& b){
	return (R == b.R) && (G == b.G) && (B == b.B) && (A == b.A);
}

bool RGBA::operator!=(const RGBA& b) {
	return !(*this == b);
}

bool RGBA::operator>(const RGBA& b){
	return (R + G + B + A) > (b.R + b.G + b.B + b.A);
}

bool RGBA::operator<(const RGBA& b){
	return (R + G + B + A) < (b.R + b.G + b.B + b.A);
}

std::ostream& operator<<(std::ostream& out, const RGBA& a){
    out <<(int) a.R;
    return out;
}
