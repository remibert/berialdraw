#include "berialdraw_imp.hpp"

using namespace berialdraw;

const char filled_star[] = 
"E=r*7794>13;"
"F=r*2531>13;"
"G=r*4817>13;"
"H=r*6630>13;"
"A=r*1828>13;"
"B=r*2519>13;"
"C=r*2958>13;"
"D=r*958>13;"
"I=r*3113>13;"

".x,y-r;"
".x+A,y-B;"
".x+E,y-F;"
".x+C,y+D;"
".x+G,y+H;"
".x,y+I;"
".x-G,y+H;"
".x-C,y+D;"
".x-E,y-F;"
".x-A,y-B;"
"~;";

const char un_filled_star[] = 
"a=r;"
"R=a;"
"b=t*218>6;"
"r=a-b;"
"E=R*7794>13;"
"F=R*2531>13;"
"G=R*4817>13;"
"H=R*6630>13;"
"A=R*1828>13;"
"B=R*2519>13;"
"C=R*2958>13;"
"D=R*958>13;"
"I=R*3113>13;"

".x,y-R;"
".x+A,y-B;"
".x+E,y-F;"
".x+C,y+D;"
".x+G,y+H;"
".x,y+I;"
".x-G,y+H;"
".x-C,y+D;"
".x-E,y-F;"
".x-A,y-B;"
"~;"

"e=r*7794>13;"
"f=r*2531>13;"
"g=r*4817>13;"
"h=r*6630>13;"
"j=r*1828>13;"
"k=r*2519>13;"
"l=r*2958>13;"
"m=r*958>13;"
"n=r*3113>13;"

".x,y-r;"
".x-j,y-k;"
".x-e,y-f;"
".x-l,y+m;"
".x-g,y+h;"
".x,y+n;"
".x+g,y+h;"
".x+l,y+m;"
".x+e,y-f;"
".x+j,y-k;"
"~;";

Star::Star(Canvas * canvas) :
	Marker(canvas)
{
}

// Render outline
void Star::paint(const Point & shift)
{
	Marker::paint(shift, filled_star, un_filled_star);
}
